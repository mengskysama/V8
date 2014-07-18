#pragma once
#include "common.h"

namespace common{  namespace ui { namespace filedialoghelper {

	/// 显示浏览文件夹对话框，返回用户选择的文件夹的路径
	COMMON_INTERFACE CString BrowseForFolder(CString sTitle = L"", DWORD dwFlag = BIF_NEWDIALOGSTYLE, HWND hwndOwner = NULL);

	/// 显示打开文件对话框
	COMMON_INTERFACE INT ShowOpenFileDlg(OUT CString& sFilePath, 
		IN OUT PBOOL pbReadOnly = NULL, 
		IN LPCTSTR lpszDefExt = NULL,
		IN LPCTSTR lpszFileName = NULL,
		IN LPCTSTR lpszFilter = NULL,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
		IN DWORD dwFlags = OFN_ENABLESIZING,
		IN CWnd* pParentWnd = NULL
		);

	/// 显示保存文件对话框
	COMMON_INTERFACE INT ShowSaveFileDlg(OUT CString& sFilePath, 
		IN LPCTSTR lpszDefExt = NULL,
		IN LPCTSTR lpszFileName = NULL,
		IN LPCTSTR lpszFilter = NULL,	// eg: "图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
		IN DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		IN CWnd* pParentWnd = NULL
		);


}
}
}