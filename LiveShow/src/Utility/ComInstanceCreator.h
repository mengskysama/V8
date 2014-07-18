#pragma once

HRESULT CoCreateInstanceFromFile(LPCTSTR pszPathName, REFCLSID rclsid, REFIID riid, LPVOID* ppv, HANDLE* pModuleHandle = NULL);
