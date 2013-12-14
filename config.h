#pragma once

#if defined(_MSC_VER)

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define USE_SCRYPT
#define HAVE_OPENCL
#define WANT_X8664_SSE4

#define HAVE_ADL

//#define WANT_X8664_SSE2
//#define WANT_X8632_SSE2
//#define WANT_SSE2_4WAY
//#define WANT_ALTIVEC_4WAY

#define PHATK_KERNNAME "phatk120724"
#define POCLBM_KERNNAME "poclbm120724"
#define DIAKGCN_KERNNAME "diakgcn120724"
#define DIABLO_KERNNAME "diablo120724"
#define SCRYPT_KERNNAME "scrypt120724"


#define PRIi64 "I64d"
#define PRIi32 "I32d"
#define PRIu32 "I32u"
#define PRIu64 "I64u"

#define PATH_MAX MAX_PATH

#define VERSION "v2.6.2"

#define PACKAGE_NAME "cgminer"
#define PACKAGE_TARNAME "cgminer"
#define PACKAGE_VERSION "2.4.2"
#define PACKAGE_STRING "cgminer 2.4.2"
#define PACKAGE "cgminer"
#define CGMINER_PREFIX ""

#define R_OK 0

#define snprintf _snprintf

#include <io.h>
#include <process.h>

#define va_copy(a, b) memcpy(&(a), &(b), sizeof(va_list))

#ifdef _WIN64
//#define __x86_64__
#endif

//#define USE_AVX2 1

#define inline __inline

#include <stdint.h>
#include <ctype.h>
#include <stdio.h>

typedef intptr_t ssize_t;


struct timezone2 
{
	__int32  tz_minuteswest; /* minutes W of Greenwich */
	__int32  tz_dsttime;     /* type of dst correction */
};


#include <time.h>
#include <WinSock2.h>

const __int64 DELTA_EPOCH_IN_MICROSECS= 11644473600000000;

/* IN UNIX the use of the timezone struct is obsolete;
I don't know why you use it. See http://linux.about.com/od/commands/l/blcmdl2_gettime.htm
But if you want to use this structure to know about GMT(UTC) diffrence from your local time
it will be next: tz_minuteswest is the real diffrence in minutes from GMT(UTC) and a tz_dsttime is a flag
indicates whether daylight is now in use
*/

inline int gettimeofday(struct timeval *tv/*in*/, struct timezone2 *tz/*in*/)
{
	FILETIME ft;
	__int64 tmpres = 0;
	TIME_ZONE_INFORMATION tz_winapi;
	int rez=0;

	ZeroMemory(&ft,sizeof(ft));
	ZeroMemory(&tz_winapi,sizeof(tz_winapi));

	GetSystemTimeAsFileTime(&ft);

	tmpres = ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;

	/*converting file time to unix epoch*/
	tmpres /= 10;  /*convert into microseconds*/
	tmpres -= DELTA_EPOCH_IN_MICROSECS; 
	tv->tv_sec = (__int32)(tmpres*0.000001);
	tv->tv_usec =(tmpres%1000000);

	if( tz )
	{
		//_tzset(),don't work properly, so we use GetTimeZoneInformation
		rez=GetTimeZoneInformation(&tz_winapi);
		tz->tz_dsttime=(rez==2)?true:false;
		tz->tz_minuteswest = tz_winapi.Bias + ((rez==2)?tz_winapi.DaylightBias:0);
	}

	return 0;
}

inline int strcasecmp(const char *s1, const char *s2)
{
	unsigned char c1,c2;
	do {
		c1 = *s1++;
		c2 = *s2++;
		c1 = (unsigned char) tolower( (unsigned char) c1);
		c2 = (unsigned char) tolower( (unsigned char) c2);
	}
	while((c1 == c2) && (c1 != '\0'));
	return (int) c1-c2;
}

inline int strncasecmp(const char *s1,	const char *s2, size_t n)
{
	if (n == 0)
		return 0;

	while (n-- != 0 && tolower(*s1) == tolower(*s2))
	{
		if (n == 0 || *s1 == '\0' || *s2 == '\0')
			break;
		s1++;
		s2++;
	}

	return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}

#include <math.h>
inline long double roundl(long double r)
{
	return (r>0.0) ? floor(r+0.5f) : ceil(r-0.5);
}


inline void* memmem (void* buf, size_t buflen, void* pat, size_t patlen) 
{ 
	void* end = (char *) buf+buflen-patlen; 
	while (buf=memchr (buf, ((char *) pat) [0], buflen)) 
	{ 
		if (buf> end) 
			return 0; 
		if (memcmp (buf, pat, patlen) == 0) 
			return buf; 
		buf = (char *) buf+1; 
	}
	return 0; 
}


#endif

