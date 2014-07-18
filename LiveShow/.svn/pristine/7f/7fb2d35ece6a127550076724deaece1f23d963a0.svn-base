#pragma once

#include "ImageBaseImplCDib.h"
#include "ImageBaseImplBitmap.h"
#include "ImageBaseImplCxImage.h"

#ifdef USE_IMAGE_CXIMAGE
typedef CImageBaseImplCxImage CImageType;
#else
#ifdef USE_IMAGE_GDIPLUS_BITMAP
typedef CImageBaseImplBitmap CImageType;
#else 
#ifdef USE_IMAGE_CDIB
typedef CImageBaseImplCDib CImageType;
#endif //USE_IMAGE_CDIB
#endif //USE_IMAGE_GDIPLUS_BITMAP
#endif //USE_IMAGE_CXIMAGE
