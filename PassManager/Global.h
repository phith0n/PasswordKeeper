#pragma once
#include <Windows.h>

bool EnablePriv(TCHAR szPriv[], bool isEnable)
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return false;

	if ( ! LookupPrivilegeValue( NULL, szPriv, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = isEnable ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
		CloseHandle( hToken );

	return true;
}

