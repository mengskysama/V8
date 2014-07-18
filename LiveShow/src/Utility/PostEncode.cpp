#include "PostEncode.h"

CPostEncode::CPostEncode()
{

}

CPostEncode::~CPostEncode()
{

}


const string CPostEncode::post_encode( const string::const_iterator& begin, const string::const_iterator& end )
{
	string ret;
	ret.reserve( distance(begin,end)*3 );
	for( string::const_iterator p=begin; p!=end; ++p )
	{
		if( *p == '+' )
			ret += "%2B";
		else if( *p == ' ' )
			ret += "%20";
		else if( *p == '%' )
			ret += "%25";
		else if( *p == '=' )
			ret += "%3D";
		else if( *p == '&' )
			ret += "%26";
		else if( *p<33 || *p>127 )
		{
			char tmp = 0;
			ret += '%';
			tmp = ((*p>>4)&0x0F) + '0';  if( tmp > '9' ) tmp += 7u;  ret += tmp;
			tmp = ( *p    &0x0F) + '0';  if( tmp > '9' ) tmp += 7u;  ret += tmp;
		}
		else
			ret += *p;
	}

	return ret;
}


const string CPostEncode::post_encode( const string& scr )
{
	return post_encode( scr.begin(), scr.end() );
}

const pair<bool,string> CPostEncode::post_decode( const string::const_iterator& begin, const string::const_iterator& end )
{
	pair<bool,string> ret;
	ret.first = false;
	string& rs = ret.second;

	int n = distance(begin, end);
	rs.reserve( n );

	for( string::const_iterator p=begin; p!=end; ++p )
	{
		// 为了速度，这里忽略合法性验证
		if( *p == '%' )
		{
			++p;  
			if( p == end ) 
			{
				ret.second += *(--p);
				return ret;  
			}
			char c1 = *p - '0'; 
			if( c1 > 9 ) 
				c1 -= 7;

			++p;  
			if( p == end ) 
			{
				ret.second += *--p;
				return ret;  
			}
			char c2 = *p - '0'; 
			if( c2 > 9 ) 
				c2 -= 7;
			rs += ( (c1<<4) | c2 );
		}
		else
			rs += *p;
	}
	ret.first = true;
	return ret;
}

const pair<bool,string> CPostEncode::post_decode( const string& scr )
{
	return post_decode( scr.begin(), scr.end() );
}
