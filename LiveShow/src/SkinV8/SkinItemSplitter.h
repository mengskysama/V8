
#pragma once


// CSkinItemSplitter 命令目标

class CSkinItemSplitter : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemSplitter)

public:
	CSkinItemSplitter();
	virtual ~CSkinItemSplitter();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemSplitter)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemSplitter),
		SKINITEM_DISPIDS(CSkinItemSplitter),
	};
protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	//更新皮肤属性，在皮肤新附着窗口时调用
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
};
