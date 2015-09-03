/*************************************************************
	File:			Controller.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Serves as a facade between multiple input
					systems (keyboard, x360)  and game states. 
*************************************************************/
#pragma once


#include "BaseInput.h"

class CSGD_DirectInput;


class CPlayer2Controller : public IBaseInput
{	
public:

	static CPlayer2Controller* GetInstance( void );

	// Cursor Controls
	bool CursorMoveUp		( void );
	bool CursorMoveDown		( void );
	bool CursorMoveRight	( void );
	bool CursorMoveLeft		( void );

	// Player Movement
	bool MoveRight          ( void );
	bool MoveLeft           ( void );
	bool MoveUp	            ( void );
	bool MoveDown           ( void );
	bool MoveKeysPressed    ( void );
	bool MoveKeysReleased   ( void );
	bool MoveKeysDown		( void );
	bool IsJoystickDown		( void );

	// Player Attacks
	bool LightAtk           ( void );
	bool MedAtk	            ( void );
	bool HeavyAtk           ( void );

	bool SpecAtk            ( void );
	bool Fireball			( bool b );
	bool SubZero            ( bool b );
	bool SweepingWind       ( bool b );
	bool ShockWave		    ( bool b );

	bool Prev_Special		( void );
	bool Next_Special		( void );

	void Update	(float fElaspedTime);

private:

				CPlayer2Controller			( void );
				CPlayer2Controller			( const CPlayer2Controller& );
	virtual		~CPlayer2Controller			( void );
	CPlayer2Controller& operator=			( const CPlayer2Controller& );

	CSGD_DirectInput* m_pDI;

    int      Light_Atk	;
    int      Med_Atk	;
    int      Heavy_Atk	;
    int      Spec_Atk	;
    int		 next_spec	;
	int      prev_spec  ;
	int		 confirm    ;
	int      cancel     ;
	float    m_fTimer   ;

};

