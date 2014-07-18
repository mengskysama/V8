
#pragma once


// CSkinProgress 命令目标
class CImageBase;
class CSkinItemDialog;
class CSkinProgress : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinProgress)

protected:
	CSkinProgress();
	virtual ~CSkinProgress();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinProgress)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum{
		SKINBASE_DISPIDS(CSkinProgress),

		SKIN_DISPID(CSkinProgress, range),
		SKIN_DISPID(CSkinProgress, pos),
		SKIN_DISPID(CSkinProgress, backimage),
		SKIN_DISPID(CSkinProgress, posimage),
	};
public:
	//override from CSkinBase
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);

protected:
	//method for dispatch
	BSTR Getbackimage(void);
	void Setbackimage(LPCTSTR newVal);
	BSTR Getposimage(void);
	void Setposimage(LPCTSTR newVal);
	LONG Getrange(void);
	void Setrange(LONG newVal);
	LONG Getpos();
	void Setpos(LONG newVal);

protected:
	CImageBase* m_pBackImage;	//背景图片
    CString m_strBackImage;
	CImageBase* m_pPosImage;	//前景图片，进度由此图片拉伸显示而成
    CString m_strPosImage;
	LONG m_nRange;		//进度的范围
	LONG m_nPos;		//当前进度
};


