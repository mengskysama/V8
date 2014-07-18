#pragma  once

#include <afxole.h>

class CMyIEDropTarget :public IDropTarget
{
public:
	CMyIEDropTarget(IDropTarget* pDropTarget):m_p(pDropTarget){}
	~CMyIEDropTarget(){}
public:
	 HRESULT STDMETHODCALLTYPE DragEnter( 
		/* [unique][in] */ IDataObject *pDataObj,
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */ DWORD *pdwEffect) ;

		 HRESULT STDMETHODCALLTYPE DragOver( 
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */ DWORD *pdwEffect) ;

		 HRESULT STDMETHODCALLTYPE DragLeave( void) ;

	 HRESULT STDMETHODCALLTYPE Drop( 
		/* [unique][in] */ IDataObject *pDataObj,
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */ DWORD *pdwEffect) ;


		STDMETHODIMP QueryInterface(REFIID iid, LPVOID* ppvObj)
	{
		if (iid == IID_IUnknown || iid == IID_IDropTarget)
		{
			*ppvObj = this;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return 1;
	}

	STDMETHODIMP_(ULONG) Release()
	{
		return 1;
	}
private:
	IDropTarget* m_p;
};