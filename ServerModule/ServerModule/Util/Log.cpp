#include "stdafx.h"
#include "Log.h"

LogPrintf::LogPrintf()
{
	printf("- Log create : printf Log Mode\n");
}

void LogPrintf::log(WCHAR * logString)
{
	// �ް����� ���� %s , %S -> 
	// %s ����ϴ� �Լ��� ������ ���ڿ� Ÿ������ ���
	// %S ����ϴ� �Լ��� �ݴ�Ǵ� ���ڿ� Ÿ������ ���
	// wachr_t * str;
	// printf("%S", str); ����
	// %ws : �Լ��� ������� 'wchar_t *' �� ��ȯ
	// %hs : �Լ��� ������� 'char *' �� ��ȯ
	printf("%ws", logString);
}

LogFile::LogFile()
{
}
