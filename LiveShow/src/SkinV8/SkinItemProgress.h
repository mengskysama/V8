
#pragma once


// CSkinItemProgress 命令目标
#define SKINITEM_PROGRESS_IMAGE_LIST(theClass, V) \
    V(theClass, backLeftImage)  \
    V(theClass, backMidImage)  \
    V(theClass, backRightImage)  \
    V(theClass, progressImage)  \


class CSkinItemProgress : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemProgress)

public:
	CSkinItemProgress();
	virtual ~CSkinItemProgress();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemProgress)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemProgress),
		SKINITEM_DISPIDS(CSkinItemProgress),
        SKINITEM_PROGRESS_IMAGE_LIST(CSkinItemProgress, SKIN_DISPID_WITH_COMMA)
	};

protected:
    SKINITEM_PROGRESS_IMAGE_LIST(CSkinItemProgress, DECLARE_IMAGE_PROPERTY);

protected:
	BOOL GetPos(LONG& pos);
	BOOL SetPos(LONG pos);
	BOOL GetRange(LONG& minValue, LONG& maxValue);
	BOOL SetRange(LONG minValue, LONG maxValue);

protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	//更新皮肤属性，在皮肤新附着窗口时调用
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
    SKINITEM_PROGRESS_IMAGE_LIST(CSkinItem, DECLARE_IMAGE_PROPERTY_VAR);
};


