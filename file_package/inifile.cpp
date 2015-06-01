//---------------------------------------------------------------------------
// File:	inifile.cpp
// Date:	2013.04.20
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "fileassistor.h"
#include <string.h>
#include "inifile.h"

#if !defined(ASSISTOR_NO_STDLIB)
	#include <cstdlib>      // For std::size_t
	#include <assert.h>
	#include <tchar.h>
#endif



IniFile::IniFile()
: m_pbyData(NULL)
, m_bReadOnly(0)
, m_pSection(NULL)
{
	memset(&m_SectionList, 0, sizeof(m_SectionList));
}

IniFile::~IniFile()
{
	IniSection* pDelete  = NULL;
	IniSection* pSection = m_SectionList.m_pHeader;

	while (pSection)
	{
		pDelete  = pSection;
		pSection = pSection->pNext;
		Free(pDelete);
	}
	assert(m_SectionList.m_uCount == 0);

	if (m_pbyData)
		delete[] m_pbyData;
}

int IniFile::Load(const char* cpszFile, const char* cpszMode /*= NULL*/)
{
	assert(cpszFile);

	int   nResult	  = false;

	int   nBytes	  = 0;
	int   nOffset	  = 0;
	int   nNextOffset = 0;
	int   nLineOffset = 0;
	char* pszContent  = NULL;
	char* pszLine	  = NULL;

	nBytes = fileContent(cpszFile, (unsigned char** )&pszContent);
	if (nBytes == -1)
		goto Exit0;

	if (!cpszMode || strcmp(cpszMode, "r") == 0)
		m_bReadOnly = true;
	else if (strcmp(cpszMode, "w") == 0)
		m_bReadOnly = false;
	else
		goto Exit0;

	m_pSection = NULL;
	do 
	{
		nLineOffset = readLine(pszContent + nOffset, nBytes - nOffset, nNextOffset);
		if (nLineOffset != -1)
		{
			pszLine =  pszContent + nOffset;
			nOffset += nNextOffset;

			ParseLine(pszLine, nLineOffset);
		}

	} while (nLineOffset != -1 && nOffset < nBytes);

	m_pbyData = (unsigned char* )pszContent;
	nResult = true;
Exit0:
	if (!nResult)
		delete[] pszContent;

	return nResult;
}

int IniFile::Save(const char* cpszFile)
{
	FILE* pFile = NULL;
	int   nResult = false;
	int	  nOffset = 0;
	int   nLen   = 0;

	char szBuff[2048];
	
	fopen_s(&pFile, cpszFile, "w");
	if (!pFile)
		goto Exit0;

	for (IniSection* pSection = m_SectionList.m_pHeader; pSection; pSection = pSection->pNext)
	{
		nLen = (int)strlen(pSection->pszSection) + 3;

		if (nOffset + nLen >= _countof(szBuff))
		{
			fwrite(szBuff, sizeof(char), nOffset, pFile);
			nOffset = 0;
			szBuff[0] = '\0';
		}

		assert(nOffset + nLen < _countof(szBuff));
		if (nOffset + nLen < _countof(szBuff))
		{
			sprintf_s(szBuff + nOffset, _countof(szBuff) - nOffset, "[%s]\n", pSection->pszSection);
			nOffset += nLen;
		}

		IniKeyMap& rMap = pSection->KeyMap;
		for (IniKeyMap::iterator it = rMap.begin(); it != rMap.end(); ++it)
		{
			nLen  = (int)strlen(it->first);
			nLen += (int)strlen(it->second);
			nLen += 2;

			if (nOffset + nLen >= _countof(szBuff))
			{
				fwrite(szBuff, sizeof(char), nOffset, pFile);
				nOffset = 0;
				szBuff[0] = '\0';
			}
			
			assert(nOffset + nLen < _countof(szBuff));
			if (nOffset + nLen < _countof(szBuff))
			{
				sprintf_s(szBuff + nOffset, _countof(szBuff) - nOffset, "%s=%s\n", it->first, it->second);
				nOffset += nLen;
			}
		}

		if (nOffset + 1 >= _countof(szBuff))
		{
			fwrite(szBuff, sizeof(char), nOffset, pFile);
			nOffset = 0;
			szBuff[0] = '\0';
		}
		szBuff[nOffset] = '\n';
		nOffset += 1;

	}

	if (nOffset > 0)
	{
		fwrite(szBuff, sizeof(char),  nOffset, pFile);
		nOffset = 0;
	}

	nResult = true;
Exit0:
	if (pFile)
		fclose(pFile);

	return nResult;
}

int IniFile::GetString(const char* cpszSection, const char* cpszKey, const char* cpszDefault, char* pszValue, size_t uLen)
{
	assert(cpszSection);
	assert(cpszKey);
	assert(uLen > 0);

	char* cpszFind = (char* )GetValue(cpszSection, cpszKey);
	if (cpszFind && uLen > 0)
	{
		strcpy_s(pszValue, uLen, cpszFind);
		pszValue[uLen - 1] = '\0';
		return true;
	}

	if (uLen > 0)
	{
		strcpy_s(pszValue, uLen, cpszDefault);
		pszValue[uLen - 1] = '\0';
	}
	return false;
}

int IniFile::GetInt(const char* cpszSection, const char* cpszKey, int nDefault, int& rnValue)
{
	assert(cpszSection);
	assert(cpszKey);

	char* cpszFind = (char* )GetValue(cpszSection, cpszKey);
	if (cpszFind)
	{
		rnValue = (int)strtoul(cpszFind, NULL, 10);
		return true;
	}
	rnValue = nDefault;
	return false;
}

int IniFile::GetFloat(const char* cpszSection, const char* cpszKey, float fDefault, float& rfValue)
{
	assert(cpszSection);
	assert(cpszKey);

	char* cpszFind = (char* )GetValue(cpszSection, cpszKey);
	if (cpszFind)
	{
		rfValue = (float)strtod(cpszFind, NULL);

		return true;
	}
	rfValue = fDefault;
	return false;
}

int IniFile::GetDouble(const char* cpszSection, const char* cpszKey, double lfDefault, double& rlfValue)
{
	assert(cpszSection);
	assert(cpszKey);

	char* cpszFind = (char* )GetValue(cpszSection, cpszKey);
	if (cpszFind)
	{
		rlfValue = strtod(cpszFind, NULL);

		return true;
	}
	rlfValue = lfDefault;
	return false;
}

const char* IniFile::GetValue(IniSection* pSection, const char* cpszKey)
{
	if (!pSection || !cpszKey)
		return NULL;

	IniKeyMap::iterator it = pSection->KeyMap.find(cpszKey);
	if (it != pSection->KeyMap.end())
	{
		return it->second;
	}
	return NULL;
}

int IniFile::WriteString(const char* cpszSection, const char* cpszKey, const char* cpszValue)
{
	assert(cpszSection);
	assert(cpszKey);

	if (m_bReadOnly)
		return false;

	IniSection* pSection = GetSection(cpszSection);
	if (pSection)
	{
		return AppendKey(pSection, cpszKey, cpszValue);
	}
    else
    {
        pSection = AppendSection(cpszSection);
        return AppendKey(pSection, cpszKey, cpszValue);
    }
}

int IniFile::WriteInt(const char* cpszSection, const char* cpszKey, int nValue)
{
	char szValue[64];

	if (m_bReadOnly)
		return false;

	sprintf_s(szValue, _countof(szValue) - 1, "%d", nValue);

	return WriteString(cpszSection, cpszKey, szValue);
}

int IniFile::WriteFloat(const char* cpszSection, const char* cpszKey, float fValue)
{
	char szValue[64];

	if (m_bReadOnly)
		return false;

	sprintf_s(szValue, _countof(szValue) - 1, "%f", fValue);

	return WriteString(cpszSection, cpszKey, szValue);
}

int IniFile::WriteDouble(const char* cpszSection, const char* cpszKey, double lfValue)
{
	char szValue[64];

	if (m_bReadOnly)
		return false;

	sprintf_s(szValue, _countof(szValue) - 1, "%lf", lfValue);

	return WriteString(cpszSection, cpszKey, szValue);
}

int IniFile::RemoveSection(const char* cpszSection)
{
	if (m_bReadOnly)
		return false;

	IniSection* pSectionPrev = NULL; 
	IniSection* pSection = m_SectionList.m_pHeader;
	for (; pSection; pSection = pSection->pNext)
	{
		if (strcmp(pSection->pszSection, cpszSection) == 0)
		{
			if (pSectionPrev)
				pSectionPrev->pNext = pSection->pNext;
			else
				m_SectionList.m_pHeader = pSection->pNext;

			if (pSection == m_SectionList.m_pLast)
				m_SectionList.m_pLast = pSectionPrev;

			break;
		}
		pSectionPrev = pSection; 
	}
	
	if (m_pSection == pSection)
		m_pSection = NULL;

	if (pSection)
		Free(pSection);

	return true;
}

const char* IniFile::NextSection(const char* cpszSection)
{
	IniSection* pNextSection = NULL;
	if (!cpszSection)
	{
		pNextSection = m_SectionList.m_pHeader;
		m_pSection	 = pNextSection;
	}
	else
	{
		pNextSection = GetSection(cpszSection);
		if (pNextSection)
		{
			pNextSection = pNextSection->pNext;
			m_pSection	 = pNextSection;
		}
	}
	return (pNextSection ? (pNextSection->pszSection) : NULL);
}

//int IniFile::NextKey(const char* cpszKey, const char* cpszNextKey)
//{
//	if (m_pSection)
//	{
//		m_pSection.KeyMap.find(cpszKey);
//	}
//}

int IniFile::GetSectionCount()
{
	return (int)m_SectionList.m_uCount;
}

int IniFile::IsSectionExist(const char *cpszSection)
{
	return (GetSection(cpszSection) != NULL);
}

IniSection* IniFile::FindSection(const char* cpszSection /*= 0*/)
{
	if (!cpszSection)
	{
		m_pSection	 = m_SectionList.m_pHeader;
		return m_pSection;
	}
	else
	{
		IniSection* pSec = GetSection(cpszSection);
		if (pSec)
			m_pSection = pSec;

		return pSec;
	}
	return NULL;
}

IniSection* IniFile::GetSection(const char* cpszSection)
{
	if (m_pSection && strcmp(m_pSection->pszSection, cpszSection) == 0)
		return m_pSection;

	for (IniSection* pSection = m_SectionList.m_pHeader; pSection; pSection = pSection->pNext)
	{
		if (strcmp(pSection->pszSection, cpszSection) ==0)
		{
			m_pSection = pSection;
			return pSection;
		}
	}
	return NULL;
}

const char* IniFile::GetValue(const char* cpszSection, const char* cpszKey)
{
	IniSection* pSection = GetSection(cpszSection);
	if (pSection)
	{
		IniKeyMap::iterator it = pSection->KeyMap.find(cpszKey);
		if (it != pSection->KeyMap.end())
			return it->second;
	}
	return NULL;
}

int IniFile::ParseLine(char* pszLine, size_t uLen)
{
	if (IsSection(pszLine, uLen))
	{
		ParseSection(pszLine, uLen);
	}
	else if (m_pSection)
	{
		ParseKey(pszLine, uLen);
	}

	return true;
}

int IniFile::ParseSection(char* pszLine, size_t uLen)
{
	if (uLen > 2)
	{
		IniSection* pSection	= NULL;
		const char* pszSection	= pszLine + 1;
		pszLine[uLen - 1] = '\0';

		pSection = AppendSection(pszSection);
		if (!pSection)
			goto Exit0;

		m_pSection = pSection;
	}
	
	return true;
Exit0:
	return false;
}

int IniFile::ParseKey(char* pszLine, size_t uLen)
{
	if (uLen > 1 && m_pSection)
	{
		pszLine[uLen] = '\0';
		char* pszFind = (char *)strchr(pszLine, '=');
		if (!pszFind)
			goto Exit0;

		char* pszKey = pszLine;
		pszFind[0]	 = '\0';

		char* pszValue = pszFind + 1;

		if (m_bReadOnly)
		{
			m_pSection->KeyMap.insert(std::make_pair(pszKey, pszValue));
		}
		else
		{
			return AppendKey(m_pSection, pszKey, pszValue);
		}
	}

	return true;
Exit0:
	return false;
}

int IniFile::IsSection(const char* pszLine, size_t uLen)
{
	if (uLen > 0 && pszLine[0] == '[' && pszLine[uLen - 1] == ']')
		return true;

	return false;
}

IniSection* IniFile::AppendSection(const char* cpszSection)
{
	IniSection* pResult  = NULL;
	IniSection* pSection = new IniSection;
	if (!pSection)
		goto Exit0;

	pSection->pNext		 = NULL;
	pSection->pszSection = NULL;
	if (m_bReadOnly)
	{
		pSection->pszSection = (char *)cpszSection;
	}
	else
	{
		size_t uLen = strlen(cpszSection) + 1;
		char* pszSection = new char[uLen];
		if (!pszSection)
			goto Exit0;

		strcpy_s(pszSection, uLen, cpszSection);
		pszSection[uLen - 1] = '\0';
		pSection->pszSection = pszSection;
	}

	if (!m_SectionList.m_pHeader)
	{
		m_SectionList.m_pHeader = pSection;
		m_SectionList.m_pLast	= pSection;
		pSection->pNext = NULL;
	}
	else
	{
		assert(m_SectionList.m_pLast);
		m_SectionList.m_pLast->pNext = pSection;
		pSection->pNext = NULL;

		m_SectionList.m_pLast = pSection;
	}
	m_SectionList.m_uCount++;

	pResult = pSection;
Exit0:
	if (!pResult)
	{
		if(pSection)
			delete pSection;
	}
	return pResult;
}

int IniFile::AppendKey(IniSection* pSection, const char* cpszKey, const char* cpszValue)
{
	assert(pSection);
	assert(cpszKey);
	assert(cpszValue);

	IniKeyMap& rMap			= pSection->KeyMap;
	IniKeyMap::iterator it	= rMap.find(cpszKey);
	if (it != rMap.end())
	{
		char* pszValue = (char *)it->second;
		size_t uSize = strlen(cpszValue) + 1;
		pszValue = (char *)realloc(pszValue, uSize);
		if (!pszValue)
			goto Exit0;

		strcpy_s(pszValue, uSize, cpszValue);
		pszValue[uSize - 1] = '\0';
		it->second = pszValue;
	}
	else
	{
		size_t uKey   = strlen(cpszKey)   + 1;
		size_t uValue = strlen(cpszValue) + 1;

		char* pszKey = new char[uKey];
		if (!pszKey)
			goto Exit0;

		char* pszValue = new char[uValue];
		if (!pszValue)
			goto Exit0;


		strcpy_s(pszKey,   uKey,   cpszKey);
		pszKey[uKey - 1] = '\0';

		strcpy_s(pszValue, uValue, cpszValue);
		pszValue[uValue - 1] = '\0';

		rMap.insert(std::make_pair(pszKey, pszValue));
	}

	return true;
Exit0:
	return false;
}

void IniFile::Free(IniSection* pSection)
{
	assert(pSection);

	if (!m_bReadOnly)
	{
		for (IniKeyMap::iterator it = pSection->KeyMap.begin(); it != pSection->KeyMap.end(); ++it)
		{
			delete[] it->first;
			delete[] it->second;
		}
	}

	delete pSection;
	pSection = NULL;	

	m_SectionList.m_uCount--;
}