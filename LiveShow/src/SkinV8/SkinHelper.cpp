#include "StdAfx.h"
#include "SkinBase.h"
#include "SkinHelper.h"


#ifdef MS_JS_ENGINE_SUPPORT

HRESULT InvokeScriptHelper(CSkinBase* pSkin, CComPtr<IDispatch> pDisp, DISPPARAMS* pParams, VARIANT* pVarResult)
{
	TRY {
		if (pDisp != NULL) {
			return  pDisp->Invoke(0, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
				pParams, pVarResult, NULL, NULL);
		}
	} CATCH(COleException, e) {
		TCHAR error[2048] = {0};
		UINT helpContextID = 0;
		e->GetErrorMessage(error, 2048, &helpContextID);
		CString strSkinParent = (pSkin->GetSkinParent() == NULL) ? _T("NULL") : (pSkin->GetSkinParent()->GetSkinName());
		CString strSkinClass = pSkin->GetClassName();
		CString strSkinName = pSkin->GetSkinName();
		CT2A aError(error);
		CT2A aSkinParent(strSkinParent);
		CT2A aSkinClass(strSkinClass);
		CT2A aSkinName(strSkinName);
		TRACE(("OLE“Ï≥£, errorMessage:%s\n helpContext:%5d\n SkinParent:%s\n SkinClass:%s\n SkinName%s\n"), 
			aError, helpContextID, aSkinParent, aSkinClass, aSkinName);
	}
	END_CATCH

	return S_FALSE;
}

HRESULT FireScriptHelper0(CSkinBase* pSkin, IDispatch* pDisp, VARIANT* pVarResult)
{
	DISPPARAMS dispParams = {NULL, NULL, 0, 0};

	return InvokeScriptHelper(pSkin, pDisp, &dispParams, pVarResult);
};

#endif //MS_JS_ENGINE_SUPPORT


#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;

Handle<Value> V8FireScriptHelper0(CSkinBase* pSkin, Handle<Object> recv, Handle<Function> func)
{
    HandleScope handleScope;

    TryCatch tryCatch;

    Handle<Value> result = func->Call(recv, 0, 0);

    ASSERT(! result.IsEmpty());
    if (result.IsEmpty()) {
        CString strException = ValueToCString(tryCatch.Exception());
        ::MessageBox((pSkin ? pSkin->GetSkinHwnd() : NULL), strException, _T("Ω≈±æ¥ÌŒÛ"), MB_OK | MB_TOPMOST);
        return handleScope.Close(v8::Undefined());
    }

    return handleScope.Close(result);
}

#endif //V8_JS_ENGINE_SUPPORT
