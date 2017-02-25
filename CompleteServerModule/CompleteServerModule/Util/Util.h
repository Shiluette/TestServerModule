#pragma once
#include<algorithm>

#define snprintf(dst, format, ...)     _snprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)
#define snwprintf(dst, format, ...)    _snwprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)

#define fixInRange(minimum, x, maximum)     min(maximum, max(x, minimum)) 
#define isInRange(minimum, x, maximum)      (x == fixInRange(minimum, x, maximum)) ? true : false




//컴파일 기본 메크로 회피용 __FUNCTION__ 같은..
#define __W(x)              L##x
#define _W(x)               __W(x)

//-------------------------------------------------------------------//
//문자열 변환   멀티바이트  -> 유니코드  or 유니코드 ->  멀티바이트
inline void StrConvA2T(CHAR *src, TCHAR *dest, size_t destLen) {
#ifdef  UNICODE                     // r_winnt
	if (destLen < 1) {
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
#endif
}

inline void StrConvT2A(TCHAR *src, CHAR *dest, size_t destLen) {
#ifdef  UNICODE                     // r_winnt
	if (destLen < 1) {
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
#endif
}

inline void StrConvA2W(CHAR *src, WCHAR *dest, size_t destLen) {
	if (destLen < 1) {
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
}
inline void StrConvW2A(WCHAR *src, CHAR *dest, size_t destLen) {
	if (destLen < 1) {
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
}
//-------------------------------------------------------------------//

template<class T>
inline T VolatileCast(volatile T num)
{
	T * num2 = const_cast<T *>(&num);
	return *num2;
}



#undef	SAFE_DELETE
#define SAFE_DELETE(obj)						\
{												\
	if ((obj)) delete(obj);		    			\
    (obj) = 0L;									\
}
// delete object array
#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(arr)					\
{												\
	if ((arr)) delete [] (arr);		    		\
    (arr) = 0L;									\
}

// delete gameObject
#define SAFE_FREE(obj)							\
{												\
	if ((obj)) obj->free();						\
    (obj) = 0L;									\
}