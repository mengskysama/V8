#pragma once
#include "afxwin.h"
#include "51ktvtype.h"
#include "../Common.h"
#include "ImageEx.h"

namespace common 
{
	namespace ui
	{
        #define TIMER_DRAW_FLASH_FRAME      1

		//using namespace Gdiplus;
		class COMMON_INTERFACE FlashFrameWnd :
			public CWnd
		{
		public:
			FlashFrameWnd(void);
			~FlashFrameWnd(void);
			DECLARE_MESSAGE_MAP()
		protected:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			afx_msg void OnClose();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

			void DrawFlashFrame();
			
		public:
			BOOL CreateEx(CWnd* pBottomWnd, CRect& rcWnd);
			void ShowFlashFrame(CRect& rc);
			void SetFlashFrameImg(CString & strFrameImg);
			void SetFrameBorderWidth(int nBorderWidth);
			void FlashTheFrame();
		private:
			CWnd* m_pParentWnd;
			CRect m_rcWnd;
			common::ui::ImageEx* m_imgFrame;
			int m_frameBorderWidth;
			int m_flashTimes;
			bool m_bFlashing;
			//关于窗口透明
			BLENDFUNCTION m_blend ;			
		};
       
	}
}