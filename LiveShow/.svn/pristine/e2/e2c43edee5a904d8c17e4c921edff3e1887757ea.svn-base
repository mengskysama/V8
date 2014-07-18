/*****************************************************************************
Module :     URLEncode.H
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: H URL Encoder
*****************************************************************************/
#ifndef __POSTENCODE_H_
#define __POSTENCODE_H_

#include <string>

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

using namespace std;

class UTILITY_API CPostEncode
{
public:
	CPostEncode();
	virtual ~CPostEncode();

	static const string post_encode( const string& scr );
	static const pair<bool,string> post_decode( const string& scr );

private:
	static const string post_encode( const string::const_iterator& begin, 
		const string::const_iterator& end );

	static const pair<bool,string> post_decode( const string::const_iterator& begin, 
		const string::const_iterator& end );
};

#endif //__CURLENCODE_H_