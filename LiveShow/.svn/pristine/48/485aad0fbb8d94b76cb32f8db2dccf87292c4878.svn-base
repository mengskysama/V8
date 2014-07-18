/******************************************************************************
 * Utils.cpp - Common function implemention
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     N/A
 * modification history
 * --------------------
 * 01a, 19nov2008, Davidfan written
 * --------------------
 ******************************************************************************/

 
#include <stdarg.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <time.h>
#include <io.h>
//#include <Winbase.h>
#include <winsock2.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#include "..\include\Utils.h"

/******************* utils routine **********************************/
/*******************************************************************
Encode the given string to base64 format and place it into store. 
store will be 0-terminated, and must point to a writable buffer 
of at least 1+BASE64_LENGTH(length) bytes. 
Note: Routine stolen from wget (grendel).
******************************************************************/

void base64_encode(const char *s, char *store, int length)
{

  /* Conversion table. */
	char tbl[64] = {
	      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	      'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	      'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	      'w', 'x', 'y', 'z', '0', '1', '2', '3',
	      '4', '5', '6', '7', '8', '9', '+', '/'
	};

	int i;
	unsigned char *p = (unsigned char *) store;
    /* Transform the 3x8 bits to 4x6 bits, as required by base64. */
	for (i = 0; i < length; i += 3)
  	{
		*p++ = tbl[s[0] >> 2];
		*p++ = tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
	    *p++ = tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
		*p++ = tbl[s[2] & 0x3f];
		s += 3;
	}

	/* Pad the result if necessary... */
	if (i == length + 1)
		*(p - 1) = '=';
	else if (i == length + 2)
		*(p - 1) = *(p - 2) = '=';
	
	/* ...and zero-terminate it. */
	*p = '\0';

}

#ifdef ENABLE_DEBUG_MODE
void ShowMsg(const char * args, ...)
{
	char p[MAX_MSG_SIZE];
	va_list vp;
	va_start(vp, args);
	vsprintf(p, args, vp);
	va_end(vp);
	fprintf(stdout, "%s\n", p);
}
#else
void ShowMsg(const char * args, ...) { }
#endif


int Panic(const char *args, ...)
{
	char p[MAX_MSG_SIZE];
    va_list vp;
    va_start(vp, args);
    vsprintf(p, args, vp);
    va_end(vp);
	ShowMsg(p);
	exit(1);
}

#ifndef WIN32
int safe_open(const char *pathname,  int flags)
{
 	int ret;
 	
    ret = open(pathname,  flags, 0777);
    if (ret == -1) 
    {
		Panic("%s", pathname);
    }
    
    return ret;
}

ssize_t safe_write(int fd,  const void *buf,  size_t count)
{
	ssize_t n;

	do 
	{
		n = write(fd, buf, count);
	} while (n < 0 && errno == SOCKET_EINTR);
	
	return n;
}

ssize_t safe_read(int fd,  void *buf,  size_t count)
{
	ssize_t n;

	do 
	{
		n = read(fd, buf, count);
	} while (n < 0 && errno == SOCKET_EINTR);

	return n;
}

ssize_t full_write(int fd, const void *buf, size_t len)
{
        ssize_t cc;
        ssize_t total;

        total = 0;

        while (len > 0) 
		{
                cc = safe_write(fd, buf, len);

                if (cc < 0)
                        return cc;              /* write() returns -1 on failure. */

                total += cc;
                buf = ((const char *)buf) + cc;
                len -= cc;
        }
        return total;
}

int  CreateOutFile(const char * PathName )
{
	int oflags = O_WRONLY | O_CREAT | O_TRUNC; 
	int iflags = O_RDONLY;
	size_t out_full = 0;
   	size_t out_part = 0;
   	size_t in_full = 0;
   	size_t in_part = 0;
   	size_t count = 500000;
   	size_t bs = 8192;
   	ssize_t n;
   	int sync_flag = false;
   	//int trunc_flag = true;
   	int ifd;    
   	int ofd;        
   	char *buf;      
	
	if((ifd = safe_open("/dev/zero",  iflags)) < 0)
		return -1;
		
	if((ofd = safe_open(PathName, oflags)) < 0 )
		return -1;
	
	buf = (char *)malloc(bs);
		
	while (in_full + in_part != count) 
	{
 		memset(buf, '\0', bs);
       	n = safe_read(ifd, buf, bs);
       	if (n < 0)  return -1;
        if (n == 0) break;  
        
       	if ((size_t)n == bs) 
        	in_full++;
        else 
        	in_part++;
             
 	    if (sync_flag) 
		{
        	memset(buf + n, '\0', bs - n);
        	n = bs; 
        }       
        
        n = full_write(ofd, buf, n);
        if (n < 0) return -1;
       
	    if ((size_t)n == bs) 
       		out_full++;
        else 
        	out_part++;
	}

	if (close (ifd) < 0) 
       	return -1;
    if (close (ofd) < 0) 
 		return -1;
 		
    fprintf(stderr, "%ld+%ld records in\n%ld+%ld records out\n",
        						    (long)in_full, (long)in_part,
           							(long)out_full, (long)out_part);

	return 0;	
}

bool IsTargetFileExist(const char * FileName, ULONG64& filesize)
{
	struct stat stbuf;
	if(stat(FileName, &stbuf) == -1)
		return false;
	else
	{
		filesize = stbuf.st_size;
		return true;
	}
}
#else
bool IsTargetFileExist(const char * FileName, ULONG64& filesize)
{
	struct _stat stbuf;
	if(_stat(FileName, &stbuf) == -1)
		return false;
	else
	{
		filesize = stbuf.st_size;
		return true;
	}
}
#endif

char * StrDup(const char *str)
{
	char *ptr;
	int i;

	if(str == NULL) return NULL;

	for(i = 0; str[i] != '\0'; i ++) ;
	ptr = new char[i + 1];
	for(i = 0; str[i] != '\0'; i ++){
		ptr[i] = str[i];
	}
	ptr[i] = '\0';

	return ptr;
};

/* checks and sees wether the specifed string is a number */
/* TODO move to misc.c and add to misc.h */

int is_number(char *str)
{
    int i = 0;

    while (str[i])
    {
	if (isdigit(str[i]) == 0)
	{
	    return 0;
	}
	i++;
    }
    return 1;
}


/* How many digits in a (long) integer? */
int numdigit(long a)
{
    int res;

    for (res = 1; (a /= 10) != 0; res++);
    return res;
}




/* Subtract the `struct timeval' values X and Y,
*  storing the result in RESULT.
*  Return 1 if the difference is negative, otherwise
*  0. 
*/

int DiffTime(struct timeval *result, struct timeval *x, struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating Y. */
    if (x->tv_usec < y->tv_usec)
    {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
    }
	
    if (x->tv_usec - y->tv_usec > 1000000)
    {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
    }
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    return x->tv_sec < y->tv_sec;
}

#ifndef WIN32
void delay_ms(int ms)
{
    struct timeval tv_delay;

    memset(&tv_delay, 0, sizeof(tv_delay));

    tv_delay.tv_sec = ms / 1000;
    tv_delay.tv_usec = (ms * 1000) % 1000000;

    if (select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv_delay) < 0)
		printf("Warning Unable to delay\n");
}
#else
void delay_ms(int ms)
{
	Sleep(ms);
}
#endif

void SizeToStr(char *sizeStr, off_t size)
{
	double dsize = size;

	if(dsize < 0)
	{
		sprintf(sizeStr, "%3dB", 0);
		return;
	}

	if(dsize < 1000)
	{
		sprintf(sizeStr, "%3ldB", (long)dsize);
		return;
	}
	
	dsize /= 1024;
	if(dsize < 1000)
	{
		if(dsize <= 9.9)
		{
			sprintf(sizeStr, "%.1fK", dsize);
		}
		else
		{
			sprintf(sizeStr, "%3ldK", (long)dsize);
		}
		return;
	}
	
	dsize /= 1024;
	if(dsize < 1000)
	{
		if(dsize <= 9.9)
		{
			sprintf(sizeStr, "%.1fM", dsize);
		}
		else
		{
			sprintf(sizeStr, "%3ldM", (long)dsize);
		}
		return;
	}
	
	dsize /= 1024;
	if(dsize < 1000)
	{
		if(dsize <= 9.9)
		{
			sprintf(sizeStr, "%.1fG", dsize);
		}
		else
		{
			sprintf(sizeStr, "%3ldG", (long)dsize);
		}
		return;
	}
};

void TimeToStr(char *timeStr, double time)
{
	long sec, min, hour, day;
	
	min = (long)time / 60; // min
	sec = (long)time % 60; // sec

	if(min < 60)
	{
		sprintf(timeStr, "%02ld:%02ld", min, sec);
		return;
	}

	hour = min / 60;
	min %= 60;

	if(hour < 24)
	{
		sprintf(timeStr, "%2ldh%2ld", hour, min);
		return;
	}

	day = hour / 24;
	hour %= 24;

	if(day < 100)
	{
		sprintf(timeStr, "%2ldd%2ld", day, hour);
		return;
	}
	
	sprintf(timeStr, "--:--");
};


#ifdef WIN32
void thread_lock_init(CRITICAL_SECTION* m_thread_mutex)
{
	InitializeCriticalSection(m_thread_mutex);
}

void thread_lock_uninit(CRITICAL_SECTION* m_thread_mutex)
{
	DeleteCriticalSection(m_thread_mutex);
}


int thread_lock(CRITICAL_SECTION* m_thread_mutex)
{
	//printf("[thread_lock]Try get lock\n");
	EnterCriticalSection(m_thread_mutex);
	//printf("[thread_lock]Got lock\n");
	return 0;
}

int thread_unlock(CRITICAL_SECTION* m_thread_mutex)
{
	//printf("[thread_lock]Try release lock\n");
	LeaveCriticalSection(m_thread_mutex);
	//printf("[thread_lock]Released lock\n");
	return 0;
}

#else
void thread_lock_init(pthread_mutex_t* m_thread_mutex)
{
	pthread_mutex_init(m_thread_mutex, NULL);
}

void thread_lock_uninit(pthread_mutex_t* m_thread_mutex)
{
	pthread_mutex_destroy(m_thread_mutex);
}


int thread_lock(pthread_mutex_t* m_thread_mutex)
{
	int rc = pthread_mutex_lock(m_thread_mutex);
	if (0 != rc) {
		return -1;
	}
	
	return 0;
}

int thread_unlock(pthread_mutex_t* m_thread_mutex)
{
	int rc = pthread_mutex_unlock(m_thread_mutex);
	if (0 != rc) {
		return -1;
	}
	
	return 0;
}
#endif

int Base64Encode(const unsigned char *pbSrcData, int nSrcLen, char* szDest, int *pnDestLen, unsigned int dwFlags)
{
	static const char s_chBase64EncodingTable[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
		'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

		int nWritten = 0;
		int nLen1 = (nSrcLen/3)*4 ;
		int nLen2 = nLen1/76;
		int nLen3 = 19;
		int i = 0;
		int j = 0;
		int n = 0;
		int k = 0;

		if (!pbSrcData || !szDest || !pnDestLen)
		{
			return 0;
		}

		for (i=0; i<=nLen2; i++)
		{
			if (i==nLen2)
				nLen3 = (nLen1%76)/4;

			for (j=0; j<nLen3; j++)
			{
				unsigned int dwCurr = 0;
				for (n=0; n<3; n++)
				{
					dwCurr |= *pbSrcData++;
					dwCurr <<= 8;
				}
				for (k=0; k<4; k++)
				{
					unsigned char b = (unsigned char)(dwCurr>>26);
					*szDest++ = s_chBase64EncodingTable[b];
					dwCurr <<= 6;
				}
			}
			nWritten+= nLen3*4;

			if ((dwFlags & BASE64_FLAG_NOCRLF)==0)
			{
				*szDest++ = '\r';
				*szDest++ = '\n';
				nWritten+= 2;
			}
		}

		if (nWritten && (dwFlags & BASE64_FLAG_NOCRLF)==0)
		{
			szDest-= 2;
			nWritten -= 2;
		}

		nLen2 = nSrcLen%3 ? nSrcLen%3 + 1 : 0;
		if (nLen2)
		{
			unsigned int dwCurr = 0;
			for (n=0; n<3; n++)
			{
				if (n<(nSrcLen%3))
					dwCurr |= *pbSrcData++;
				dwCurr <<= 8;
			}
			for (k=0; k<nLen2; k++)
			{
				unsigned char b = (unsigned char)(dwCurr>>26);
				*szDest++ = s_chBase64EncodingTable[b];
				dwCurr <<= 6;
			}
			nWritten+= nLen2;
			if ((dwFlags & BASE64_FLAG_NOPAD)==0)
			{
				nLen3 = nLen2 ? 4-nLen2 : 0;
				for (j=0; j<nLen3; j++)
				{
					*szDest++ = '=';
				}
				nWritten += nLen3;
			}
		}

		*pnDestLen = nWritten;

		return 1;
}

int DecodeBase64Char(unsigned int ch)
{
	// returns -1 if the character is invalid
	// or should be skipped
	// otherwise, returns the 6-bit code for the character
	// from the encoding table
	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 0;	// 0 range starts at 'A'
	if (ch >= 'a' && ch <= 'z')
		return ch - 'a' + 26;	// 26 range starts at 'a'
	if (ch >= '0' && ch <= '9')
		return ch - '0' + 52;	// 52 range starts at '0'
	if (ch == '+')
		return 62;
	if (ch == '/')
		return 63;
	return -1;
}

bool Base64Decode(const char* szSrc, int nSrcLen, unsigned char* pbDest, int *pnDestLen)
{
	// walk the source buffer
	// each four character sequence is converted to 3 bytes
	// CRLFs and =, and any characters not in the encoding table
	// are skiped

	if (!szSrc || !pbDest || !pnDestLen)
	{
		return false;
	}

	const char* szSrcEnd = szSrc + nSrcLen;
	int nWritten = 0;
	while (szSrc < szSrcEnd)
	{
		DWORD dwCurr = 0;
		int i;
		int nBits = 0;
		for (i=0; i<4; i++)
		{
			if (szSrc >= szSrcEnd)
				break;
			int nCh = DecodeBase64Char(*szSrc);
			szSrc++;
			if (nCh == -1)
			{
				// skip this char
				i--;
				continue;
			}
			dwCurr <<= 6;
			dwCurr |= nCh;
			nBits += 6;
		}
		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		dwCurr <<= 24-nBits;
		for (i=0; i<nBits/8; i++)
		{
			*pbDest = (uint8_t) ((dwCurr & 0x00ff0000) >> 16);
			dwCurr <<= 8;
			pbDest++;
			nWritten++;
		}
	}

	*pnDestLen = nWritten;
	return true;
}



