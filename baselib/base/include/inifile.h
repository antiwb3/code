//---------------------------------------------------------------------------
// File:	inifile.h
// Date:	2013.04.20
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------

#ifndef INI_FILE_H_
#define INI_FILE_H_


#ifndef ENGINE_API
    #define ENGINE_API extern
#endif

#include <map>
#include <vector>

struct IniKey
{
	char* pszKey;
	char* pszValue;
};

struct LessEqual
{	
	bool operator()(const char * _Left, const char* _Right) const
	{	
		return (strcmp(_Left, _Right) < 0);
	}
};

struct KeyData
{
	const char* pszKey;
	const char* pszValue;
	int nPriority;
};
typedef std::vector<KeyData *> KeyDataVector;

typedef std::map<const char *, KeyData, LessEqual> IniKeyMap;

struct IniSection
{
	char*		pszSection;
	IniKeyMap	KeyMap;		
	int			nOrder;

	IniSection* pNext;
};

struct IniSectionList
{
	IniSection* m_pLast;
	IniSection* m_pHeader;
	size_t		m_uCount;
};

class ENGINE_API IniFile
{
public:
	IniFile();
	~IniFile();

	int Load(const char* cpszFile, const char* cpszMode = NULL);// "r": 或者为NULL 的时候 加载后只用来获取不做修改，"w": 加载后会作修改

	int Save(const char* cpszFile);

	int GetString(const char* cpszSection, const char* cpszKey, const char* cpszDefault, char* pszValue, size_t uLen);

	int GetInt(const char* cpszSection, const char* cpszKey, int nDefault, int& rnValue);
	
	int GetFloat(const char* cpszSection, const char* cpszKey, float fDefault, float& rfValue);
	
	int GetDouble(const char* cpszSection, const char* cpszKey, double lfDefault, double& rlfValue);

	int WriteString(const char* cpszSection, const char* cpszKey, const char* cpszValue);

	int WriteInt(const char* cpszSection, const char* cpszKey, int nValue);

	int WriteFloat(const char* cpszSection, const char* cpszKey, float fValue);

	int WriteDouble(const char* cpszSection, const char* cpszKey, double lfValue);

	int RemoveSection(const char* cpszSection);

	const char* NextSection(const char* cpszSection);
	
	//int NextKey(const char* cpszKey, const char* cpszNextKey);
	
	int GetSectionCount();

	int IsSectionExist(const char *cpszSection);

private:
	int ParseLine(char* pszLine, size_t uLen);

	int ParseSection(char* pszLine, size_t uLen);

	int ParseKey(char* pszLine, size_t uLen);
	
	int AppendKey(IniSection* pSection, const char* cpszKey, const char* cpszValue);

	IniSection* AppendSection(const char* cpszSection);

	IniSection* GetSection(const char* cpszSection);

	const char* GetValue(const char* cpszSection, const char * cpszKey);

	inline int  IsSection(const char* pszLine, size_t uLen);

	inline void Free(IniSection* pSection);
private:
	unsigned char*	m_pbyData;
	unsigned char	m_bReadOnly; 
	IniSection*		m_pSection;
	IniSectionList	m_SectionList;
};

#endif // INI_FILE_H_