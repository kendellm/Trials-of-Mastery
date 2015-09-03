#pragma once

#include "CSGD/IListener.h"
class CEntity;
class GameInfo;
class CHUD : public IListener
{
public:

	CHUD(void);
	~CHUD(void);

	void Update   ( CEntity* pEntity);
	void Update   (float fElaspedTime);
	void Render   ( void );
	void Import   ( GameInfo* pInfo );
	void ShowEXP  ( int nExp, float nTime);
	void SetHPBar ( int hp );

private:

	int			m_nExpPts		;
	float		m_nExpTimer     ;
	int			m_nCurrExp      ;
	int			m_nHealth		;
	int			m_nMaxHealth    ;
	int			m_nSpecial		;
	int			m_nMaxSpecial   ;
	int			m_nLives		;
	int			m_nCurrElement  ;

	int		    m_nHPBarImgid	;
	int		    m_nHPnodeImgid	;
	int		    m_nSPnodeImgid	;
	int		    m_nExpImgid		;
	int         m_nLivesImgid   ;

	int			m_nElement[4]   ;


	void HandleEvent( CEvent* pEvent );

};

