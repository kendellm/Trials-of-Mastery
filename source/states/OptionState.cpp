#include "OptionState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../Game.h"
#include "StateManager.h"
#include "../states/GameplayState.h"
#include "../states/MainMenuState.h"
#include "../GameInfo.h"
#include "../../resource.h"
#include <sstream>


COptionState* COptionState::GetInstance( void )
{
	static COptionState s_Instance;
	return &s_Instance;
}


COptionState::COptionState(void)
{

	m_nCursorX		= -1;
	m_nCursorY		= -1;

	m_nCurrPage		= -1;
	m_nSubPage		= -1;


}


COptionState::~COptionState(void)
{
}

void COptionState::Enter( void )
{
	Active(true);
	
	m_pGi = CGame::GetInstance()->GetProfile();
	
	m_nSelect = 0;
	m_nCurrPage = 0;
	m_nSubPage = 0;

	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_options.png"));


	CSGD_XAudio2::GetInstance()->MusicSetMasterVolume(m_pGi->BGM()/ 100.0f );
	CSGD_XAudio2::GetInstance()->SFXSetMasterVolume(  m_pGi->SFX() / 100.0f );
}

void COptionState::Exit( void )
{

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);


	m_nSelect = 0;
	m_nCurrPage = 0;
	m_nSubPage = 0;
	m_pGi = nullptr;

	Active(false);
}

bool COptionState::Input( void )
{
	CController*	 pCtrl =  CController::GetInstance();
	CAudioManager*	 pAUM  =  CAudioManager::GetInstance();


	if( pCtrl->Cancel() )
	{
		if(m_nCurrPage > 0)
			m_nCurrPage = 0;
		else
		{
			CGame::GetInstance()->GetState()->RemoveState(this);
			return true;
		}
		pAUM->Play(SFX::CURSOR);
	}

	switch(m_nCurrPage)
	{
	case PAGE::MAIN:
		{	
			if ( pCtrl->CursorMoveUp())
			{
				if(m_nSelect >0)
					m_nSelect -=1;
				pAUM->Play(SFX::CURSOR);

			}

			if (pCtrl->CursorMoveDown() )
			{
				if(m_nSelect <3)
					m_nSelect +=1;
				pAUM->Play(SFX::CURSOR);

			}

			if (pCtrl->Confirm() )
			{
				switch(m_nSelect)
				{
				case CURSOR::WINDOW:	
					{
						m_nCurrPage = PAGE::FULLSCREEN;

						if(CGame::GetInstance()->Fullscreen() == true)
							m_nSelect = 1;
						else
							m_nSelect = 0;
					}
					break;
				case CURSOR::LANG:      m_nCurrPage = PAGE::LANGUAGE;
					break;
				case CURSOR::VOLUME:
					{
						m_nCurrPage = PAGE::SOUND;
						m_nSelect = 0;
					}
					break;

				case CURSOR::EXIT:
					{
						CGame::GetInstance()->GetState()->RemoveState(this);
						return true;
					}
					break;
				}
				pAUM->Play(SFX::CURSOR);
			}
		}
		break;
	case PAGE::FULLSCREEN:
		{

			if ( pCtrl->CursorMoveUp())
			{
				if(m_nSelect >0)
					m_nSelect -=1;	

				pAUM->Play(SFX::CURSOR);
			}

			if (pCtrl->CursorMoveDown() )
			{
				if(m_nSelect <2)
					m_nSelect +=1;

				pAUM->Play(SFX::CURSOR);
			}

			if (pCtrl->Confirm() )
			{
				pAUM->Play(SFX::CURSOR);

				if(m_nSelect < 2)
				{

					m_pGi->Fullscreen(m_nSelect);
					CGame::GetInstance()->Fullscreen(m_pGi->Fullscreen() != 0);
					m_nSelect = 2;
				}
				else if(m_nSelect == 2)
					m_nCurrPage = PAGE::MAIN;	

			}
		}
		break;
	case PAGE::LANGUAGE:
		{
			if ( pCtrl->CursorMoveUp())
			{
				if(m_nSelect >0)
					m_nSelect -=1;	

				pAUM->Play(SFX::CURSOR);
			}

			if (pCtrl->CursorMoveDown() )
			{
				if(m_nSelect <2)
					m_nSelect +=1;

				pAUM->Play(SFX::CURSOR);
			}

			if (pCtrl->Confirm() )
			{
				if(m_nSelect == 0)
				{
					m_pGi->Language(LANG_NEUTRAL);
				}
				else if(m_nSelect == 1)
				{
					m_pGi->Language(LANG_SPANISH);
				}
				else if(m_nSelect == 2)
					m_nCurrPage = PAGE::MAIN;	

				pAUM->Play(SFX::CURSOR);
			}
		}
		break;
	case PAGE::SOUND:
		{
			if ( pCtrl->CursorMoveUp())
			{
				if(m_nSelect >0)
					m_nSelect -=1;	

				pAUM->Play(SFX::CURSOR);
			}

			if (pCtrl->CursorMoveDown() )
			{
				if(m_nSelect <2)
					m_nSelect +=1;

				pAUM->Play(SFX::CURSOR);
			}

			if( pCtrl->CursorMoveLeft())
			{
				if( m_nSelect == 0)
				{
					if(m_pGi->BGM() > 0)
						m_pGi->BGM(m_pGi->BGM() -10 );
					CSGD_XAudio2::GetInstance()->MusicSetMasterVolume(m_pGi->BGM()/ 100.0f );
					//pAUM->Play(SFX::CURSOR);
				}
				else if (m_nSelect == 1)
				{
					if(m_pGi->SFX() >0)
						m_pGi->SFX( m_pGi->SFX() - 10);
					CSGD_XAudio2::GetInstance()->SFXSetMasterVolume( m_pGi->SFX() / 100.0f );
					pAUM->Play(SFX::CURSOR);
				}
			}

			if( pCtrl->CursorMoveRight())
			{
				if(m_nSelect == 0)
				{
					if(m_pGi->BGM() <100)
						m_pGi->BGM(m_pGi->BGM() +10 );
					CSGD_XAudio2::GetInstance()->MusicSetMasterVolume(m_pGi->BGM()/ 100.0f );
					//pAUM->Play(SFX::CURSOR);
				}
				else if ( m_nSelect == 1)
				{
					if(m_pGi->SFX() < 100)
						m_pGi->SFX( m_pGi->SFX() + 10);
					CSGD_XAudio2::GetInstance()->SFXSetMasterVolume(m_pGi->SFX() / 100.0f);
					pAUM->Play(SFX::CURSOR);
				}
			}

			if (pCtrl->Confirm() )
			{
					m_nCurrPage = PAGE::MAIN;	

				pAUM->Play(SFX::CURSOR);
			}

		
		}
			break;
	}

	return true;

}
void COptionState::Update( float fElaspedTime )
{
	FontManager* pFont = CGame::GetInstance()->GetFont();

	switch(m_nCurrPage)
	{
	case PAGE::MAIN:
		{		
			switch(m_nSelect)
			{
			case CURSOR::WINDOW :
				{
					m_nCursorX = 235;
					m_nCursorY = 190;
				}
				break;
			case CURSOR::LANG:
				{
					m_nCursorX = 235;
					m_nCursorY = 216;
				}
				break;
			case CURSOR::VOLUME:
				{
					m_nCursorX = 235;
					m_nCursorY = 239;
				}
				break;
			case EXIT:
				{
					m_nCursorX = 235;
					m_nCursorY = 290;
				}
				break;
			}
		}
		break;
	case PAGE::FULLSCREEN:
		{
			
			if(m_nSelect == 0)
			{
				m_nCursorX = 235;
				m_nCursorY = 218;
			}
			else if(m_nSelect == 1)
			{
				m_nCursorX = 235;
				m_nCursorY = 241;
			}
			else
			{
				m_nCursorX = 235;
				m_nCursorY = 296;
			}

		}
		break;
	case PAGE::LANGUAGE:
		{
			if(m_nSelect == 0)
			{
				m_nCursorX = 235;
				m_nCursorY = 218;
			}
			else if(m_nSelect == 1)
			{
				m_nCursorX = 235;
				m_nCursorY = 241;
			}
			else
			{
				m_nCursorX = 235;
				m_nCursorY = 296;
			}
		}
		break;
	case PAGE::SOUND:
		{
			if(m_nSelect == 0)
			{
				m_nCursorX = 235;
				m_nCursorY = 218;
			}
			else if(m_nSelect == 1)
			{
				m_nCursorX = 235;
				m_nCursorY = 241;
			}
			else
			{
				m_nCursorX = 235;
				m_nCursorY = 296;
			}

		}
		break;
	}//end options switch

}

void COptionState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	RECT rBackground = { 206, 128, 637, 425};
	RECT rBar		 = {  12, 671, 198, 698};
	RECT rNode		 = { 144, 630, 152, 651};
	RECT rBox		 = {  74, 619, 127, 670};
	RECT rXbox		 = {  15, 620,  67, 660};
	RECT rTitle		 = { 295,  63, 510, 126};
	RECT rCursor     = { 523, 640, 544, 660};

	pTM->Draw( m_nMenuArtid, 184, 82, 1.0f, 1.0f, &rBackground );

	switch(m_nCurrPage)
	{
	case PAGE::MAIN:
		{	

			pTM->Draw(m_nMenuArtid,293, 62, 1.0f, 1.0f, &rTitle );

			pFont->Draw( ARIAL , OPS_STATE_MENU,
				257, 187,0.8f,0.8f, BROWN);
		}
		break;
	case PAGE::FULLSCREEN:
		{
			pFont->Draw( ARIAL , OPS_STATE_FSC  ,257, 187,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , STATE_YES		,257, 216,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , STATE_NO		,257, 236,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , STATE_BACK		,257, 290,	0.8f,	0.8f,  BROWN);
		}
		break;
	case PAGE::LANGUAGE:
		{
			pFont->Draw( ARIAL , OPS_STATE_LANG ,257, 187,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , OPS_STATE_ENG	,257, 216,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , OPS_STATE_SPAN	,257, 236,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , STATE_BACK		,257, 290,	0.8f,	0.8f,  BROWN);


		}
		break;
	case PAGE::SOUND:
		{
			std::wostringstream wss;
			wss.str(L"");
			
			pFont->Draw( ARIAL , OPS_STATE_VOL ,257, 187,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , OPS_STATE_BGM ,257, 216,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , OPS_STATE_SFX ,257, 237,	0.8f,	0.8f,  BROWN);
			pFont->Draw( ARIAL , STATE_BACK		,257, 290,	0.8f,	0.8f,  BROWN);
		
			for(int i = 0; i < (m_pGi->BGM()/10)*3.5f; ++i)
			{
				pTM->Draw( m_nMenuArtid, 317 + (i*5),216, 1.0f, 1.0f, &rNode );
			}

			for(int i = 0; i < (m_pGi->SFX()/10)*3.5f; ++i)
			{
				pTM->Draw( m_nMenuArtid, 317 + (i*5),237, 1.0f, 1.0f, &rNode );
			}

			pTM->Draw(m_nMenuArtid,310, 214, 1.0f, 1.0f, &rBar );
			pTM->Draw(m_nMenuArtid,310, 235, 1.0f, 1.0f, &rBar );

		}
		break;
	}//end options switch


		pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY,
		  1.0f, 1.0f, &rCursor);

	
}


void COptionState::Import( GameInfo* pInfo )
{
	m_pGi = pInfo;

	CGame::GetInstance()->Fullscreen(m_pGi->Fullscreen() != 0);
	CSGD_XAudio2::GetInstance()->MusicSetMasterVolume(m_pGi->BGM()/ 100.0f );
	CSGD_XAudio2::GetInstance()->SFXSetMasterVolume(  m_pGi->SFX() / 100.0f );

}

