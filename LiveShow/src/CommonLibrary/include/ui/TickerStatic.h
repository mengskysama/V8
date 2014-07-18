#pragma once
#include <vector>
#include "51ktvtype.h"
#include "../Common.h"
using namespace std;

namespace common 
{
	namespace ui
	{
		
		struct tickerInfo
		{
			CString m_Message;
			CString m_Url;
		};

		struct tickerInfoRect
		{
			tickerInfo m_tickerInfo;
			CRect m_Rect;
		};

		//跑马灯
		class COMMON_INTERFACE CTickerStatic : public CStatic
		{
			DECLARE_DYNAMIC(CTickerStatic)

		public:
			CTickerStatic();
			virtual ~CTickerStatic();
			 
		protected:
			DECLARE_MESSAGE_MAP()

		public:
			void RunTicker(vector<tickerInfo> &tickerVec,uint32 intervalTime = 150);  //运行跑马灯
			void SetTickerFont(Gdiplus::Font &fontDefault,Gdiplus::Font &fontSelect);  //设置默认字体和悬停时字体   请在RunTicker之前调用
			void SetTickerColor(COLORREF colorDefault,COLORREF colorSelect);  //设置默认字体颜色和悬停时字体颜色  请在RunTicker之前调用
			void SetPause(bool isPause = true); //设置鼠标悬停时是否暂停滚动   请在RunTicker之前调用
			void SetClickTitle(bool isClick = true); //设置是否有鼠标点击后打开对应url事件  请在RunTicker之前调用
			void OnPostParentPaint(HDC hParentHDC);			

		private:
			virtual void PreSubclassWindow();
			afx_msg void OnPaint();
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam); //鼠标离开
			afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam); //鼠标悬停
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  //鼠标点击
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  //鼠标点击
			afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
			afx_msg void OnSize(UINT nType, int cx, int cy);

			int GetCursorPointIndex();  //获取鼠标当前point所在的下标
			void SetCursourHead(bool head);  //设置鼠标光标类型 true为手型，false为标准箭头
		
		private:
			vector<tickerInfoRect> m_tickerVec;  //跑马灯数据列表
			uint32 m_intervalTime;  //间隔时间
			Gdiplus::Font *m_fontDefault;  //默认字体
			Gdiplus::Font *m_fontSelect;  //悬停时字体			
			Gdiplus::SolidBrush m_colorDefault;  //默认字体颜色
			Gdiplus::SolidBrush m_colorSelect;  //悬停时字体颜色
			Gdiplus::SolidBrush m_colorDown;    //点击时字体颜色
			int m_intervallen; //间隔长度
			int m_nScrollPos;  //文本显示位置
			bool m_bOver;	//鼠标位于控件之上时该值为true，反之为flase
			bool m_bDown;	//鼠标点击时为true，反之为flase
			int m_curIndex; //鼠标当前point所在的下标
			HCURSOR m_cursour; 
			bool m_isPause; //鼠标悬停时是否暂停滚动, true为暂停滚动，false为继续滚动
			bool m_isClick; //鼠标是否可点击标题，以使用浏览器打开对应url

			CDC m_dcBk;
			CBitmap m_bmpBk;
			CBitmap* m_pbmpOldBk;
			bool m_bNeedUpdate;
		};
	}
}