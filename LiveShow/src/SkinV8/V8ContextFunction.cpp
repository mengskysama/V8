#include "StdAfx.h"

#ifdef V8_JS_ENGINE_SUPPORT

#include "V8ContextFunction.h"

using namespace std;

using namespace v8;

CV8ContextFunction::CV8ContextFunction() 
{
    HandleScope handleScope;

    int adjusted = V8::AdjustAmountOfExternalAllocatedMemory(1024 * 1024);

    first.Dispose();
    first.Clear();
    second.Dispose();
    second.Clear();
};

CV8ContextFunction::CV8ContextFunction(const CV8ContextFunction& contextFunc) 
{
    HandleScope handleScope;

    int adjusted = V8::AdjustAmountOfExternalAllocatedMemory(1024 * 1024);

    first.Dispose();
    first.Clear();
    second.Dispose();
    second.Clear();

    first = Persistent<Context>::New(contextFunc.first);
    second = Persistent<Function>::New(contextFunc.second);
};

CV8ContextFunction::CV8ContextFunction(Handle<Context> context, Handle<Function> func) 
{
    HandleScope handleScope;

    int adjusted = V8::AdjustAmountOfExternalAllocatedMemory(1024 * 1024);

    first.Dispose();
    first.Clear();
    second.Dispose();
    second.Clear();

    first = Persistent<Context>::New(context);
    second = Persistent<Function>::New(func);
};

CV8ContextFunction::~CV8ContextFunction()
{
    HandleScope handleScope;

    first.Dispose();
    first.Clear();
    second.Dispose();
    second.Clear();
};

CV8ContextFunction& CV8ContextFunction::operator = (const CV8ContextFunction& contextFunc) 
{
    HandleScope handleScope;

    if (&contextFunc == this)
        return *this;

    first.Dispose();
    first.Clear();
    second.Dispose();
    second.Clear();

    first = Persistent<Context>::New(contextFunc.first);
    second = Persistent<Function>::New(contextFunc.second);

    return *this;
};
#endif //V8_JS_ENGINE_SUPPORT
