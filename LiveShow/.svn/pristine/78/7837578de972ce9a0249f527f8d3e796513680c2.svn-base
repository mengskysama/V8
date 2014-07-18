#pragma once


// CSkinItemSlider

class CSkinItemSlider : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemSlider)

public:
	CSkinItemSlider();
	virtual ~CSkinItemSlider();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemSlider)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemSlider),
		SKINITEM_DISPIDS(CSkinItemSlider),

		SKIN_DISPID(CSkinItemSlider, thumbClickImage),
		SKIN_DISPID(CSkinItemSlider, thumbNormalImage),
		SKIN_DISPID(CSkinItemSlider, channelImage),
        SKIN_DISPID(CSkinItemSlider, fullImage),//DragonZ:使两侧颜色不一样，这是填充图
		SKIN_DISPID(CSkinItemSlider, barHeight),//滑动条高度。PS:只是改变所加载进来的图片的高度，控件本身的高度没变
		SKIN_DISPID(CSkinItemSlider, fullBegin),//填充图片开始位置。PS:为支持圆角而做。直角则直接为0开始即可
		SKIN_DISPID(CSkinItemSlider, veritySlider),//是水平还是垂直的滑动条
	};

protected:
	BOOL GetPos(LONG& pos);
	BOOL SetPos(LONG pos);
	BOOL GetMin(LONG& minValue);
	BOOL SetMin(LONG minValue);
	BOOL GetMax(LONG& maxValue);
	BOOL SetMax(LONG maxValue);
	BOOL GetThumbRect(RECT& thumbRect);
	BOOL GetChannelRect(RECT& channelRect);
	BOOL GetFullRect(RECT& fullRect);//DragonZ
	BOOL GetThumbImgRect(RECT& thumbRect);//DragonZ

protected:
	void SetchannelImage(LPCTSTR newVal);
	BSTR GetchannelImage();
	void SetthumbNormalImage(LPCTSTR newVal);
	BSTR GetthumbNormalImage();
	void SetthumbClickImage(LPCTSTR newVal);
	BSTR GetthumbClickImage();
    void SetfullImage(LPCTSTR newVal);//DragonZ
	BSTR GetfullImage();
	void SetbarHeight(LONG newVal);
	LONG GetbarHeight();
	void SetfullBegin(LONG newVal);
	LONG GetfullBegin();
	void SetveritySlider(VARIANT_BOOL newVal);
	VARIANT_BOOL GetveritySlider();

protected:
	virtual void DrawBackground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL, BOOL bChildSkin = FALSE);
	virtual void DrawForeground(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void DrawFrame(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	virtual void Draw(CDC* pDC, LPRECT lpInvalidRect = NULL, CRgn* pInvalidRgn = NULL);
	//更新皮肤属性，在皮肤新附着窗口时调用
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

protected:
	CImageBase* m_pChannelImage;
	CString m_strChannelImage;
	CImageBase* m_pThumbNormalImage;
	CString m_strThumbNormalImage;
	CImageBase* m_pThumbClickImage;
	CString m_strThumbClickImage;
    BOOL m_bInThumbOrDrag;
	CImageBase* m_pFullImage;//DragonZ
	CString m_strFullImage;//DragonZ
	LONG m_lBarHeight;//DragonZ
	LONG m_lFullBegin;//DragonZ
	BOOL m_bVeritySlider;//DragonZ
};
