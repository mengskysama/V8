#pragma once
#include "afxwin.h"
#include "51ktvtype.h"
#include "../Common.h"
#include "ImageEx.h"

namespace common 
{
	namespace ui
	{
		#define  WM_ONCLICK_GIFBUTTON         WM_USER + 660

		class COMMON_INTERFACE GifButton :
			public CWnd
		{
		public:
			GifButton(void);
			~GifButton(void);
			DECLARE_MESSAGE_MAP()
		protected:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam); 
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

			void DrawGif();
		public:
			BOOL CreateEx(CWnd* pwndParent, CString& strIconPath);
			void SetGifBtnGifIcon(CString& strPath);
			void SetBtnHandCursor(BOOL bHand);
			void SetDrawTime(LONG drawTime);
			void ShowGifBtn(CRect& rc);
			void SetGifBtnTipText(CString& strTip);
		private:
			CString m_strGifIcon;
			BOOL m_bHandCursor;
			common::ui::ImageEx * m_pGifIcon;
			LONG m_drawTime;
			CToolTipCtrl m_OwnToolTipCtrl;
			CString m_strTipText;
			CWnd* m_pParentWnd;
			CRect m_rcWnd;//使用GetWindowRect()有风险。在屏保或休眠或待机等时候
			//关于窗口透明
			BLENDFUNCTION m_blend ;			
		};
        

	}
}