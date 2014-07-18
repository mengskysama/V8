#pragma once
#include "..\Resource.h"
#include "ScreenEffectManager.h"

// CFlyingTextDlg 对话框
struct TrackInfo 
{
	int nIndex;
	int nStartPosY;
	int nCurPosX;
	bool bInUse;
	std::wstring strNickName;
	std::wstring strContent;
	Font * pNickNameFont;
	Font * pSayFont;
	Font * pContentFont;
	int nImageIndex;
	std::vector<Image*> vecImageList;
	flying_text_info * pFlyingInfo;

	TrackInfo() : nIndex(0), nStartPosY(0), nCurPosX(0), bInUse(false), strNickName(L""), strContent(L""), pNickNameFont(NULL), pSayFont(NULL), pContentFont(NULL), nImageIndex(0), pFlyingInfo(NULL)
	{

	}

	void Release()
	{
		if (pNickNameFont != NULL)
		{
			delete pNickNameFont;
			pNickNameFont = NULL;
		}

		if (pSayFont != NULL)
		{
			delete pSayFont;
			pSayFont = NULL;
		}

		if (pContentFont != NULL)
		{
			delete pContentFont;
			pContentFont = NULL;
		}

		for (std::vector<Image*>::iterator it = vecImageList.begin(); it != vecImageList.end(); ++it)
		{
			if (*it != NULL)
			{
				delete *it;
				*it = NULL;
			}
		}
		vecImageList.clear();

		nIndex = 0;
		nCurPosX = 0;
		bInUse = false;
		nImageIndex = 0;
		pFlyingInfo = NULL;
	}
};

class CFlyingTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CFlyingTextDlg)

public:
	CFlyingTextDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlyingTextDlg();

	void Show(std::wstring strNickName, std::wstring strText, ENM_FlyingText_Type enmType);
	bool IsFinish() { return m_bFinish; }

	void Init();
	//bool IsTrackFull();

// 对话框数据
	enum { IDD = IDD_FLYINGTEXTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	BLENDFUNCTION m_blend;
	void InitRes(flying_text_info * pFlyingInfo);
	void Draw();
	int DrawTitleImage(Graphics & graph, int nIndex, int nPosX, int nPosY);		//画标题图片，返回宽度
	int DrawNickName(Graphics & graph, int nIndex, int nPosX, int nPosY);		//画昵称，返回宽度
	int DrawSay(Graphics & graph, int nIndex, int nPosX, int nPosY);			//画“说：”，返回宽度
	int DrawContent(Graphics & graph, int nIndex, int nPosX, int nPosY);		//画内容，返回宽度
	void FirstDraw();

private:
	TrackInfo m_trackInfo;
	bool m_bFinish;
	HDC	m_hdcMemory1;										//内存DC
	HDC m_hdcMemory2;										//内存DC
	int m_nTotalWidth;										//总长度
	int m_nDrawIndex;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
};
