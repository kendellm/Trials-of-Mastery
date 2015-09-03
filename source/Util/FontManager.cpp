/*************************************************************
	File:			FontManager.cpp
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		convert string characters into bitmapped images
*************************************************************/
#include "FontManager.h"

#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../TinyXML/tinyxml.h"
#include "../Game.h"
#include <atlstr.h>
#include <sstream>
#include <cassert>
#include "../GameInfo.h"

FontManager::FontManager(void)
{

}


FontManager::~FontManager(void)
{

}

void FontManager::Initialize(std::string filepath)
{
	TiXmlDocument x_doc;
	x_doc.LoadFile( filepath.c_str());
	//assert(x_doc.LoadFile( filepath.c_str()) != false && " Font XML file could not be loaded");
	
	TiXmlElement* x_pRoot = x_doc.RootElement();

	//assert( x_pRoot != nullptr && "XML root is NULL");

	CharSet* pFont;
	int count = 0;

	TiXmlElement* x_pFont = x_pRoot->FirstChildElement("fonts");
	x_pFont->Attribute("count", &count);

	for(int i = 0; i < count; i++)
	{
		x_pFont = x_pFont->NextSiblingElement("font");
		pFont = new CharSet;
		int id, x,y,w,h,xOff,yOff,Xadv; 
		std::string file;
		pFont->szLabel = x_pFont->Attribute("label");
		file = x_pFont->Attribute("image");

		x_pFont->Attribute("space", &pFont->m_nSpace);
		x_pFont->Attribute("newLine", &pFont->m_nNewLine);

		std::wostringstream wss;
		wss.str(L"");
		wss << file.c_str();
		pFont->m_nFontImg = CSGD_TextureManager::GetInstance()->LoadTexture(wss.str().c_str());
		assert( pFont->m_nFontImg != -1 && "Font Bitmap not set");

		TiXmlElement* x_pChars = x_pFont->FirstChildElement("chars");
		TiXmlElement* x_pChar = x_pChars->FirstChildElement("char");

		while( x_pChar != nullptr)
		{
			x_pChar->Attribute("id",&id);
			x_pChar->Attribute("x",&x );
			x_pChar->Attribute("y",&y );
			x_pChar->Attribute("width",&w );
			x_pChar->Attribute("height",&h );
			x_pChar->Attribute("xoffset",&xOff );
			x_pChar->Attribute("yoffset",&yOff );
			x_pChar->Attribute("xadvance",&Xadv );

			pFont->Char[id].X(x);
			pFont->Char[id].Y(y);
			pFont->Char[id].Width(w);
			pFont->Char[id].Height(h);
			pFont->Char[id].XOffset(xOff);
			pFont->Char[id].YOffset(yOff);
			pFont->Char[id].XAdvance(Xadv);

			x_pChar = x_pChar->NextSiblingElement("char");

		}		

		fonts.push_back(pFont);
	}

}

void FontManager::Shutdown()
{
	for(unsigned int i = 0; i < fonts.size(); i++)
	{	
		delete fonts[i];
	}	
}

void FontManager::Draw(unsigned int type,  const TCHAR* strOutput, int nX, int nY,
					   float fScaleX, float fScaleY, DWORD dwColor) const
{
	//determines where on screen to draw the text
	RECT drawRect;
	drawRect.left = nX;
	drawRect.top  = nY;
		
	//loops through all the characters in the string
	for( int i = 0; strOutput[ i ]; i++ )
	{
		//store character index
		TCHAR ch = strOutput[ i ];

		if( ch == _T(' ') )
		{
			drawRect.left = drawRect.left + fonts[type]->m_nSpace;	
			continue;
		}
		else if( ch == _T('\n') )
		{
			drawRect.top  = LONG(drawRect.top + (fonts[type]->m_nNewLine * fScaleY));
			drawRect.left = nX;
			continue;
		}
		
		//gets the texture rect for the character in bitmap
		RECT imageRect = CharRect(type, (int)ch);

		drawRect.right  = drawRect.left + fonts[type]->Char[int(ch)].Width();
		drawRect.bottom = drawRect.top + fonts[type]->Char[int(ch)].Height();

		CSGD_TextureManager::GetInstance()->Draw(
				fonts[type]->m_nFontImg, drawRect.left + fonts[type]->Char[int(ch)].XOffset() ,
				drawRect.top + fonts[type]->Char[int(ch)].YOffset(),
				fScaleX, fScaleY, &imageRect,
				0.0f,0.0f,0.0f, dwColor );

		drawRect.left = (LONG)(drawRect.left + (fonts[type]->Char[int(ch)].XAdvance() * fScaleX));
	}	
}

void FontManager::Draw( unsigned int type, int nResourceID, int nX, int nY, float fScaleX,
								float fScaleY, DWORD dwColor ) const
{
	GameInfo* pInfo = CGame::GetInstance()->GetProfile();
	CString strOutput;
	strOutput.LoadString( CGame::GetInstance()->HInstance() , nResourceID, pInfo->Language() );

	Draw(  type,  strOutput,  nX,  nY, fScaleX,  fScaleY,  dwColor);
}

RECT FontManager::CharRect( unsigned int type , int id ) const
{
	RECT rSrc;
	rSrc.left   = fonts[type]->Char[id].X();
	rSrc.top    = fonts[type]->Char[id].Y();
	rSrc.right  = rSrc.left + fonts[type]->Char[id].Width();
	rSrc.bottom = rSrc.top + fonts[type]->Char[id].Height();
	
	return rSrc;
}
