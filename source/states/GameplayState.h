/*************************************************************
	File:			GameplayState.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		handles all the game play			
*************************************************************/
#pragma once

#include "IGamestate.h"

#include <vector>

#include "../CSGD/IListener.h"

class CEntity;

class CSGD_MessageSystem;
class CSGD_EventSystem;
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class TileManager;
class CEntityManager;
class AnimationManager;
class FontManager;
class GameInfo;
class CEnemy;
class IMessage;
class CHUD;
class CCamera;
class CSpecial;


class CGameplayState :	public IGameState, public IListener
{
public:


	static CGameplayState* GetInstance( void );
	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
				


	static  void MessageProc( IMessage* pMsg );
		    void Level	(int nLevel)   {m_nLevel = nLevel;}
		
			void HandleEvent( CEvent* pEvent );

			CEntity* GetPlayer () const {return m_pPlayer1;}


private:

	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CSGD_MessageSystem*		m_pMS;
	CSGD_EventSystem*		m_pES;
	FontManager*			m_pFont;
	CEntityManager*			m_pEM;
	TileManager*			m_pTileM;
	CCamera*				m_pCamera;
	CPopUpManager*			m_pPUM;

	CGameplayState						( void );
	CGameplayState						( const CGameplayState& );
	virtual         ~CGameplayState		( void );
	CGameplayState& operator=			( const CGameplayState& );
	int			m_nBackgroundImgID;
	int			m_nLevel;
	bool		m_bWin;
	bool        m_bFirstTime;
	bool		m_bGameOver;
	CEntity*	m_pPlayer1;
	CEntity*	m_pPlayer2;
	CHUD*		m_pHUD;	
	CEntity*  CreatePlayer		( GameInfo* pInfo, float x, float y, bool bIsPlayerOne = true, bool bPvp = false);
	void	  CreateLevel (int nlevel);
	//CSpecial* CreateSpecialAtk (float x, float y, int lvl, int type);

	void RenderBG();

	std::vector<std::vector<int>> ScreenLockWaves;
	int wave_Count_Down;
	int current_Wave;
	void HandleScreenLock();
	void Reset_Screen_Lock();
	float LockY;
};
