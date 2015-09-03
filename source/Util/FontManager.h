/*************************************************************
	File:			FontManager.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		convert string characters into bitmapped images
*************************************************************/
#pragma once

#include <windows.h>
#include "Character.h"
#include <vector>

enum FONTS {ARIAL, ARNOVA, ARNOVA_PURPLE, CHINESE_TAKEAWAY};

class FontManager
{
public:


	struct CharSet
	{
		std::string  szLabel;
		int			 m_nFontImg;
		int			 m_nSpace;
		int			 m_nNewLine;
		CCharacter   Char[256];
	};

	FontManager(void);
	~FontManager(void);

	void Initialize(std::string filepath);
	void Shutdown();
	//draws from a string
	void Draw(unsigned int type,  const TCHAR* strOutput, int nX, int nY,
		float fScaleX=1.0f, float fScaleY=1.0f, DWORD dwColor= 0xFFFFFFFF) const;
	//draws from a resources in the string table
	void Draw(unsigned int type, int nResourceID,
			int nX, int nY, float fScaleX=1.0f, float fScaleY=1.0f, DWORD dwColor =0xFFFFFFFF) const;

	RECT CharRect( unsigned int type , int id ) const;

private:

	std::vector<CharSet*> fonts;
};

