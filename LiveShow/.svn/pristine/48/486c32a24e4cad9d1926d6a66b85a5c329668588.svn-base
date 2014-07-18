#pragma once
#include "GiftListCtrl.h"

class CPropsListCtrl :	public CGiftListCtrl
{
public:
	CPropsListCtrl(std::wstring strTabName);
	virtual ~CPropsListCtrl(void);

	virtual bool InitGiftListCtrl();
	//virtual bool SetListData(VecGiftItem & vecGiftList);

protected:
	void AddStampItem();
	void AddSpeakerItem();
	void AddWishSong();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};
