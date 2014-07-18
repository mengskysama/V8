#include "StdAfx.h"
#include "VDCSliderCtrl.h"

CVDCSliderCtrl::CVDCSliderCtrl(void)
{
}

CVDCSliderCtrl::~CVDCSliderCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CVDCSliderCtrl, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CVDCSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int imin;
	int imax;
	GetRange(imin,imax);
	CRect rect;
	this->GetClientRect(&rect);
	int pos  = 0;
	if (point.x > rect.Width()/2)
	{
		pos= (int)((((float)(point.x + 6)*(float)(imax + 1 - imin)/(float)(rect.Width() - 8))*2 + 1)/2);
	}
	else
	{
		pos= (int)((((float)(point.x - 6)*(float)(imax + 1 -imin)/(float)(rect.Width()))*2 + 1)/2);
	}

	SetPos(pos);
	Invalidate(FALSE);
	CSliderCtrl::OnLButtonDown(nFlags, point);
}
