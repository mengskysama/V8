#pragma once

enum IMAGE_TRANSFORM_TYPE
{
	TRANSFORM_TYPE_NONE,	//ÎÞ±ä»»
	TRANSFORM_TYPE_SMOOTH,	//Æ½»¬
	TRANSFORM_TYPE_COLORIZE,	//Colorize
	TRANSFORM_TYPE_SHIFTRGB,	//ShiftRGB
};

class CImageTransformParam
{
public:
	CImageTransformParam(void);
	~CImageTransformParam(void);

public:
	bool operator == (const CImageTransformParam& rOperand) const;

public:
	IMAGE_TRANSFORM_TYPE enTransformType;
	union {
		struct {
			long width;
			long height;
		};
		struct {
			BYTE hue;
			BYTE sat;
			float blend;
		};
		struct {
			long r;
			long g;
			long b;
		};
	};
	static CImageTransformParam s_globalParam;
};
