#include "DynamicLibraryHolder.h"

CDynamicLibraryHolder::CDynamicLibraryHolder(void)
:
m_hModule(NULL)
{
}

CDynamicLibraryHolder::~CDynamicLibraryHolder(void)
{
    if (m_hModule != NULL) {
        BOOL bSuc = ::FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
}

bool CDynamicLibraryHolder::Attach(HMODULE hModule)
{
    if (m_hModule != NULL)
        return false;

    if (hModule == NULL)
        return false;

    m_hModule = hModule;
    return true;
}

HMODULE CDynamicLibraryHolder::Detach()
{
    HMODULE hModule = m_hModule;
    m_hModule = NULL;
    return hModule;
}
