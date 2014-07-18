#pragma once

#ifdef USE_CXIMAGE

class CxIStreamFile :
    public CxFile
{
public:
    CxIStreamFile(IStream* pIStream)
    {
        m_pIStream = pIStream;
    }

    virtual ~CxIStreamFile(void)
    {
    }

public:
    virtual bool Close()
    {
        m_pIStream = NULL;

        return true;
    }

    virtual size_t Read(void *buffer, size_t size, size_t count)
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        ULONG readSize = -1;
        HRESULT hr = m_pIStream->Read(buffer, (ULONG)(size * count), &readSize);
        if (FAILED(hr))
            return 0;

        ASSERT(readSize > 0);
        ASSERT(readSize < UINT_MAX);

        return (size_t)readSize;
    }

    virtual size_t Write(const void *buffer, size_t size, size_t count)
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        ULONG writtenSize = -1;
        HRESULT hr = m_pIStream->Write(buffer, (ULONG)(size * count), &writtenSize);
        if (FAILED(hr))
            return 0;

        ASSERT(writtenSize > 0);
        ASSERT(writtenSize < UINT_MAX);

        return (size_t)writtenSize;
    }

    virtual bool Seek(long offset, int origin)
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return false;

        LARGE_INTEGER liOffset; 
        liOffset.QuadPart = offset;
        DWORD dwOrigin = origin;
        ULARGE_INTEGER newPosition; 
        newPosition.QuadPart = -1;
        HRESULT hr = m_pIStream->Seek(liOffset, dwOrigin, &newPosition);
        if (FAILED(hr))
            return false;

        return true;
    }

    virtual long Tell()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        return 0;
    }

    virtual long Size()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        STATSTG stg = {0};
        HRESULT hr = m_pIStream->Stat(&stg, 0);
        if (FAILED(hr))
            return 0;

        ASSERT(stg.cbSize.QuadPart > INT_MAX);
        return (long)(stg.cbSize.QuadPart);
    }

    virtual bool Flush()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return false;

        HRESULT hr = m_pIStream->Commit(STGC_DEFAULT);
        if (FAILED(hr))
            return false;

        return true;
    }

    virtual bool Eof()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return true;

        return false;
    }

    virtual long Error()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        return 0;
    }

    virtual bool PutC(unsigned char c)
    {
        // Default implementation
        size_t nWrote = Write(&c, 1, 1);
        return (bool)(nWrote == 1);
    }

    virtual long GetC()
    {
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        char c;
        size_t nWrote = Read(&c, 1, 1);
        ASSERT(nWrote == 1);

        return c;
    }

    virtual char* GetS(char *string, int n)
    {
        ASSERT(0);
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return NULL;

        return NULL;
    }

    virtual long Scanf(const char *format, void* output)
    {
        ASSERT(0);
        ASSERT(m_pIStream != NULL);
        if (m_pIStream == NULL)
            return 0;

        return 0;
    }

protected:
    CComPtr<IStream> m_pIStream;
};

#endif //USE_CXIMAGE
