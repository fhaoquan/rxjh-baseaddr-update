#ifndef __CUPDATEBASEADDR_H_
#define __CUPDATEBASEADDR_H_  
#include "stdafx.h"

#include <windows.h>


class CUpdateAddr{

private:


	void HexStrToX(IN OUT CHAR* szpHexbuf); //通配符的支持
	DWORD bytesToHexStr(IN BYTE *nbData, DWORD nbDataSize, OUT char* szpHexbuf); //字节集转化为十六进制
	bool HexStrCmp(IN const char* hexStr1, IN const char* hexStr2);  //十六进制字符串比较
	char IsLowToBigCase(char c); //如果是小写字母则返回大写字母
	bool HexStrToBigCase(IN OUT char* szHexstr);

	BOOL CharToHex(OUT char* szpHexbuf, BYTE c);
	BOOL IsHexStr(char c);
	//DWORD  ReadBaseAddr( DWORD ndCurAddr, DWORD ndSize);

	
public:
	DWORD ScanFeatureCode(
		IN OUT char* szpFeatureCode,
		DWORD ndStartAddress = 00400000,
		DWORD ndEndAddress = 0x7FFFFFFF); //扫描特征码的函数
	CUpdateAddr(HANDLE hd);
	~CUpdateAddr();
	DWORD  ReadBaseAddr(DWORD ndCurAddr, DWORD ndSize);
	//DWORD OneKeyUpdate(char* szpFeaturCode , DWORD AddrOffset);

private:
	const DWORD PageSize = 0x1024;
	HANDLE handle;

};


#endif