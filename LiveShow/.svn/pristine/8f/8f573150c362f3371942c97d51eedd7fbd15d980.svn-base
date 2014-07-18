#pragma once

template<typename CCmdTargetT>
class CCmdTargetPtr
{
	typedef CCmdTargetT T;
public:
	CCmdTargetPtr() throw()
		:m_pT(NULL)
	{
	}

	CCmdTargetPtr(int nNull) throw()
	{
		ATLASSERT(nNull == 0);
		m_pT = NULL;
	}

	CCmdTargetPtr(const CCmdTargetPtr& other) throw()
	{
		m_pT = other.m_pT;
		if (m_pT != NULL) {
			m_pT->ExternalAddRef();
		}
	}

	CCmdTargetPtr(T* pT) throw()
	{
		m_pT = pT;
		if (m_pT != NULL) {
			m_pT->ExternalAddRef();
		}
	}

	~CCmdTargetPtr(void) throw()
	{
		if (m_pT != NULL) {
			m_pT->ExternalRelease();
			m_pT = NULL;
		}
	}

	operator T* () const throw()
	{
		return m_pT;
	}

	T& operator * () const throw()
	{
		ASSERT(m_pT != NULL);
		return *m_pT;
	}

	T** operator & () throw()
	{
		ASSERT(m_pT == NULL);
		return &m_pT;
	}

	T* operator -> () const throw()
	{
		return m_pT;
	}

	bool operator ! () const throw()
	{
		return (m_pT == NULL);
	}

	bool operator < (T* pT) const throw()
	{
		return (m_pT < pT);
	}

	bool operator == (T* pT) const throw()
	{
		return (m_pT == pT);
	}

	bool operator <= (T* pT) const throw()
	{
		return (operator<(pT) || operator==(pT));
	}

	bool operator > (T* pT) const throw()
	{
		return !operator<=(pT);
	}

	bool operator >= (T* pT) const throw()
	{
		return !operator<(pT);
	}

	bool operator != (T* pT) const throw()
	{
		return !operator==(pT);
	}

	void Release() throw()
	{
		T* pTemp = m_pT;
		if (pTemp != NULL) {
			m_pT = NULL;
			pTemp->ExternalRelease();
		}
	}

	T* operator = (const CCmdTargetPtr& other) throw()
	{
		if (*this != other) {
			if (m_pT != NULL) {
				m_pT->ExternalRelease();
				m_pT = NULL;
			}
			m_pT = other.m_pT;
			if (m_pT != NULL) {
				m_pT->ExternalAddRef();
			}
		}
		return *this;
	}

	T* operator = (T* pT) throw()
	{
		if (*this != pT) {
			if (m_pT != NULL) {
				m_pT->ExternalRelease();
				m_pT = NULL;
			}
			m_pT = pT;
			if (m_pT != NULL) {
				m_pT->ExternalAddRef();
			}
		}
		return *this;
	}

private:
	T* m_pT;
};
