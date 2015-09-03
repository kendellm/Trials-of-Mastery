/*************************************************************
	File:			GameInfo.cpp
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Contains main gameplay attributes for player
*************************************************************/
#include "GameInfo.h"

#include "Game.h"
#include "CSGD/CSGD_Direct3D.h"
#include "CSGD/CSGD_TextureManager.h"
#include "Util/FontManager.h"
#include "CSGD/SGD_String.h"

GameInfo::GameInfo(void)
{
	m_nLevel		  = 0;
	m_nExpPts		  = 0;
	m_nHealth		  = 0;
	m_nSpecial		  = 0;
	m_nLives		  = 0;
	m_nFireSpl		  = 0;
	m_nWindSpl		  = 0;
	m_nIceSpl		  = 0;
	m_nEarthSpl		  = 0;
	m_nFullscreen     = 0;
	m_nLanguage       = 0;
	m_nBGM            = 0;
	m_nSFX            = 0;

	m_nMenuArtid	  = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/profile.png"));

}

GameInfo::GameInfo( int level, int Exp, int health, int special,
int lives, int fire, int wind, int ice, int earth, int fullscreen,
int language, int Bgm, int Sfx )
{
	m_nLevel		  = level       ;
	m_nExpPts		  = Exp			;
	m_nHealth		  = health		;
	m_nSpecial		  = special		;
	m_nLives		  = lives		;
	m_nFireSpl		  = fire		;
	m_nWindSpl		  = wind		;
	m_nIceSpl		  = ice			;
	m_nEarthSpl		  = earth		;
	m_nFullscreen     = fullscreen	;
	m_nLanguage       = language	;
	m_nBGM            = Bgm			;
	m_nSFX            = Sfx			;

	m_nMenuArtid	  = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/profile.png"));
}


GameInfo::~GameInfo(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);
}

void GameInfo::Display( int x, int y )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pTM->Draw( m_nMenuArtid, x, y);

	TOSTRINGSTREAM wss;	

	wss.str(L"");
	wss << "Stage 0" << m_nLevel;
	pFont->Draw( CHINESE_TAKEAWAY , wss.str().c_str(), x + 43, y+36, 0.5f,0.5f);

	wss.str(L"");
	wss << "EXP: " << m_nExpPts;
	pFont->Draw( CHINESE_TAKEAWAY , wss.str().c_str(), x + 201, y+36, 0.5f,0.5f);


	wss.str(L"");
	wss << m_nFireSpl;
	pFont->Draw( CHINESE_TAKEAWAY ,  wss.str().c_str(), x + 83, y + 78, 0.5f,0.5f);

	wss.str(L"");
	wss << m_nWindSpl;
	pFont->Draw( CHINESE_TAKEAWAY ,  wss.str().c_str(), x + 160, y + 78, 0.5f,0.5f);

	wss.str(L"");
	wss << m_nIceSpl;
	pFont->Draw( CHINESE_TAKEAWAY ,  wss.str().c_str(), x + 237, y + 78, 0.5f,0.5f);

	wss.str(L"");
	wss << m_nEarthSpl;
	pFont->Draw( CHINESE_TAKEAWAY ,  wss.str().c_str(), x + 304, y + 78,0.5f,0.5f);

}

void GameInfo::Upgrade( unsigned int nType )
{
	switch(nType)
	{
	case HEALTH:  m_nHealth   = min(m_nHealth + 1, 3);
		break;				  
	case SPECIAL: m_nSpecial  = min(m_nSpecial + 1, 3);
		break;
	case LIVES:   m_nLives    = min(m_nLives + 1, 3);
		break;
	case FIRE:	  m_nFireSpl  = min(m_nFireSpl + 1, 3);
		break;				  
	case WIND:    m_nWindSpl  = min(m_nWindSpl + 1, 3);
		break;				  
	case ICE:     m_nIceSpl   = min(m_nIceSpl + 1, 3);
		break;
	case EARTH:   m_nEarthSpl = min(m_nEarthSpl + 1, 3);
		break;
	}
}

bool GameInfo::CheckGrade( unsigned int nType, int Lvl )
{
	switch(nType)
	{
	case HEALTH:  return ( ( m_nHealth   < Lvl) ? true : false);
		break;	  
	case SPECIAL: return ( ( m_nSpecial  < Lvl) ? true : false);
		 break;	 
	case LIVES:   return ( ( m_nLives    < Lvl) ? true : false);
		break;	 
	case FIRE:	  return ( ( m_nFireSpl  < Lvl) ? true : false);
		 break;	 			  			
	case WIND:    return ( ( m_nWindSpl  < Lvl) ? true : false);
		 break;	 			  			
	case ICE:     return ( ( m_nIceSpl   < Lvl) ? true : false);
		break;	  						
	case EARTH:   return ( ( m_nEarthSpl < Lvl) ? true : false);
		 break;
	default: return false;
		break;
	}
}


