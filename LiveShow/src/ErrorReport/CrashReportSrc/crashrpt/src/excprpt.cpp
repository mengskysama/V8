///////////////////////////////////////////////////////////////////////////////
//
//  Module: excprpt.cpp
//
//    Desc: See excprpt.h
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#include "../../../stdafx.h"
#include "excprpt.h"
#include "utility.h"

CString CExceptionReport::m_sModule = _T("");
CString CExceptionReport::m_sException = _T("");
CString CExceptionReport::m_sAddress = _T("");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// CExceptionReport::CExceptionReport
//
// 
//
CExceptionReport::CExceptionReport(PEXCEPTION_POINTERS ExceptionInfo)
{
   m_excpInfo = ExceptionInfo;
}


//-----------------------------------------------------------------------------
// CExceptionReport::getCrashFile
//
// Creates the dump file returning the file name
//
CString CExceptionReport::getCrashFile()
{
   CString sFile;

   // Create the dump file name
   sFile.Format(_T("%s\\%s.dmp"), getenv("TEMP"), CUtility::getAppName());

   // Create the file
   HANDLE hFile = CreateFile(
      sFile,
      GENERIC_WRITE,
      0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL);

   //
   // Write the minidump to the file
   //
   if (hFile)
   {
      MINIDUMP_EXCEPTION_INFORMATION eInfo;
      eInfo.ThreadId = GetCurrentThreadId();
      eInfo.ExceptionPointers = m_excpInfo;
      eInfo.ClientPointers = FALSE;

      MINIDUMP_CALLBACK_INFORMATION cbMiniDump;
      cbMiniDump.CallbackRoutine = CExceptionReport::miniDumpCallback;
      cbMiniDump.CallbackParam = 0;

      MiniDumpWriteDump(
         GetCurrentProcess(),
         GetCurrentProcessId(),
         hFile,
         MiniDumpNormal,
         m_excpInfo ? &eInfo : NULL,
         NULL,
         &cbMiniDump);
   }

   // Close file
   CloseHandle(hFile);

   return sFile;
}


//-----------------------------------------------------------------------------
// CExceptionReport::getCrashLog
//
// Creates the XML log file returning the name
//
CString CExceptionReport::getCrashLog()
{
   CString sFile;
   IXMLDOMDocument *pDoc  = NULL;
   IXMLDOMNode *root      = NULL;
   IXMLDOMNode *node      = NULL;
   IXMLDOMNode *newNode   = NULL;
   BSTR rootName = ::SysAllocString(L"Exception");
   VARIANT v;

   CoInitialize(NULL);

   // Create an empty XML document
   CHECKHR(CoCreateInstance(
      CLSID_DOMDocument, 
      NULL, 
      CLSCTX_INPROC_SERVER,
      IID_IXMLDOMDocument, 
      (void**)&pDoc));

   // Create root node
   root = CreateDOMNode(pDoc, NODE_ELEMENT, rootName);

   //
   // Add exception record node
   //
   if (m_excpInfo)
   {
      node = CreateExceptionRecordNode(pDoc, m_excpInfo->ExceptionRecord);
      CHECKHR(root->appendChild(node, &newNode));
      // The XML Document should now own the node.
      SAFERELEASE(node);
      SAFERELEASE(newNode);
   }

   //
   // Add processor node
   //
   node = CreateProcessorNode(pDoc);
   CHECKHR(root->appendChild(node, &newNode));
   // The XML Document should now own the node.
   SAFERELEASE(node);
   SAFERELEASE(newNode);

   //
   // Add OS node
   //
   node = CreateOSNode(pDoc);
   CHECKHR(root->appendChild(node, &newNode));
   // The XML Document should now own the node.
   SAFERELEASE(node);
   SAFERELEASE(newNode);

   //
   // Add modules node
   //
   node = CreateModulesNode(pDoc);
   CHECKHR(root->appendChild(node, &newNode));
   // The XML Document should now own the node.
   SAFERELEASE(node);
   SAFERELEASE(newNode);

   // Add the root to the doc
   CHECKHR(pDoc->appendChild(root, NULL));

   //
   // Create dat file name and save
   //
   sFile.Format(_T("%s\\%s.xml"), getenv("TEMP"), CUtility::getAppName());
   V_VT(&v) = VT_BSTR;
   V_BSTR(&v) = sFile.AllocSysString();
   pDoc->save(v);
   SysFreeString(V_BSTR(&v));

CleanUp:
   SAFERELEASE(pDoc);
   SAFERELEASE(root);
   SAFERELEASE(node);
   SAFERELEASE(newNode);
   SysFreeString(rootName);

   CoUninitialize();

   return sFile;
}

CString CExceptionReport::getDescriptionFile( CString strAccount, CString strDesc, CString strVersion )
{
	CString sFile;

	// Create the dump file name
	sFile.Format(_T("%s\\Description.txt"), getenv("TEMP"));

	// Create the file
	HANDLE hFile = CreateFile(
		sFile,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile)
	{
		CString strTime;
		DWORD dwWriteByte;

		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		strTime.Format( "%4d-%02d-%02d %02d:%02d:%02d:%03d ÐÇÆÚ%1d\r\n",sys.wYear,sys.wMonth,
			sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
		WriteFile(hFile, (LPVOID)strTime.GetBuffer(0), strTime.GetLength(), (LPDWORD) &dwWriteByte,  NULL);

		strAccount = CString(_T("²Ù×÷ÕËºÅ:")) + strAccount + CString("\r\n");
		WriteFile(hFile, (LPVOID)strAccount.GetBuffer(0), strAccount.GetLength(), (LPDWORD) &dwWriteByte,  NULL);

		strDesc = CString(_T("²Ù×÷ÃèÊö:\r\n")) + strDesc + CString("\r\n");
		WriteFile(hFile, (LPVOID)strDesc.GetBuffer(0), strDesc.GetLength(), (LPDWORD) &dwWriteByte,  NULL);

		strVersion = CString(_T("°×ÍÃKTV°æ±¾ºÅ:\r\n")) + strVersion + CString("\r\n");
		WriteFile(hFile, (LPVOID)strVersion.GetBuffer(0), strVersion.GetLength(), (LPDWORD) &dwWriteByte,  NULL);
	}

	// Close file
	CloseHandle(hFile);

	return sFile;
}

//-----------------------------------------------------------------------------
// CExceptionReport::getNumSymbolFiles
//
// Returns the number of symbols files found
//
int CExceptionReport::getNumSymbolFiles()
{
   return m_symFiles.GetSize();
}

//-----------------------------------------------------------------------------
// CExceptionReport::getSymbolFile
//
// Returns the symbol file name given an index
//
CString CExceptionReport::getSymbolFile(int index)
{
   CString ret;

   if (0 < index && index < m_symFiles.GetSize())
      ret = m_symFiles[index];

   return ret;
}

//-----------------------------------------------------------------------------
// CExceptionReport::CreateDOMNode
//
// Helper function 
//
IXMLDOMNode*
CExceptionReport::CreateDOMNode(IXMLDOMDocument* pDoc, 
                                int type, 
                                BSTR bstrName)
{
    IXMLDOMNode * node;
    VARIANT vtype;

    vtype.vt = VT_I4;
    V_I4(&vtype) = (int)type;

    pDoc->createNode(vtype, bstrName, NULL, &node);
    return node;
}

//-----------------------------------------------------------------------------
// CExceptionReport::CreateExceptionRecordNode
//
//
//
IXMLDOMNode*
CExceptionReport::CreateExceptionRecordNode(IXMLDOMDocument* pDoc, 
                                            EXCEPTION_RECORD* pExceptionRecord)
{
   IXMLDOMNode*     pNode    = NULL;
   IXMLDOMElement*  pElement = NULL;
   BSTR nodeName                    = ::SysAllocString(L"ExceptionRecord");
   BSTR modName                     = ::SysAllocString(L"ModuleName");
   BSTR codeName                    = ::SysAllocString(L"ExceptionCode");
   BSTR descName                    = ::SysAllocString(L"ExceptionDescription");
   BSTR addrName                    = ::SysAllocString(L"ExceptionAddress");
   VARIANT v;
   CString sAddr;

   // Create exception record node
   pNode = CreateDOMNode(pDoc, NODE_ELEMENT, nodeName);

   // Get element interface
   CHECKHR(pNode->QueryInterface(IID_IXMLDOMElement, (void**)&pElement));

   //
   // Set module name attribute
   //
   TCHAR szModName[_MAX_FNAME + 1];
   GetModuleFileName(NULL, szModName, _MAX_FNAME);
   m_sModule = szModName;
   V_VT(&v)    = VT_BSTR;
   V_BSTR(&v)  = A2BSTR(szModName);
   pElement->setAttribute(modName, v);
   // Recycle variant
   SysFreeString(V_BSTR(&v));

   //
   // Set exception code
   //
   sAddr.Format(_T("%#x"), pExceptionRecord->ExceptionCode);
   m_sException = sAddr;
   V_VT(&v)    = VT_BSTR;
   V_BSTR(&v)  = sAddr.AllocSysString();
   pElement->setAttribute(codeName, v);
   // Recycle variant
   SysFreeString(V_BSTR(&v));

   //
   // Set exception description
   //
   V_VT(&v)    = VT_BSTR;
   switch (pExceptionRecord->ExceptionCode)
   {
   case EXCEPTION_ACCESS_VIOLATION:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_ACCESS_VIOLATION");
      break;
   case EXCEPTION_DATATYPE_MISALIGNMENT:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_DATATYPE_MISALIGNMENT");
      break;
   case EXCEPTION_BREAKPOINT:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_BREAKPOINT");
      break;
   case EXCEPTION_SINGLE_STEP:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_SINGLE_STEP");
      break;
   case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
      break;
   case EXCEPTION_FLT_DENORMAL_OPERAND:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_DENORMAL_OPERAND");
      break;
   case EXCEPTION_FLT_DIVIDE_BY_ZERO:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_DIVIDE_BY_ZERO");
      break;
   case EXCEPTION_FLT_INEXACT_RESULT:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_INEXACT_RESULT");
      break;
   case EXCEPTION_FLT_INVALID_OPERATION:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_INVALID_OPERATION");
      break;
   case EXCEPTION_FLT_OVERFLOW:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_OVERFLOW");
      break;
   case EXCEPTION_FLT_STACK_CHECK:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_STACK_CHECK");
      break;
   case EXCEPTION_FLT_UNDERFLOW:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_FLT_UNDERFLOW");
      break;
   case EXCEPTION_INT_DIVIDE_BY_ZERO:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_INT_DIVIDE_BY_ZERO");
      break;
   case EXCEPTION_INT_OVERFLOW:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_INT_OVERFLOW");
      break;
   case EXCEPTION_PRIV_INSTRUCTION:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_PRIV_INSTRUCTION");
      break;
   case EXCEPTION_IN_PAGE_ERROR:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_IN_PAGE_ERROR");
      break;
   case EXCEPTION_ILLEGAL_INSTRUCTION:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_ILLEGAL_INSTRUCTION");
      break;
   case EXCEPTION_NONCONTINUABLE_EXCEPTION:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_NONCONTINUABLE_EXCEPTION");
      break;
   case EXCEPTION_STACK_OVERFLOW:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_STACK_OVERFLOW");
      break;
   case EXCEPTION_INVALID_DISPOSITION:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_INVALID_DISPOSITION");
      break;
   case EXCEPTION_GUARD_PAGE:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_GUARD_PAGE");
      break;
   case EXCEPTION_INVALID_HANDLE:
      V_BSTR(&v) = ::SysAllocString(L"EXCEPTION_INVALID_HANDLE");
      break;
   default:
      V_BSTR(&v) = L"EXCEPTION_UNKNOWN";
      break;
   }
   pElement->setAttribute(descName, v);
   // Recycle variant
   SysFreeString(V_BSTR(&v));

   //
   // Set exception address
   //
   sAddr.Format(_T("%#x"), pExceptionRecord->ExceptionAddress);
   m_sAddress = sAddr;
   V_VT(&v)    = VT_BSTR;
   V_BSTR(&v)  = sAddr.AllocSysString();
   pElement->setAttribute(addrName, v);
   // Recycle variant
   SysFreeString(V_BSTR(&v));

CleanUp:
   ::SysFreeString(nodeName);
   ::SysFreeString(modName);
   ::SysFreeString(codeName);
   ::SysFreeString(addrName);
   SAFERELEASE(pElement);

   return pNode;
}

//-----------------------------------------------------------------------------
// CExceptionReport::CreateProcessorNode
//
//
//
IXMLDOMNode*
CExceptionReport::CreateProcessorNode(IXMLDOMDocument* pDoc)
{
   IXMLDOMNode*     pNode    = NULL;
   IXMLDOMElement*  pElement = NULL;
   BSTR nodeName                    = ::SysAllocString(L"Processor");
   BSTR archName                    = ::SysAllocString(L"Architecture");
   BSTR levelName                   = ::SysAllocString(L"Level");
   BSTR numberName                  = ::SysAllocString(L"NumberOfProcessors");
   SYSTEM_INFO si;
   VARIANT v;

   // Create exception record node
   pNode = CreateDOMNode(pDoc, NODE_ELEMENT, nodeName);

   // Get element interface
   CHECKHR(pNode->QueryInterface(IID_IXMLDOMElement, (void**)&pElement));

   //
   // Get processor info
   //
   GetSystemInfo(&si);

   //
   // Set architecture
   //
   V_VT(&v) = VT_BSTR;
   switch (si.wProcessorArchitecture)
   {
   case PROCESSOR_ARCHITECTURE_INTEL:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_INTEL");
      break;
   case PROCESSOR_ARCHITECTURE_MIPS:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_MIPS");
      break;
   case PROCESSOR_ARCHITECTURE_ALPHA:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_ALPHA");
      break;
   case PROCESSOR_ARCHITECTURE_PPC:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_PPC");
      break;
   case PROCESSOR_ARCHITECTURE_SHX:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_SHX");
      break;
   case PROCESSOR_ARCHITECTURE_ARM:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_ARM");
      break;
   case PROCESSOR_ARCHITECTURE_IA64:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_IA64");
      break;
   case PROCESSOR_ARCHITECTURE_ALPHA64:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_ALPHA64");
      break;
   case PROCESSOR_ARCHITECTURE_UNKNOWN:
      V_BSTR(&v) = ::SysAllocString(L"PROCESSOR_ARCHITECTURE_UNKNOWN");
      break;
   default:
      V_BSTR(&v) = ::SysAllocString(L"Unknown");
   }
   pElement->setAttribute(archName, v);
   // Recycle variant
   SysFreeString(V_BSTR(&v));

   //
   // Set level
   //
   V_VT(&v) = VT_BSTR;
   if (PROCESSOR_ARCHITECTURE_INTEL == si.wProcessorArchitecture)
   {
      switch (si.wProcessorLevel)
      {
      case 3:
         V_BSTR(&v) = ::SysAllocString(L"Intel 30386");
         break;
      case 4:
         V_BSTR(&v) = ::SysAllocString(L"Intel 80486");
         break;
      case 5:
         V_BSTR(&v) = ::SysAllocString(L"Intel Pentium");
         break;
      case 6:
         V_BSTR(&v) = ::SysAllocString(L"Intel Pentium Pro or Pentium II");
         break;
      default:
         V_BSTR(&v) = ::SysAllocString(L"Unknown");
      }
   }
   pElement->setAttribute(levelName, v);
   // Recycle variant
   SysFreeString(V_BSTR(&v));

   //
   // Set num of processors
   //
   V_VT(&v) = VT_I4;
   V_I4(&v) = si.dwNumberOfProcessors;
   pElement->setAttribute(numberName, v);

CleanUp:
   ::SysFreeString(nodeName);
   ::SysFreeString(archName);
   ::SysFreeString(levelName);
   ::SysFreeString(numberName);
   SAFERELEASE(pElement);

   return pNode;
}

//-----------------------------------------------------------------------------
// CExceptionReport::CreateOSNode
//
//
//
IXMLDOMNode* 
CExceptionReport::CreateOSNode(IXMLDOMDocument* pDoc)
{
   IXMLDOMNode*     pNode    = NULL;
   IXMLDOMElement*  pElement = NULL;
   BSTR nodeName                    = ::SysAllocString(L"OperatingSystem");
   BSTR majorName                   = ::SysAllocString(L"MajorVersion");
   BSTR minorName                   = ::SysAllocString(L"MinorVersion");
   BSTR buildName                   = ::SysAllocString(L"BuildNumber");
   BSTR csdName                     = ::SysAllocString(L"CSDVersion");
   OSVERSIONINFO oi;
   VARIANT v;

   // Create exception record node
   pNode = CreateDOMNode(pDoc, NODE_ELEMENT, nodeName);

   // Get element interface
   CHECKHR(pNode->QueryInterface(IID_IXMLDOMElement, (void**)&pElement));

   //
   // Get OS info
   //
   oi.dwOSVersionInfoSize = sizeof(oi);
   GetVersionEx(&oi);

   //
   // Set major version
   //
   V_VT(&v) = VT_I4;
   V_I4(&v) = oi.dwMajorVersion;
   pElement->setAttribute(majorName, v);

   //
   // Set minor version
   //
   V_VT(&v) = VT_I4;
   V_I4(&v) = oi.dwMinorVersion;
   pElement->setAttribute(minorName, v);

   //
   // Set build version
   //
   V_VT(&v) = VT_I4;
   V_I4(&v) = oi.dwBuildNumber;
   pElement->setAttribute(buildName, v);

   //
   // Set CSD version
   //
   V_VT(&v) = VT_BSTR;
   V_BSTR(&v) = A2BSTR(oi.szCSDVersion);
   pElement->setAttribute(csdName, v);
   ::SysFreeString(V_BSTR(&v));

CleanUp:
   ::SysFreeString(nodeName);
   ::SysFreeString(majorName);
   ::SysFreeString(minorName);
   ::SysFreeString(buildName);
   ::SysFreeString(csdName);
   SAFERELEASE(pElement);

   return pNode;
}

DWORD CExceptionReport::GetFileSize(LPCWSTR szFilePath)
{
	DWORD dwRet = 0;

	HANDLE hFile = CreateFileW(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		dwRet = ::GetFileSize(hFile, NULL);
		if (INVALID_FILE_SIZE == dwRet)
		{
			//DWORD dwError=GetLastError();
		}

		CloseHandle(hFile);
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// CExceptionReport::CreateModulesNode
//
//
//
IXMLDOMNode* 
CExceptionReport::CreateModulesNode(IXMLDOMDocument* pDoc)
{
   IXMLDOMNode*     pNode    = NULL;
   IXMLDOMNode*     pNode2   = NULL;
   IXMLDOMNode*     pNewNode = NULL;
   IXMLDOMElement*  pElement = NULL;
   IXMLDOMElement*  pElement2= NULL;
   BSTR nodeName                    = ::SysAllocString(L"Modules");
   BSTR nodeName2                   = ::SysAllocString(L"Module");
   BSTR fullPath                    = ::SysAllocString(L"FullPath");   
   BSTR sizeName                    = ::SysAllocString(L"Size");
   BSTR timeStampName               = ::SysAllocString(L"TimeStamp");
   BSTR fileVerName                 = ::SysAllocString(L"FileVersion");

   CString sAddr;
   VARIANT v;


   // Create modules node
   pNode = CreateDOMNode(pDoc, NODE_ELEMENT, nodeName);

   //
   // Add module information
   // 
   node = start.next;
   while (node)
   {
      // Create module node
      pNode2 = CreateDOMNode(pDoc, NODE_ELEMENT, nodeName2);

      // Get element interface
      CHECKHR(pNode2->QueryInterface(IID_IXMLDOMElement, (void**)&pElement));

      //
      // Set full path
      //
      V_VT(&v) = VT_BSTR;
      V_BSTR(&v) = W2BSTR(node->item.FullPath);
      pElement->setAttribute(fullPath, v);
      // Recycle variant
      SysFreeString(V_BSTR(&v));
    
      //
      // Set module size
      //
      sAddr.Format(_T("%d"), GetFileSize(node->item.FullPath));
      V_VT(&v) = VT_BSTR;
      V_BSTR(&v) = sAddr.AllocSysString();
      pElement->setAttribute(sizeName, v);
      // Recycle variant
      SysFreeString(V_BSTR(&v));

      //
      // Set timestamp
      //
	  CString str(node->item.FullPath);
	  FILETIME    ft = CUtility::getLastWriteFileTime(str);
      SYSTEMTIME  st = {0};

      FileTimeToSystemTime(&ft, &st);

      sAddr.Format(
         _T("%02u/%02u/%04u %02u:%02u:%02u"), 
         st.wMonth, 
         st.wDay, 
         st.wYear, 
         st.wHour, 
         st.wMinute, 
         st.wSecond);

      V_VT(&v) = VT_BSTR;
      V_BSTR(&v) = sAddr.AllocSysString();
      pElement->setAttribute(timeStampName, v);
      // Recycle variant
      SysFreeString(V_BSTR(&v));

      //
      // Set file version
      //
      sAddr.Format("%d.%d.%d.%d", 
                   HIWORD(node->item.VersionInfo.dwFileVersionMS),
                   LOWORD(node->item.VersionInfo.dwFileVersionMS),
                   HIWORD(node->item.VersionInfo.dwFileVersionLS),
                   LOWORD(node->item.VersionInfo.dwFileVersionLS));
      V_VT(&v) = VT_BSTR;
      V_BSTR(&v) = sAddr.AllocSysString();
      pElement->setAttribute(fileVerName, v);
      // Recycle variant
      SysFreeString(V_BSTR(&v));

      //
      // Append module to modules
      //
      pNode->appendChild(pNode2, &pNewNode);
      // The XML Document should now own the node.
      SAFERELEASE(pNode2);
      SAFERELEASE(pElement2);
      SAFERELEASE(pNewNode);

      node = node->next;
   }

CleanUp:

   ::SysFreeString(nodeName);
   ::SysFreeString(nodeName2);
   ::SysFreeString(fullPath);
   ::SysFreeString(sizeName);
   ::SysFreeString(timeStampName);
   ::SysFreeString(fileVerName);
   SAFERELEASE(pNode2);
   SAFERELEASE(pNewNode);
   SAFERELEASE(pElement);
   SAFERELEASE(pElement2);

   return pNode;
}

//-----------------------------------------------------------------------------
// CExceptionReport::miniDumpCallback
//
// Mini dump module callback.  Hit once for each module processed by
// MiniDumpWriteDump.  Builds a linked list of all module names which is
// eventually used to create the <modules> node in the XML log file.
//
BOOL CALLBACK 
CExceptionReport::miniDumpCallback(PVOID,
                                   CONST PMINIDUMP_CALLBACK_INPUT CallbackInput,
                                   PMINIDUMP_CALLBACK_OUTPUT)
{
   if (ModuleCallback == CallbackInput->CallbackType)
   {
	   try
	   {
		   node->next=NULL;
		node->next = new _ModuleListEntry;
		node = node->next;
		node->next = NULL;
		node->item = CallbackInput->Module;
		node->item.FullPath = _wcsdup(CallbackInput->Module.FullPath);
	   }
	   catch(...)
	   {
	   }
   }

   return TRUE;
}