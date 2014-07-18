#include "stdafx.h"
#include "FactoryImpl.h"
#include "FloatAACPlusDecodeCodec.h"

class FactoryProduct:public IFactory
{
public:
	ICodec* RegisterCodec();
};

FactoryProduct g_factoryProduct;

ICodec* FactoryProduct::RegisterCodec()
{
	return new FloatAACPlusDecodeCodec();
}

IFactory* GetFloatDecodeFactory()
{
	return &g_factoryProduct;
}

