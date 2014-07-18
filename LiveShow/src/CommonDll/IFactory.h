#pragma once
#include "ICodec.h"

class IFactory
{
public:
	virtual ICodec* RegisterCodec() = 0;
};