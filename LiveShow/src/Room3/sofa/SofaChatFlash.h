#pragma once
#include "..\flash/flashWnd.h"
#include "51ktv.h"

class CSofaChatFlash : public CFlashWnd
{
public:
	CSofaChatFlash(void);
	~CSofaChatFlash(void);

	void OnSofaChat(CString strChatParam, bool bAll = false);
	void OnSofaKick(uint8 unIndex);
	void UpdateSize(int cx);
	void HideChatDlg(uint8 unIndex, bool bAll = false);
};
