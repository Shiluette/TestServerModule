#include "stdafx.h"
#include "Log.h"

LogPrintf::LogPrintf()
{
	printf("- Log create : printf Log Mode\n");
}

void LogPrintf::log(WCHAR * logString)
{
	// 햇갈리기 쉬운 %s , %S -> 
	// %s 사용하는 함수와 동일한 문자열 타입으로 출력
	// %S 사용하는 함수와 반대되는 문자열 타입으로 출력
	// wachr_t * str;
	// printf("%S", str); 정답
	// %ws : 함수와 상관없이 'wchar_t *' 로 변환
	// %hs : 함수와 상관없이 'char *' 로 변환
	printf("%ws", logString);
}

LogFile::LogFile()
{
}
