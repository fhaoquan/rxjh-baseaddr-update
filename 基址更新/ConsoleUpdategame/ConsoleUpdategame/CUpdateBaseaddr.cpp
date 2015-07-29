#include "stdafx.h"

#include "CUpdateBaseaddr.h"
#include <stdio.h>



void CUpdateAddr::HexStrToX(IN OUT CHAR* szpHexbuf) //通配符的支持
{
	DWORD ndStrlen = strlen(szpHexbuf);

	for (DWORD i = 0; i < ndStrlen; i++)
	{
		if (FALSE == IsHexStr(szpHexbuf[i])){
			szpHexbuf[i] = 'X';
		}
	}
	return;

}
DWORD CUpdateAddr::bytesToHexStr(IN BYTE *nbData, DWORD nbDataSize, OUT char* szpHexbuf) //字节集转化为十六进制
{


	for (DWORD i = 0; i < nbDataSize; i++)
	{
		CharToHex(&szpHexbuf[i * 2], nbData[i]);
	}


	return TRUE;
}
bool CUpdateAddr::HexStrCmp(IN const char* hexStr1, IN const char* hexStr2)  //十六进制字符串比较
{


	DWORD ndHexlen1 = strlen(hexStr1);
	DWORD ndHexlen2 = strlen(hexStr2);


	if (ndHexlen1 > ndHexlen2)
	{
		ndHexlen1 = ndHexlen2;
	}

	for (DWORD i = 0; i < ndHexlen1; i++)
	{
		if (hexStr1[i] == 'X' || hexStr2[i] == 'X')
		{
			continue;
		}

		if (hexStr1[i] == hexStr2[i])
		{
			continue;
		}

		return FALSE;
	}


	return TRUE;


}
char CUpdateAddr::IsLowToBigCase(char c) //如果是小写字母则返回大写字母
{
	if (c >= 'a' && c <= 'z')
	{
		return c = c + 'A' - 'a';
	}

	return c;

}
bool CUpdateAddr::HexStrToBigCase(IN OUT char* szHexstr)
{

	DWORD ndstrLen = strlen(szHexstr);

	for (DWORD i = 0; i < ndstrLen; i++)
	{
		szHexstr[i] = IsLowToBigCase(szHexstr[i]);
	}

	return TRUE;
}


CUpdateAddr::CUpdateAddr(HANDLE hd){

	handle = hd;
}
CUpdateAddr::~CUpdateAddr()
{

	handle = NULL;
}



DWORD CUpdateAddr::ScanFeatureCode(
	IN OUT char* szpFeatureCode,
	DWORD ndStartAddress,
	DWORD ndEndAddress) //扫描特征码的函数
{
	//支持通配符
	HexStrToX(szpFeatureCode);
	//把特征码转化为大写字符串
	HexStrToBigCase(szpFeatureCode);

	DWORD ndLenHexCode = strlen(szpFeatureCode);

	BYTE* nbDataBuf = new BYTE[PageSize + ndLenHexCode / 2 + 2];
	
	DWORD RealSize = 0;

	for (DWORD ndCurAddr = ndStartAddress; ndCurAddr < ndEndAddress - ndLenHexCode / 2; ndCurAddr += PageSize)
	{

		ReadProcessMemory(handle, (LPVOID)ndCurAddr, (LPVOID)nbDataBuf, PageSize + ndLenHexCode / 2 + 2, &RealSize);

		for (DWORD i = 0; i < PageSize; i++)
		{
			char szpTemp[256] = "";
			bytesToHexStr(&nbDataBuf[i], ndLenHexCode / 2 , szpTemp);
			if (HexStrCmp(szpTemp, szpFeatureCode) != FALSE){
				return ndCurAddr + i;
			}
		}
	}


	return NULL;

}


BOOL CUpdateAddr::CharToHex(OUT char* szpHexbuf, BYTE c){

	BYTE bLow = 0;
	BYTE bHigh = 0;

	bLow = c % 16;
	bHigh = c / 16;

	if (bHigh >9)
	{
		szpHexbuf[0] = 'A' + bHigh - 10;
	}
	else
	{
		szpHexbuf[0] = '0' + bHigh;
	}


	if (bLow > 9){
		szpHexbuf[1] = 'A' + bLow - 10;
	}
	else
	{
		szpHexbuf[1] = '0' + bLow;
	}

	szpHexbuf[2] = '\0';

	return TRUE;


}
BOOL CUpdateAddr::IsHexStr(char c){

	if ( c >= '0' && c <= '9')
	{
		return TRUE;
	} else if ( c >= 'a' && c <= 'z')
	{
		return TRUE;
	}else if (c >= 'A' && c <= 'Z')
	{
		return TRUE;
	}
	else
	{

		return FALSE;
	}


}
DWORD  CUpdateAddr::ReadBaseAddr( DWORD ndCurAddr, DWORD ndSize){

	DWORD ndTemp = NULL;

	ReadProcessMemory(handle, (LPVOID)ndCurAddr, &ndTemp, ndSize, NULL);

	return ndTemp;

}


//DWORD CUpdateAddr::OneKeyUpdate(char* szpFeaturCode, DWORD AddrOffset){
//
//	DWORD ndBCurAddr = NULL;
//	DWORD ndBaseAddr = NULL;
//
//
//	ndBCurAddr = this->ScanFeatureCode(szpFeaturCode);
//	ndBaseAddr = ReadBaseAddr(ndBCurAddr + AddrOffset, 4);
//
//
//	return ndBaseAddr;
//}