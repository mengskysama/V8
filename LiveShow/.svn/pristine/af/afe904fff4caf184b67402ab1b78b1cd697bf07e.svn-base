#include "StdAfx.h"

#ifdef CDIB_SUPPORT

#include "ImageBaseImplCDib.h"

CImageBaseImplCDib::CImageBaseImplCDib(void)
:m_pDib(NULL)
{

}

CImageBaseImplCDib::CImageBaseImplCDib(const CImageBaseImplCDib& r)
{

}

CImageBaseImplCDib::~CImageBaseImplCDib(void)
{
	if (m_pDib != NULL){
		delete m_pDib;
		m_pDib = NULL;
	}
}

CImageBaseImplCDib& CImageBaseImplCDib::operator = (const CImageBaseImplCDib& r)
{
	if (this != &r){
		//do nothing
	}
	return *this;
}

BOOL CImageBaseImplCDib::LoadFromFile(LPCTSTR filePathName)
{
	if (m_pDib != NULL){
		delete m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new CDib;
	if (m_pDib == NULL || !m_pDib->ReadInFile(filePathName)){
		delete m_pDib;
		m_pDib = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL CImageBaseImplCDib::LoadFromData(const char* pzData, long size)
{
	return FALSE;
}

BOOL CImageBaseImplCDib::Transform(const CImageTransformParam* pParam)
{
	return FALSE;
}

BOOL CImageBaseImplCDib::TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor)
{
	return FALSE;
}

BOOL CImageBaseImplCDib::Draw(HDC hDC, LPCRECT lpDstRect, LPRECT lpClipRect, const CImageTransformParam* pParam, 
                              LONG clrTransparent, BOOL bAlpha, 
                              BOOL bSideStretch, LONG sideWidth, LONG sideHeigh)
{
	ASSERT(m_pDib != NULL && lpDstRect != NULL);
	if (m_pDib == NULL || lpDstRect == NULL)
		return FALSE;

	CRect drawRect(lpDstRect);
	return m_pDib->StretchShow(hDC, drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height());
}

LONG CImageBaseImplCDib::Width()
{
	ASSERT(m_pDib != NULL);
	if (m_pDib == NULL)
		return -1;

	return m_pDib->Width();
}

LONG CImageBaseImplCDib::Height()
{
	ASSERT(m_pDib != NULL);
	if (m_pDib == NULL)
		return -1;

	return m_pDib->Height();
}

#endif //CDIB_SUPPORT
