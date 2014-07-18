#pragma once
#include "GiftListCtrl.h"

class CMyGoodsListCtrl : public CGiftListCtrl
{
public:
	CMyGoodsListCtrl(void);
	~CMyGoodsListCtrl(void);

	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	virtual bool InitGiftListCtrl();
	virtual bool SetListData(std::vector<CMyGoodsItem*> & vecMyGoodsList);

protected:
	virtual void InitListData();

protected:
	DECLARE_MESSAGE_MAP()

private:
	std::vector<CMyGoodsItem*> m_vecMyGoodsList;
};
