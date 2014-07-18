#pragma once

#ifdef V8_JS_ENGINE_SUPPORT

#include <map>

class CV8ContextFunction : public std::pair<v8::Persistent<v8::Context>, v8::Persistent<v8::Function>>
{
public:
    CV8ContextFunction();
    CV8ContextFunction(const CV8ContextFunction& contextFunc);
    CV8ContextFunction(v8::Handle<v8::Context> context, v8::Handle<v8::Function> func);
    ~CV8ContextFunction();
    CV8ContextFunction& operator = (const CV8ContextFunction& contextFunc);
};

#endif //V8_JS_ENGINE_SUPPORT
