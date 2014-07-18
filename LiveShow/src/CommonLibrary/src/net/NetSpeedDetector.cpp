#include "..\..\include\Net\NetSpeedDetector.h"

namespace common
{
	namespace net
	{

	enum
	{	
		MAX_DEFAULT_TIMEOUT = 0xFFFFFFFF,
		TASK_TIMEOUT_TIME = 3*1000,
		NSD_DEFAULT_SEQUENCE_ID = 1,
		NSD_SOCKET_TIMEOUT = 2000,
		NSD_SOCKET_EVENT = 0x3333,
		NSD_TIMEOUT_EVENT = 0x3334,	
		NSD_DEFAULT_SOCKET_BUFFER_LENGTH = 1024,
	};

	#define NSD_MODULE_WINDOW _T("NetSpeedDetectorModuleWindow")

	#pragma pack(1)

	typedef struct icmp_hdr
	{
		unsigned char  type;
		unsigned char  code;
		unsigned short checksum;
		unsigned short id;
		unsigned short sequence;
		unsigned long  timestamp;
	}ICMP_HDR;

	#pragma pack()


	CNetSpeedDetector::CNetSpeedDetector()
	{
		m_hWnd					= NULL;
		m_hSocket				= INVALID_SOCKET;
		m_uSequenceID			= NSD_DEFAULT_SEQUENCE_ID;
		m_pEventHandler			= NULL;
		m_bInitSuccess			= FALSE;
		m_pReceiveBuffer		= NULL;
		m_unReceiveBufferLen	= NSD_DEFAULT_SOCKET_BUFFER_LENGTH;
		
		srand((unsigned)time(0));
		
		::QueryPerformanceFrequency(&m_performanceFrequency);
	}

	CNetSpeedDetector::~CNetSpeedDetector()
	{
		Cleanup();
	}

	BOOL CNetSpeedDetector::Initialize(const INetSpeedDetectorEvent * pEventHandler)
	{
		BOOL bReturn = FALSE;

		if (m_pEventHandler == NULL && !m_bInitSuccess)
		{
			m_pEventHandler = (INetSpeedDetectorEvent *)pEventHandler;
			m_bInitSuccess = FALSE;
			bReturn = TRUE;
		}

		// 初始化网络、窗口和SOCKET对象
		if (bReturn && CreateNSDWindow() && CreateSocket())
		{
			m_bInitSuccess = TRUE;
			bReturn = TRUE;
		}
	//	OutputTraceInfo(_T("Initialize bool = %d, m_socket = %d"), bReturn, m_hSocket);
		return bReturn;
	}

	void CNetSpeedDetector::Detect(std::vector<std::wstring>& vecIPArray)
	{
		m_index = 0;

		SetTimeoutTimer();

		std::vector<bool> vecIndex;
		int size = vecIPArray.size();
		for(int i=0;i<size;i++)
		{
			vecIndex.push_back(false);
		}
		for(int j=0;j<size;j++)
		{
			while(1)
			{
				int index = rand()%size;
				if(!vecIndex[index])
				{
					SendRequest(vecIPArray[index]);
					vecIndex[index] = true;
					break;
				}
			}
		}
	}
	
	BOOL CNetSpeedDetector::Finalize()
	{
		BOOL bReturn = FALSE;

		if (m_bInitSuccess)
		{
			Cleanup();

			bReturn = TRUE;
		}

		m_pEventHandler = NULL;
		m_hWnd = NULL;

		return bReturn;
	}

	void CNetSpeedDetector::OnRead(SOCKET hSocket, int nErrorCode)
	{
		USES_CONVERSION;

		DWORD timestamp	= ::GetTickCount();

		BOOL bReturn = FALSE;

		if (m_hSocket != INVALID_SOCKET && m_pReceiveBuffer != NULL && m_bInitSuccess)
		{
			bReturn = TRUE;
		}

		if (bReturn)
		{
			sockaddr_in from = {0};
			int nFromLength = sizeof(from);
			int nErroCode = 0;
			memset(m_pReceiveBuffer, 0, m_unReceiveBufferLen);

			int nRead = recvfrom(m_hSocket, (char *)m_pReceiveBuffer, 
				m_unReceiveBufferLen, 0, (sockaddr *)&from, &nFromLength);

			if(nRead == SOCKET_ERROR)
			{
				nErroCode  = WSAGetLastError();
			}

		//	OutputTraceInfo(_T("from [%s:%d] recv udp [%d]bytes m_hSocket = %d"), 
		//		inet_ntoa(*((in_addr *)&from.sin_addr.S_un.S_addr)),
		//		ntohs(from.sin_port),
		//		nRead, 
		//		m_hSocket
		//		);

			if (nRead > sizeof(ICMP_HDR)+20)
			{
				LARGE_INTEGER currentTime;
				QueryPerformanceCounter(&currentTime);

				ICMP_HDR * pPDU = (ICMP_HDR *)(m_pReceiveBuffer+20);
				
				if (pPDU->type == 0 && pPDU->id == (USHORT)::GetCurrentProcessId())
				{
					DWORD dwTimeout = timestamp - pPDU->timestamp;
					std::wstring strIP = A2T(inet_ntoa(*((in_addr *)&from.sin_addr.S_un.S_addr)));

					for(std::vector<IP2Time>::iterator it = m_mapIP2BeginTime.begin();
						it != m_mapIP2BeginTime.end(); ++it)
					{						
						if(wcscmp(it->strIP.c_str(), strIP.c_str()) == 0)
						{
							//wchar_t sz[100];
							//wsprintf(sz, L" %I64d %I64d %I64d", currentTime.QuadPart, it->nTime.QuadPart, m_performanceFrequency.QuadPart);
							//OutputDebugString(sz);

							DWORD dwTimeout = (double)(currentTime.QuadPart-it->nTime.QuadPart)*1000.0/
								(double)m_performanceFrequency.QuadPart;
							m_pEventHandler->OnDetectReply(strIP, dwTimeout, m_index++);
							break;
						}						
					}
				}
			}
			else
			{
				bReturn = FALSE;
				//OutputErrorInfo(_T("pdu size error,not support"));
			}

			if(nRead == SOCKET_ERROR)
			{
				//OutputTraceInfo(_T("socket error, errorcode = %d, close socket"), nErroCode);
				//OnErrorToCloseDetect();	
				bReturn = FALSE;
			}
		}
	}

	void CNetSpeedDetector::OnTimeout()
	{
		KillTimeoutTimer();

		m_pEventHandler->OnDetectTimeout();
	}

	unsigned short checksum(unsigned short *addr, int len)  
	{  
		int nleft=len;  
		int sum=0;  
		unsigned short *w=addr;  
		unsigned short answer=0;  
		while(nleft > 1)  
		{  
			sum += *w++;  
			nleft -= 2;  
		}  
		if( nleft == 1)  
		{  
			*(unsigned char *)(&answer) = *(unsigned char *)w;  
			sum += answer;  
		}  
		sum = (sum >> 16) + (sum & 0xffff);  
		sum += (sum >> 16);  
		answer = ~sum;  
		return answer;  
	}

	BOOL CNetSpeedDetector::SendRequest(std::wstring& strIP)
	{
		USES_CONVERSION;

		BOOL bReturn = m_hSocket != INVALID_SOCKET;

		char buffer[sizeof(ICMP_HDR)+32];

		if (bReturn)
		{
			ICMP_HDR* pdu = (ICMP_HDR*)buffer;
			sockaddr_in dest = {0};

			pdu->type		= 8;
			pdu->code		= 0;
			pdu->id			= (USHORT)::GetCurrentProcessId();
			pdu->checksum	= 0;
			pdu->timestamp	= ::GetTickCount();
			pdu->sequence   = GetNextSequenceID();
			pdu->checksum	= 0;
			pdu->checksum	= checksum((USHORT*)buffer, sizeof(buffer));

			dest.sin_family		 = AF_INET;
			dest.sin_addr.s_addr = inet_addr(T2A((LPTSTR)strIP.c_str()));
			dest.sin_port		 = htons(0);

			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);

			IP2Time st;
			st.nTime = currentTime;
			st.strIP = strIP;
			m_mapIP2BeginTime.push_back(st);

			int nSentBytes = sendto(m_hSocket, (const char *)buffer, sizeof(buffer), 0, 
				(sockaddr *)&dest, sizeof(dest));

			bReturn = nSentBytes != SOCKET_ERROR;
		}

		return bReturn;
	}

	BOOL CNetSpeedDetector::DestoryNSDWindow()
	{
		BOOL bReturn = FALSE;

		if (IsWindow(m_hWnd))
		{
			::DestroyWindow(m_hWnd);
			m_hWnd = NULL;
			UnregisterClass(NSD_MODULE_WINDOW, NULL);
		}
		
		bReturn = m_hWnd == NULL;

		return bReturn;
	}

	BOOL CNetSpeedDetector::CreateNSDWindow()
	{
		BOOL bReturn = FALSE;

		UnregisterClass(NSD_MODULE_WINDOW, NULL);

		WNDCLASS wndclass = {0};

		wndclass.style			= 0;
		wndclass.lpfnWndProc	= NSDWindowWinProc;
		wndclass.cbClsExtra		= 0 ;
		wndclass.cbWndExtra		= 0 ;
		wndclass.hInstance		= 0 ;
		wndclass.hIcon			= 0 ;
		wndclass.hCursor		= 0 ;
		wndclass.hbrBackground	= 0 ;
		wndclass.lpszClassName	= NSD_MODULE_WINDOW;

		RegisterClass(&wndclass); // todo: 这里注册会失败 gw?

		m_hWnd = CreateWindow(
			NSD_MODULE_WINDOW,	// window class name
			NSD_MODULE_WINDOW,	// window caption
			NULL,				// window style
			CW_USEDEFAULT,		// initial x position
			CW_USEDEFAULT,		// initial y position
			CW_USEDEFAULT,		// initial x size
			CW_USEDEFAULT,		// initial y size
			NULL,				// parent window handle
			NULL,			    // window menu handle
			NULL,				// program instance handle
			NULL 				// creation parameters
			);
		bReturn = m_hWnd != NULL;

		if (bReturn)
		{
			SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
		}

		return bReturn;
	}

	BOOL CNetSpeedDetector::CreateSocket()
	{
		BOOL bReturn = FALSE;

		if (m_hSocket == INVALID_SOCKET && m_hWnd != NULL)
		{
			bReturn = TRUE;
		}

		// 1 创建一个udp的Socket
		if (bReturn)
		{
			m_hSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
			bReturn = m_hSocket != INVALID_SOCKET;
		}

		// 2 设置为异步Socket
		if (bReturn)
		{
			ULONG uCmd = 1;
			bReturn = ioctlsocket(m_hSocket, FIONBIO, &uCmd) != SOCKET_ERROR;
		}

		// 3 根据系统默认大小创建接收缓冲区
		if (bReturn)
		{
			int nError = 0;
			int	nOptlen = 4;
			char * pOptValue = (char *)&m_unReceiveBufferLen;

			nError = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, pOptValue, &nOptlen);

			if (nError == SOCKET_ERROR)
			{
				m_unReceiveBufferLen = NSD_DEFAULT_SOCKET_BUFFER_LENGTH;
			}

			// 建立接收缓冲区
			m_pReceiveBuffer = new BYTE[m_unReceiveBufferLen];
			memset(m_pReceiveBuffer, 0, m_unReceiveBufferLen);

			bReturn = m_pReceiveBuffer != NULL;
		}

		// 4 设置超时
		if (bReturn)
		{
			bReturn = SetSocketTimeout(NSD_SOCKET_TIMEOUT);
		}

		// 5 要求接收READ事件
		if (bReturn)
		{
			LONG lEvents = FD_READ;
			
			bReturn = WSAAsyncSelect(m_hSocket, m_hWnd, NSD_SOCKET_EVENT, lEvents) != SOCKET_ERROR;
		}

		return bReturn;
	}
	
	int CNetSpeedDetector::CloseSocket()
	{
		BOOL bReturn = FALSE;

		if (m_hSocket != INVALID_SOCKET)
		{
			bReturn = closesocket(m_hSocket) != SOCKET_ERROR;
			m_hSocket = INVALID_SOCKET;
		}

		bReturn = m_hSocket == INVALID_SOCKET;
		return bReturn;
	}

	BOOL CNetSpeedDetector::SetSocketTimeout(DWORD dwTimeout)
	{
		BOOL bReturn = FALSE;

		if (m_hSocket != INVALID_SOCKET)
		{
			bReturn = TRUE;
		}

		if (bReturn)
		{
			bReturn = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, 
				(char *)&dwTimeout, sizeof(dwTimeout)) != SOCKET_ERROR;
		}

		if (bReturn)
		{
			bReturn = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, 
				(char *)&dwTimeout, sizeof(dwTimeout)) != SOCKET_ERROR;
		}

		return bReturn;
	}

	BOOL CNetSpeedDetector::SetTimeoutTimer()
	{
		BOOL bReturn = FALSE;
		
		if (m_hWnd != NULL)
		{
			bReturn = SetTimer(m_hWnd, NSD_TIMEOUT_EVENT, TASK_TIMEOUT_TIME, NULL) != 0;
		}

		return bReturn;
	}

	BOOL CNetSpeedDetector::KillTimeoutTimer()
	{
		BOOL bReturn = FALSE;
		
		if (m_hWnd != NULL)
		{
			bReturn = KillTimer(m_hWnd, NSD_TIMEOUT_EVENT) != 0;
		}

		return bReturn;
	}

	void CNetSpeedDetector::Cleanup()
	{
		CloseSocket();

		KillTimeoutTimer();

		DestoryNSDWindow();

		if (m_pReceiveBuffer != NULL)
		{
			delete m_pReceiveBuffer;
			m_pReceiveBuffer = NULL;
		}
	}

	void CNetSpeedDetector::OnErrorToCloseDetect()
	{
		Cleanup();
	}

	LRESULT CALLBACK NSDWindowWinProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		SOCKET hEventSocket = (SOCKET)wParam;
		int nErrorCode		= WSAGETSELECTERROR(lParam);
		LONG lWindowData	= ::GetWindowLong(hWnd, GWL_USERDATA);
		CNetSpeedDetector * pNetSpeedDetector = (CNetSpeedDetector *)lWindowData;

		switch (nMessage)
		{
		case NSD_SOCKET_EVENT:
			if (pNetSpeedDetector != NULL)
			{
				switch (WSAGETSELECTEVENT(lParam))
				{
				case FD_READ:
					pNetSpeedDetector->OnRead(hEventSocket, nErrorCode);
					break;
				default:
					break;
				}
			}
			break;
		case WM_TIMER:
			if (pNetSpeedDetector != NULL && wParam == NSD_TIMEOUT_EVENT)
			{
				pNetSpeedDetector->OnTimeout();
			}
			break;
		default:
			break;
		}

		return DefWindowProc (hWnd, nMessage, wParam, lParam);
	}

	}
}