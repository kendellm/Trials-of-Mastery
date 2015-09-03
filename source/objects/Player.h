#pragma once

#include "Human.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../ComboSystem/ComboManager.h"
#include "../CSGD/IListener.h"
#include "EntityState.h"


#include <vector>

class CAnimInfo;
class CEntityState;
class CAudioManager;
class IBaseInput;

class CPlayer : public CHuman, IListener
{
public:

	float		m_fDashTimer;
	enum ELEMENTS {FIRE, WIND, ICE, EARTH};
	
	CPlayer(int HP=100, int SP=100, int lives=3, int exp=0, int lvl=0, 
			 int fire=1, int wind=0, int ice=0, int earth=0,
			 bool bIsPlayerOne = true, bool bPvP = false );
	virtual ~CPlayer(void);

	virtual void Update            ( float fElapsedTime )	 override;
	virtual void Render            ( void )					 override;
	virtual void HandleActiveRectCollision( const IEntity* pOther )  override;
	virtual void ModifyHealth	   ( int damage)			 override;
	virtual void HandleEvent       ( CEvent* pEvent )		 override;
	virtual void HandleCollision   ( const IEntity* pOther, const RECT rOverlap )	override;
			void ModifyEXP		   ( int nExp);		
			void ModifySP		   ( int nSpl);


	// Accessors
	int		Level		    ( void ) const  {return m_nLevel		;}
	int		EXP 		    ( void ) const  {return m_nExpPts		;}
	int		Health		    ( void ) const  {return m_nHealth		;}
	int		Special		    ( void ) const  {return m_nSpecial		;}
	int		Lives		    ( void ) const  {return m_nLives		;}
	int		Fire		    ( void ) const  {return m_nFireSpl		;}
	int		Wind		    ( void ) const  {return m_nWindSpl		;}
	int		Ice		        ( void ) const  {return m_nIceSpl		;}
	int		Earth		    ( void ) const  {return m_nEarthSpl	    ;}
	int		Element         ( void ) const  {return m_nCurrElement  ;}
	int     MaxHP           ( void ) const  {return m_nMaxHealth    ;}
	int     MaxSP           ( void ) const  {return m_nMaxSpecial   ;}
	int     MaxLives	    ( void ) const  {return m_nMaxLives		;}
	float   GetCheckPointX	( void )	    {return m_fCheckPointX  ;}
	float   GetCheckPointY	( void )	    {return m_fCheckPointY  ;} 
	bool	WasSoundPlayed	( void )	    {return m_bSoundPlayed  ;}

	std::vector<int>&  GetButtonPresses ( void ) { return ButtonPresses; }


	// Mutators
	void    Level			   ( int nLevel		)	{  m_nLevel		      = nLevel 	 ;}          
	void    EXP 			   ( int nExpPts	)	{  m_nExpPts 	      = nExpPts	 ;}             	
	void    Health			   ( int nHealth	)	{  m_nHealth	      = nHealth	 ;}                 	
	void    Special			   ( int nSpecial	)	{  m_nSpecial	      = nSpecial ;}          	
	void    Lives			   ( int nLives		)	{  m_nLives		      = nLives 	 ;}         
	void    Fire			   ( int nFireSpl	)	{  m_nFireSpl	      = nFireSpl ;}          	
	void    Wind			   ( int nWindSpl 	)	{  m_nWindSpl	      = nWindSpl ;}          	
	void    Ice			       ( int nIceSpl	)	{  m_nIceSpl	      = nIceSpl	 ;}           	
	void    Earth			   ( int nEarthSpl	)	{  m_nEarthSpl	      = nEarthSpl;}  
	void	Element		       ( int nElement   )   {  m_nCurrElement     = nElement ;}
	void	MaxHP		       ( int nHp		)   {  m_nMaxHealth	      = nHp		 ;}
	void	MaxSP		       ( int nSp		)   {  m_nMaxSpecial      = nSp		 ;}
	void    MaxLives	       ( int n1UP       )   {  m_nMaxLives	      = n1UP	 ;}
	void    SetSuccessfulCombo ( bool value     )	{  m_bSuccessfulCombo = value    ;}
	void	SetComboFailed	   ( bool value     )   {  m_bFailedCombo	  = value    ;}
	void    SetCheckPointX     ( float x        )   {  m_fCheckPointX     = x        ;}
	void    SetCheckPointY     ( float y        )	{  m_fCheckPointY     = y        ;}
	void	SetSoundPlayed	   ( bool b			)	{  m_bSoundPlayed	  = b		 ;}
	
	void    SetCheckPoint      ( float x, float y);
	void	SetInputTimer      ( float nTimer )		{ m_fInputTimer	   = nTimer;	 }
	void    SetComboTimers     ( float fSuccessTimer = 0.0f, float fPrevInputTime = 0.0f,
	float	fInputTimer = 0.0f, float fFailTimer = 0.0f );

	//Entity State Mutators
	void			SetState		( CEntityState::ENTITY_STATE c	);
	CEntityState*	GetState		(		void		);





	std::vector<bool> GetPopUps(){return m_bPop_Ups;}
	void SetPopUps(std::vector<bool> b){m_bPop_Ups = b;}
	void SpecialAttack  (unsigned int nElement );
	void ReturnToCheckPoint();

private:
		
	std::vector<bool>   m_bPop_Ups;
	std::vector<int>    m_vElements;
	bool		win               ;
	bool		m_bIsPlayerOne	  ;
	bool		m_bPvP			  ;
								  
	int			m_nLevel		  ;
	int			m_nExpPts		  ;
	int			m_nHealth		  ;
	int			m_nMaxHealth      ;
	int			m_nSpecial		  ;
	int			m_nMaxSpecial     ;
	int			m_nLives		  ;
	int			m_nMaxLives       ;
	int			m_nFireSpl		  ;
	int			m_nWindSpl		  ;
	int			m_nIceSpl		  ;
	int			m_nEarthSpl		  ;
	int			m_nCurrElement    ;						  

	
	float		m_fCheckPointX	  ;
	float		m_fCheckPointY    ;
	float		m_nSpeed          ;
								  
	int			m_nCurrComboDepth ;

	// Combo Bools 
	bool		m_bFailedCombo;
	bool		m_bSuccessfulCombo;
	//------------------------------

	// Combo Timers
	float		m_fPrevInputTime;
	float		m_fInputTimer;		
	float		m_fSuccessTimer;
	float		m_fFailTimer;
	float		m_fRetreatTimer;
	//------------------------------

	// Input bools
	bool	m_bWasJoystickDown;
	bool	m_bIsJoystickDown;
	//------------------------------

	// Sound bools
	bool	m_bSoundPlayed;

	ComboManager*		m_pCM;
	CEntityState*		m_pEState;
	IBaseInput*			m_pCtrl;
	CAudioManager*		m_pAudio;

	std::vector<int> ButtonPresses;

	// Helper Functions
	void HandleResult(COMBO_RESULT cr, std::string anim);
	void HandleMovement();

	// Player One Input Functions
	void P1MovementInput(IBaseInput* pCtrl, float fElapsedTime);
	void P1ComboInput(IBaseInput* pCtrl);
	void P1SpecialInput(IBaseInput* pCtrl);

	// Player Two Input Functions
	void P2MovementInput(IBaseInput* pCtrl, float fElapsedTime);
	void P2ComboInput(IBaseInput* pCtrl);
	void P2SpecialInput(IBaseInput* pCtrl);

	void SwitchElement( int modElement );
	// Image ID's
	int			m_nKey1ImgID;
	int			m_nKey2ImgID;
	int			m_nKey3ImgID;
	int			m_nSuccessImgID;
	int			m_nFailImgID;
	int			m_nfireImg;
	int			m_nWindImg;
	int			m_nIceImg;
	int			m_nEarthImg;

};

