#include "stdafx.h"
#include"CMyIEDropTarget.h"



HRESULT CMyIEDropTarget::Drop(
		IDataObject * pDataObject,//Pointer to the interface for the source data
        DWORD grfKeyState, //Current state of keyboard modifier keys
        POINTL pt,         //Current cursor coordinates
        DWORD * pdwEffect  //Pointer to the effect of the drag-and-drop // operation
		)
	{
		//pDataObject->
		return m_p->Drop(
			pDataObject,//Pointer to the interface for the source data
			 grfKeyState, //Current state of keyboard modifier keys
			 pt,         //Current cursor coordinates
			 pdwEffect  //Pointer to the effect of the drag-and-drop // operation
			);
		//return S_OK;
	}


	HRESULT CMyIEDropTarget::DragEnter( 
		/* [unique][in] */ IDataObject *pDataObj,
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */ DWORD *pdwEffect) 
	{
		*pdwEffect = DROPEFFECT_COPY;
		return S_OK;
		return m_p->DragEnter(pDataObj,
			 grfKeyState,
			 pt,
			pdwEffect) ;
		//return S_OK;
	}

	HRESULT CMyIEDropTarget::DragOver( 
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */ DWORD *pdwEffect)
	{
		return m_p->DragOver( 
			grfKeyState,
			pt,
			pdwEffect);
		//return S_OK;
	}

	HRESULT CMyIEDropTarget::DragLeave( void)
	{
		return m_p->DragLeave( );
		//return S_OK;
	}


