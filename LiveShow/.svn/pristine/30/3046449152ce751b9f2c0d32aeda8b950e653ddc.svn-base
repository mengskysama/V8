/**
* packet.h
* 用于网络字节序转换
*
* @version 1.0 (06 ,10, 2008)
* @author reinhardt ken
*
*/
#pragma once
#include <WinSock2.h>
#include "rainbow2.h"
#include <string>
#include <vector>
#include "assert.h"
#include <boost/tuple/tuple.hpp>
#include <utility>

#include "utility/stringhelper.h"

//using namespace common;




namespace Nautilus
{

static uint16 DEFAULT_PACKET_SIZE=1500;

enum
{
	IN_PACKET,
	OUT_PACKET,
	FIXED,
	NO_FIXED,//NOT SUPPORT NOW...
};


struct CommonType
{
	typedef std::size_t size_t;
	typedef std::vector<char> Container;
	typedef Container::iterator Iterator;
	typedef Container::const_iterator ConstIterator;
	typedef std::pair<void *,size_t> PSPair;
	typedef std::pair<void const *,size_t const> CPCSPair;
};


#define  DEFINE_SUBCLASS_POINT(TYPE) TYPE * pThis=static_cast<##TYPE##*>(this);

#define PACKET_1(A,B) A<<B

template<typename Packet>
class InOp :public CommonType
{
public:
	Packet &operator >>(uint8 &b)
	{
		if (EnoughData(sizeof(b)) )
		{
			DEFINE_SUBCLASS_POINT(Packet);
			b = pThis->c[pThis->curIndex];
			pThis->curIndex+= sizeof(b);
		}
		else
		{
			assert(0);
			b = 0;
		}

		return *static_cast<Packet*>(this);;
	}

	Packet &operator >>(uint16 &w)
	{
		if (EnoughData(sizeof(w))) 
		{
			DEFINE_SUBCLASS_POINT(Packet);
			w = ntohs(*(uint16 *)&pThis->c[pThis->curIndex]);
			pThis->curIndex += sizeof(w);
		} 
		else
		{
			assert(0);
			w = 0;
		}

		return *static_cast<Packet*>(this);;
	}

	Packet &operator >>(uint32 &dw)
	{
		if (EnoughData(sizeof(dw))) 
		{
			DEFINE_SUBCLASS_POINT(Packet);
			dw = ntohl(*(uint32 *)&pThis->c[pThis->curIndex]);
			pThis->curIndex += sizeof(dw);
		} 
		else
		{
			assert(0);
			dw = 0;
		}

		return *static_cast<Packet*>(this);;
	}

	Packet &operator >>(uint64 &ddw)
	{
		uint32 hv,lv;
		(*this) >> hv >> lv;
		ddw = hv;
		ddw = ddw << 32;
		ddw += lv;
		return *static_cast<Packet*>(this);;
	}

	Packet &operator >>(char const *&str)
	{
		uint16 len;
		operator >>(len);

		DEFINE_SUBCLASS_POINT(Packet);

		if (EnoughData(len) && !pThis->c[pThis->curIndex+len - 1]) 
		{
			str = (char*)&pThis->c[pThis->curIndex];
			pThis->curIndex += len;
		} 
		else
		{
			assert(0);
			str = "";
		}

		return *static_cast<Packet*>(this);;
	}
	Packet &operator >>(std::string &str)
	{
		char const  *p;
		operator >>(p);
		str = p;
		return *static_cast<Packet*>(this);;
	}
	Packet & operator>>(PSPair & pair/*char  *const buf, uint16 const size*/)
	{
		uint16 size;
		*this>>size;
		pair.second=size;
		if (EnoughData(size))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			memcpy( pair.first,&pThis->c[pThis->curIndex], pair.second);
			pThis->curIndex += pair.second;
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}


	Packet &operator>>(Container & c)
	{
		assert(c.size()==0 );
		uint32 size;
		*this>>size;

		if (EnoughData(size))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			c.resize(size);
			memcpy( &c[0],&pThis->c[pThis->curIndex],size);
			pThis->curIndex += size;
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}

#ifdef SUPPORT_WSTRING
	Packet &operator >>(std::wstring &str)
	{
		std::string tempStr;
		*this>>tempStr;
		str=common::utility::stringhelper::UTF8ToUnicode(tempStr);
		return *static_cast<Packet*>(this);;
	}
#endif





public:
		bool EnoughData(size_t s)
		{
			DEFINE_SUBCLASS_POINT(Packet);
			return (s<=pThis->LeftSpace())?true:false;
		}
};

template<typename Packet>
class OutOp :public CommonType
{
public:

	Packet &operator <<(uint8 b)
	{
		if (EnoughSpace(sizeof(b)))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			pThis->c[pThis->curIndex] = b;
			pThis->curIndex+= sizeof(b);
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);
	}

	Packet &operator <<(uint16 w)
	{
		if (EnoughSpace(sizeof(w)))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			*(uint16 *)&pThis->c[pThis->curIndex] = htons(w);
			pThis->curIndex+= sizeof(w);
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}

	Packet &operator <<(uint32 dw)
	{
		if (EnoughSpace(sizeof(dw)))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			*(uint32 *)&pThis->c[pThis->curIndex] = htonl(dw);
			pThis->curIndex+= sizeof(dw);
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}
	Packet &operator <<(uint64 ddw)
	{
		(*this) << H32(ddw) << L32(ddw);
		return *static_cast<Packet*>(this);;
	}

	Packet &operator <<(char const  *str)
	{
		uint16 len = strlen(str) + 1;
		if (EnoughSpace(len))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			operator <<(len);
			strncpy((char*)&pThis->c[pThis->curIndex], str,len);
			pThis->curIndex += len;
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}

	Packet &operator <<(std::string &str) 
	{
		return operator <<(str.c_str());
	}
	Packet & operator <<(CPCSPair & pair/*char const  *buf, uint16 n*/)
	{
		if (EnoughSpace(pair.second+2))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			*this<<(uint16)pair.second;
			memcpy(&pThis->c[pThis->curIndex], pair.first, pair.second);
			pThis->curIndex += pair.second;
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}


	Packet &operator<<(Container const& c)
	{
		//return *this<<(CPCSPair((void*)&c[0],c.size()));
		if (EnoughSpace(c.size()+2))
		{
			DEFINE_SUBCLASS_POINT(Packet);
			*this<<(uint32)c.size();
			memcpy(&pThis->c[pThis->curIndex], &c[0], c.size());
			pThis->curIndex += c.size();
		}
		else
			assert(0);

		return *static_cast<Packet*>(this);;
	}

#ifdef SUPPORT_WSTRING
	Packet &operator <<(std::wstring const&str)
	{
		std::string tempStr=common::utility::stringhelper::UnicodeToUTF8(str);
		*this<<tempStr;
		return *static_cast<Packet*>(this);;
	}
#endif



	template <typename T1,typename T2>
	Packet &operator<<(boost::tuple<T1 const & ,T2 const &> t)
	{
		return *this<<boost::tuple::get<0>(t)<<boost::tuple::get<1>(t);
	}




private:
	bool EnoughSpace(size_t s)
	{
		DEFINE_SUBCLASS_POINT(Packet);
		bool re=(s<=pThis->LeftSpace())?true:false;

		if(re==false)
		{
			if(pThis->fixed==NO_FIXED)
			{
				pThis->ReSize(pThis->Capacity()+s);	//原有容量+需要容量。	
				return true;
			}
			else
				return false;
		}
		else
			return true;
	}
};


template <typename T1,int T2>
struct Select
{

};

template<typename T1>
struct Select<T1,IN_PACKET>
{
	typedef typename class InOp<T1> BaseClass;
};

template<typename T1>
struct Select<T1,OUT_PACKET>
{
	typedef typename class OutOp<T1> BaseClass;
};



template <int TYPE,int FIXED,typename Container=std::vector<char> > 
class Packet :
	public Select<Packet<TYPE,FIXED>,TYPE>::BaseClass
	,public CommonType
	{
	public:
		
		

		enum COSNTRUCT_TYPE
		{
			CONSTRUCT_COPY,
			CONSTRUCT_SWAP,
		};

	public:
		//construct
		Packet():c(DEFAULT_PACKET_SIZE),curIndex(0),error(0),fixed(FIXED){}
		Packet(size_t size):c(size),curIndex(0),error(0),fixed(FIXED){}
		Packet(Container & c,COSNTRUCT_TYPE constructType=CONSTRUCT_SWAP)
			:curIndex(0),error(0),fixed(FIXED)
		{
			if(constructType==CONSTRUCT_SWAP)
				this->c.swap(c);
			else if(constructType==CONSTRUCT_COPY)
				this->c=c;
			else
				assert(0);
		}

	private:
		Packet(Packet const & );
		Packet & operator =	(Packet const & );

	public:
		//destruct
		virtual ~Packet(){}


		//normal op
		size_t Size(){return curIndex;}
		size_t Capacity(){return c.size();}
		char const *Data(){return & c[0];} 
		bool IsValid(){return error==0;}
		void Error(uint32 error){this->error=error;}
		Container & Get(){return c;}

	

		
	protected:
		Container c;
		size_t  curIndex;
		uint32 error;
		int fixed;

	private:
		void ReSize(size_t s){c.resize(s);}
		size_t LeftSpace(){return Capacity()-Size();}
		friend class Select<Packet<TYPE,FIXED>,TYPE>::BaseClass;

	};


typedef Packet<OUT_PACKET,FIXED> OutPacket;
typedef Packet<IN_PACKET,NO_FIXED> InPacket;

}








// template   <typename   Tuple,int   Index>   struct   print_helper   { 
// 	static   void   print(const   Tuple&   t)   { 
// 		print_helper <Tuple,Index-1> ::print(t); 
// 		std::cout   < <   boost::tuples::get <Index> (t)   < <   '\n '; 
// 	} 
// }; 
// template <typename   Tuple>   struct   print_helper <Tuple,0>   { 
// 	static   void   print(const   Tuple&   t)   { 
// 		std::cout   < <   boost::tuples::get <0> (t)   < <   '\n '; 
// 	} 
// }; 
// template   <typename   Tuple>   void   print_all(const   Tuple&   t)   { 
// 	print_helper <Tuple,boost::tuples::length <Tuple> ::value-1> ::print(t); 
// } 
// int   main()   { 
// 	boost::tuple <int,std::string,double>   tup(42, "A   four   and   a   two ",42.424242); 
// 	print_all(tup); 
// } 