#pragma once

#include <vector>
#include "UIFlashCtrl.h"

class CActiveXCtrl;

//CFlashWnd:自己管理资源，使用者无需来释放

class CFlashWnd : public CWnd, public IUIFlashEventNotify, public IActivXOwner
{
public:
	CFlashWnd();
	~CFlashWnd();

	//是否支持flash播放
	static bool IsSupportFlash();

	//停止所有flash，释放资源(关闭房间时调用)
	static void StopAllFlash();

	//playTime:播放时间(ms) playCircle:每帧间隔(ms)
	bool Play(int width, int height, int playTime, int playCircle, CString strFlashPath, CWnd * pParentWnd = NULL);
	void CallFunction(CString func, std::vector<CString> vecParams);

	bool IsFinish() { return m_bFinish; }
	void SetFinish(bool bFinish) { m_bFinish = bFinish; }
	void Finish();
	long GetVersion();

	//IUIFlashEventNotify

	virtual void OnReadyStateChange(long newState )
	{
	}
	virtual void OnProgress(long percentDone )
	{
	}
	virtual void FSCommand(const CString& command, const CString& args )
	{
	}
	virtual void FlashCall(const CString& request )
	{
	}

	//IActivXOwner

	virtual HWND AxGetHWND()
	{
		return m_hWnd;
	}

	virtual void AxSetFocus(bool bFocus)
	{
	}

	DECLARE_MESSAGE_MAP()

protected:

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

protected:
	int m_count;

	CUITransparentFlashCtrl m_flash;
	bool m_bFinish;
	bool m_bPermanent;
	long m_version;

	static std::vector<CFlashWnd*> ms_vecFlashWnd;
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};