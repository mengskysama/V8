// blowfish.h     interface file for blowfish.cpp
// _THE BLOWFISH ENCRYPTION ALGORITHM_
// by Bruce Schneier
// Revised code--3/20/94
// Converted to C++ class 5/96, Jim Conger

#define MAXKEYBYTE_BLOWFISHS 	56		// 448 bits max
#define NPASS           16		// SBox passes

#define DWORD_BLOWFISH  		unsigned int
#define WORD_BLOWFISH_BLOWFISH  		unsigned short
#define BYTE_BLOWFISH  		unsigned char

#include "../../Common.h"

namespace common 
{
class COMMON_INTERFACE CBlowFish
{
private:
	DWORD_BLOWFISH 		* PArray ;
	DWORD_BLOWFISH		(* SBoxes)[256];
	void 		Blowfish_encipher (DWORD_BLOWFISH *xl, DWORD_BLOWFISH *xr) ;
	void 		Blowfish_decipher (DWORD_BLOWFISH *xl, DWORD_BLOWFISH *xr) ;

public:
			CBlowFish () ;
			~CBlowFish () ;
	void 		Initialize (BYTE_BLOWFISH key[], int keybytes) ;
	DWORD_BLOWFISH		GetOutputLength (DWORD_BLOWFISH lInputLong) ;
	DWORD_BLOWFISH		Encode (BYTE_BLOWFISH * pInput, BYTE_BLOWFISH * pOutput, DWORD_BLOWFISH lSize) ;
	void		Decode (BYTE_BLOWFISH * pInput, BYTE_BLOWFISH * pOutput, DWORD_BLOWFISH lSize) ;

} ;

// choose a byte order for your hardware
#define ORDER_DCBA	// chosing Intel in this case

#ifdef ORDER_DCBA  	// DCBA - little endian - intel
	union aword {
	  DWORD_BLOWFISH dword;
	  BYTE_BLOWFISH byte [4];
	  struct {
	    unsigned int byte3:8;
	    unsigned int byte2:8;
	    unsigned int byte1:8;
	    unsigned int byte0:8;
	  } w;
	};
#endif

#ifdef ORDER_ABCD  	// ABCD - big endian - motorola
	union aword {
	  DWORD_BLOWFISH dword;
	  BYTE_BLOWFISH byte [4];
	  struct {
	    unsigned int byte0:8;
	    unsigned int byte1:8;
	    unsigned int byte2:8;
	    unsigned int byte3:8;
	  } w;
	};
#endif

#ifdef ORDER_BADC  	// BADC - vax
	union aword {
	  DWORD_BLOWFISH dword;
	  BYTE_BLOWFISH byte [4];
	  struct {
	    unsigned int byte1:8;
	    unsigned int byte0:8;
	    unsigned int byte3:8;
	    unsigned int byte2:8;
	  } w;
};
#endif
}