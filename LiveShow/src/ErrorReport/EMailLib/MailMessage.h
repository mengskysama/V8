// MailMessage.h: interface for the CMailMessage class.
// Copyright (c) 1998, Wes Clyburn
//////////////////////////////////////////////////////////////////////

//*** Every modification marked with <JFO>
//*** have been added by Jean-Francois Ouellet lafaune@total.net (15 dec 98)
//***
//*** These modifications have been done to enhance the real good object
//*** of Wes.Clyburn, with the functionnality of CC and BCC copy and in the
//*** context of keeping the same public interface.
//***
//*** All new parameters are only optionnal.
//***
//*** Still thanks to Wes Clyburn.

#if !defined(AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_)
#define AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

// CMailMessage
// Formats a message compliant with RFC 822.
//
class CMailMessage  
{
public:
	CMailMessage();
	virtual ~CMailMessage();

	enum RECIPIENTS_TYPE { TO, CC, BCC }; // <JFO>

	void FormatMessage();
	int GetNumRecipients(RECIPIENTS_TYPE type = TO /* <JFO> */);
	BOOL GetRecipient( CString& sEmailAddress, CString& sFriendlyName, int nIndex = 0, RECIPIENTS_TYPE type = TO /* <JFO> */ );
	BOOL AddRecipient( LPCTSTR szEmailAddress, LPCTSTR szFriendlyName = _T(""), RECIPIENTS_TYPE type = TO /* <JFO> */ );
	BOOL AddMultipleRecipients( LPCTSTR szRecipients = NULL, RECIPIENTS_TYPE type = TO /* <JFO> */ );
	UINT GetCharsPerLine();
	void SetCharsPerLine( UINT nCharsPerLine );

	CString m_sFrom;
	CString m_sSubject;
	CString m_sEnvelope;
	CString m_sMailerName;
	CString m_sHeader;
	CTime m_tDateTime;	
	CString m_sBody;

private:
	UINT m_nCharsPerLine;
	class CRecipient
	{
		public:
			CString m_sEmailAddress;
			CString m_sFriendlyName;
	};
	CArray <CRecipient, CRecipient&> m_Recipients;
	CArray <CRecipient, CRecipient&> m_CCRecipients;    //*** <JFO>
	CArray <CRecipient, CRecipient&> m_BCCRecipients;   //*** <JFO>

protected:
	// When overriding prepare_header(), call base class 
	// version first, then add specialized 
	// add_header_line calls.
	// This ensures that the base class has a chance to
	// create the header lines it needs.
	virtual void prepare_header();
	virtual void prepare_body();
	virtual void end_header();
	virtual void start_header();

	// This rarely needs overwriting, but is virtual just in case.
	// Do not include the trailing CR/LF in parameter.
	virtual void add_header_line( LPCTSTR szHeaderLine );
};

#endif // !defined(AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_)
