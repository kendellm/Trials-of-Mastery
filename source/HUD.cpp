#include "HUD.h"

#include "CSGD/CSGD_TextureManager.h"
#include "CSGD/CSGD_Direct3D.h"
#include "Util/FontManager.h"
#include "CSGD/SGD_String.h"
#include "Game.h"
#include "GameInfo.h"
#include "states/GameplayState.h"
#include "objects/Player.h"

//All HUD elements are offset from this position
#define XPOS 120 
#define YPOS 60

CHUD::CHUD(void)
{
	m_nHealth         = 0;
	m_nSpecial        = 0; 
	m_nExpPts         = 0; 
	m_nCurrExp		  = 0;
	m_nExpTimer		  = 0;
	m_nCurrElement    = 0; 
	m_nMaxHealth	  = 0;
	m_nMaxSpecial     = 0;


	m_nHPnodeImgid  = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/HUD/HP1_node.png"));

	 m_nSPnodeImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/HUD/SP_node.png"));

	 m_nExpImgid   = CSGD_TextureManager::GetInstance()->LoadTexture(
		 _T("resource/graphics/HUD/plus.png"));

	 m_nLivesImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		  _T("resource/graphics/HUD/lives_icon.png"));

	 m_nElement[0]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		 _T("resource/graphics/HUD/fire_hud.png"));
	 m_nElement[1]         = CSGD_TextureManager::GetInstance()->LoadTexture(
		 _T("resource/graphics/HUD/wind_hud.png"));
	 m_nElement[2]          = CSGD_TextureManager::GetInstance()->LoadTexture(
		 _T("resource/graphics/HUD/ice_hud.png"));
	 m_nElement[3]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		 _T("resource/graphics/HUD/earth_hud.png"));
	
	 CPlayer* pPlayer = dynamic_cast< CPlayer* >(
		 CGameplayState::GetInstance()->GetPlayer() );
	 SetHPBar(pPlayer->MaxHP());
}


CHUD::~CHUD(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nExpImgid);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nSPnodeImgid);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHPnodeImgid);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHPBarImgid);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nElement[3]);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nElement[2]);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nElement[1]);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nElement[0]);
}

void CHUD::Update( float fElaspedTime )
{
	CPlayer* pPlayer = dynamic_cast< CPlayer* >(
		CGameplayState::GetInstance()->GetPlayer() );
	
	m_nHealth		= pPlayer->Health();
	m_nSpecial		= pPlayer->Special();
	m_nCurrElement  = pPlayer->Element();
	m_nCurrExp		= pPlayer->EXP();
	m_nLives	    = pPlayer->Lives();
	m_nMaxHealth	= pPlayer->MaxHP();
	m_nMaxSpecial   = pPlayer->MaxSP();

	if(m_nExpTimer)
	{
		m_nExpTimer = max( m_nExpTimer - fElaspedTime, 0);

		if(m_nExpTimer == 0)
			m_nExpPts = 0;
	}
}


void CHUD::Render()
{
	CSGD_Direct3D* pD3d = CSGD_Direct3D::GetInstance();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont(); 
	
	int HPCount = int((m_nHealth  / (float)m_nMaxHealth  )*100);
	int SPCount = m_nSpecial / 10;
	RECT rHP = {XPOS+3, YPOS + 4, rHP.left + m_nHealth+2,  YPOS + 21};
	RECT rSP = {0,0,32,25};

	pTM->Draw(m_nHPBarImgid,
		XPOS, YPOS, 1.0f, 1.0f);

	pD3d->DrawRect(rHP, D3DCOLOR_ARGB(255,0,150,0));

	for(int i = 0; i < SPCount; ++i)
	{
		pTM->Draw(m_nSPnodeImgid,
			XPOS + (18 * i), YPOS + 25, 0.7f, 0.7f, &rSP);
	}
	
	pTM->Draw(m_nElement[m_nCurrElement],
		XPOS-64, YPOS-21, 1.0f, 1.0f);

	for(int i = 0; i < m_nLives; ++i)
	{
		pTM->Draw(m_nLivesImgid, XPOS + (30 * i), YPOS-25);
	}
	

	if(m_nExpTimer)
	{
		RECT rPlus = {0, 0, 27, 27};
		pTM->Draw(m_nExpImgid, 660, 280 +((int)m_nExpTimer * 10), 1.0f,1.0f,&rPlus );

		TOSTRINGSTREAM wss;
		wss.str(L"");
		wss << m_nExpPts;
		pFont->Draw( ARNOVA_PURPLE, wss.str().c_str(), 695  , 260 +((int)m_nExpTimer * 10)  );

		if(m_nExpTimer <= 1)
		{
			wss.str(L"");
			wss << "0"<< m_nCurrExp;
			pFont->Draw( ARIAL, wss.str().c_str(), 695  , 300 +((int)m_nExpTimer * 10) ,
				1.0f, 1.0f, MAGENTA);
		}
	}
}

void CHUD::Import( GameInfo* pInfo ) 
{

}

void CHUD::ShowEXP( int nExp, float nTime)
{
	m_nExpPts = nExp;
	m_nExpTimer = nTime;
}

void CHUD::HandleEvent( CEvent* pEvent )
{

}

void CHUD::SetHPBar( int hp )
{
	switch(hp)
	{
	case 100:
		m_nHPBarImgid   = CSGD_TextureManager::GetInstance()->LoadTexture(
			_T("resource/graphics/HUD/HP_bar1.png") );
		break;
	case 200: 
		m_nHPBarImgid   = CSGD_TextureManager::GetInstance()->LoadTexture(
			_T("resource/graphics/HUD/HP_bar2.png") );
		break;
	case 300: 
		m_nHPBarImgid   = CSGD_TextureManager::GetInstance()->LoadTexture(
			_T("resource/graphics/HUD/HP_bar3.png") );
		break;
	default:
		m_nHPBarImgid   = CSGD_TextureManager::GetInstance()->LoadTexture(
			_T("resource/graphics/HUD/HP_bar1.png") );
		break;
	}




	
}


