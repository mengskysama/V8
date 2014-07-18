#include "stdafx.h"
#include "ui/FileDialogHelper.h"

namespace common{  namespace ui { namespace filedialoghelper {

	CString BrowseForFolder(CString sTitle/* = ""*/, DWORD dwFlag/* = BIF_NEWDIALOGSTYLE*/, HWND hwndOwner/* = NULL*/)
	{
		CString sRet;

		LPMALLOC pMalloc;
		if (::SHGetMalloc(&pMalloc) == NOERROR)
		{
			LPITEMIDLIST pidl;
			//SHGetSpecialFolderLocation(hwndOwner,CSIDL_DRIVES, &pidl);

			BROWSEINFO bi;
			bi.hwndOwner = hwndOwner;
			bi.pidlRoot = 0/*pidl*/;
			bi.pszDisplayName = 0;
			bi.lpszTitle = sTitle;
			bi.ulFlags = dwFlag;
			bi.lpfn = NULL;
			bi.lParam = 0;
			if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
			{
				if (::SHGetPathFromIDList(pidl, sRet.GetBuffer(MAX_PATH)))
				{ 
					sRet.ReleaseBuffer();
				}

				pMalloc->Free(pidl);
			}
			pMalloc->Release();
		} 

		return sRet;
	}


	INT ShowOpenFileDlg(OUT CString& sFilePath, 
		IN OUT PBOOL pbReadOnly/* = NULL*/, 
		IN LPCTSTR lpszDefExt/* = NULL*/,
		IN LPCTSTR lpszFileName/* = NULL*/,
		IN LPCTSTR lpszFilter /*= NULL*/,	// like "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
		IN DWORD dwFlags/* = OFN_ENABLESIZING*/,
		IN CWnd* pParentWnd/* = NULL*/
		)
	{
		sFilePath.Empty();
		if (pbReadOnly){*pbReadOnly = FALSE;}
		INT nRet = IDCANCEL;

		CFileDialog dlg(TRUE, lpszDefExt, lpszFileName, dwFlags | OFN_ENABLESIZING, lpszFilter, pParentWnd);
		if ((nRet = dlg.DoModal()) == IDOK)
		{
			sFilePath = dlg.GetPathName();
			if (pbReadOnly)
			{
				*pbReadOnly = dlg.GetReadOnlyPref();
			}
		}

		return nRet;
	}

	INT ShowSaveFileDlg(OUT CString& sFilePath, 
		IN LPCTSTR lpszDefExt/* = NULL*/,
		IN LPCTSTR lpszFileName/* = NULL*/,
		IN LPCTSTR lpszFilter/* = NULL*/,	// like "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
		IN DWORD dwFlags/* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/,
		IN CWnd* pParentWnd/* = NULL*/
		)
	{
		sFilePath.Empty();
		INT nRet = IDCANCEL;

		CFileDialog dlg(FALSE, lpszDefExt, lpszFileName, dwFlags | OFN_ENABLESIZING, lpszFilter, pParentWnd);
		if ((nRet = dlg.DoModal()) == IDOK)
		{
			sFilePath = dlg.GetPathName();
		}

		return nRet;
	}

}
}
}