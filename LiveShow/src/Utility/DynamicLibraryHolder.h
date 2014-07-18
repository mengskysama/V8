#pragma once
#include "windows.h"

class CDynamicLibraryHolder
{
public:
    CDynamicLibraryHolder(void);
    ~CDynamicLibraryHolder(void);

public:
    bool Attach(HMODULE hModule);
    HMODULE Detach();
    operator HMODULE() {return m_hModule;};

protected:
    HMODULE m_hModule;
};
