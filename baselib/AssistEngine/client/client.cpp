// client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <list>
#include "RecycleBin.h"
#include "inifile.h"
#include <Windows.h>
#include "path.h"

using namespace std;
class A
{
public:
	A();
	~A();
private:
	int a;
};

A::A()
{
	a = 100;
}

A::~A()
{
	a = 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	struct Task
    {
        int Run(const char* pszPath, WIN32_FIND_DATAA& FindData)
        {
            if ( !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                m_FileList.push_back(pszPath);
            }

            return true;
        }
        std::list<std::string> m_FileList;
    } FileTask;

    struct TaskW
    {
        int Run(const wchar_t* pwszPath, WIN32_FIND_DATAW& FindData)
        {
            if ( !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                m_FileList.push_back(pwszPath);
            }

            return true;
        }
        std::list<std::wstring> m_FileList;
    } FileTaskW;

    MakeDir("E:\\lua_build1\\a\\b\\");
    MakeDirW(L"E:\\lua_build2\\a\\b\\");

    WalkDir( "E:\\lua_build",  &FileTask, "*.exe" );
    WalkDir( "E:\\lua_build",  &FileTask );

    WalkDirW( L"E:\\lua_build",  &FileTaskW, L"*.exe" );
    WalkDirW( L"E:\\lua_build",  &FileTaskW );

	IniFile Ini;

	Ini.Load("config.ini", "w");

	int bEnable = 0;
	float fAngle = 0.0f;
	double lfAngle = 0.0;
	char szPath[128];

	//Ini.GetInt("UIEditor", "Enable", 0, bEnable);
	//Ini.GetString("UIEditor", "Path", NULL, szPath, _countof(szPath));

	//Ini.GetString("KG3DENGINE", "ModelShadowParams", NULL, szPath, _countof(szPath));

	//Ini.GetFloat("KG3DENGINE", "CammeraAngle", 1.2f, fAngle);
	//Ini.GetDouble("KG3DENGINE", "CammeraAngle", 1.2, lfAngle);

	//Ini.WriteFloat("KG3DENGINE", "CammeraAngle", 5.5f);

	//Ini.WriteString("KG3DENGINE", "Name", "shenliqiao");

	//Ini.WriteInt("KG3DENGINE", "NameId", 123);

	//Ini.WriteDouble("KG3DENGINE", "NameX", 123.0);
	//Ini.WriteFloat("KG3DENGINE", "NameY", 123.0f);
	//
	int StandardCanvasWidth = 0;
	const char* pszNextSection = NULL;
	pszNextSection = Ini.NextSection(NULL);
	while (pszNextSection)
	{
		if (strcmp(pszNextSection, "UIEditor") == 0)
			Ini.GetInt("UIEditor", "Enable", 0, bEnable);

		if (strcmp(pszNextSection, "Main") == 0)
			Ini.GetInt("Main", "StandardCanvasWidth", 0, StandardCanvasWidth);

		

		pszNextSection = Ini.NextSection(pszNextSection);
	}
	//Ini.RemoveSection("MODLEEDITOR");
	//Ini.RemoveSection("JX3Profile");

	Ini.Save("config_v1.ini");

	return 0;
}

