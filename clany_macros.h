#ifndef __CLANY_MACROS_H__
#define __CLANY_MACROS_H__

#define _CLANY_BEGIN	namespace clany {
#define _CLANY_END		}

#if defined CLANYAPI_EXPORTS
#define CLANY_EXPORTS __declspec(dllexport)
#else
#define CLANY_EXPORTS
#endif

#if defined (min) || defined (max)
#undef max
#undef min
#endif

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

_CLANY_BEGIN
using namespace std;
_CLANY_END

#endif
