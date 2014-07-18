#include "StdAfx.h"

#include "ImageTransformParam.h"

CImageTransformParam CImageTransformParam::s_globalParam;

CImageTransformParam::CImageTransformParam(void)
:
enTransformType(TRANSFORM_TYPE_NONE)
{
}

CImageTransformParam::~CImageTransformParam(void)
{
}

bool CImageTransformParam::operator == (const CImageTransformParam& rOperand) const
{
	if (enTransformType != rOperand.enTransformType)
		return false;
	switch (enTransformType)
	{
	case TRANSFORM_TYPE_NONE:
		return true;
	case TRANSFORM_TYPE_SMOOTH:
		return ((width == rOperand.width) && (height == rOperand.height));
	case TRANSFORM_TYPE_COLORIZE:
		return ((hue == rOperand.hue) && (sat == rOperand.sat) && (blend == rOperand.blend));
	case TRANSFORM_TYPE_SHIFTRGB:
		return ((r == rOperand.r) && (g == rOperand.g) && (b == rOperand.b));
	}
	return false;
}
