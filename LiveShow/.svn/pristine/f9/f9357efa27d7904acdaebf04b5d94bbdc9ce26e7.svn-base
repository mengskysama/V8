#include "stdafx.h"
#include "FactoryImpl.h"
#include "FloatAACPlusEncodeCodec.h"

class FactoryProduct:public IFactory
{
public:
	ICodec* RegisterCodec();
};

FactoryProduct g_factoryProduct;

ICodec* FactoryProduct::RegisterCodec()
{
	return new FloatAACPlusEncodeCodec();
}

COMMON_OUT_CLASS IFactory* GetFloatEncodeFactory()
{
	return &g_factoryProduct;
}

