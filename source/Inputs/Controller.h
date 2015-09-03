/*************************************************************
	File:			Controller.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Serves as a facade between multiple input
					systems (keyboard, x360)  and game states. 
*************************************************************/
#pragma once

#include "BaseInput.h"

#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
class CSGD_DirectInput;


class CController : public IBaseInput
{
	enum INPUT {KEYBOAD, JOYSTICK, CONTROLLER };
	enum MODE {LOW, MEDIUM, HIGH};

public:

	static CController* GetInstance( void );

	// Cursor Controls
	virtual bool CursorMoveUp		( void );
	virtual bool CursorMoveDown		( void );
	virtual bool CursorMoveRight	( void );
	virtual bool CursorMoveLeft		( void );

	// Player Movement
	virtual bool MoveRight          ( void );
	virtual bool MoveLeft           ( void );
	virtual bool MoveUp	            ( void );
	virtual bool MoveDown           ( void );
	virtual bool MoveKeysPressed    ( void );
	virtual bool MoveKeysReleased   ( void );
	virtual bool MoveKeysDown		( void );
	virtual bool IsJoystickDown		( void );

	// Player Attacks
	virtual bool LightAtk           ( void );
	virtual bool MedAtk	            ( void );
	virtual bool HeavyAtk           ( void );

	virtual bool SpecAtk            ( void );
	virtual bool Fireball			( bool b );
	virtual bool SubZero            ( bool b );
	virtual bool SweepingWind       ( bool b );
	virtual bool ShockWave		    ( bool b );

	virtual bool Prev_Special		( void );
	virtual bool Next_Special		( void );


	// Menu Keys
	virtual bool Confirm            ( void );
	virtual bool Cancel             ( void );
	virtual bool Escape	            ( void );
	virtual bool Pause	            ( void );

	void Update	(float fElaspedTime);
	bool isConnected		( void );
	void Rumble(int left=0, int right=1);
	void Rumble(MODE setting);
	void SetXpad (bool b);

private:

	CController						( void );
	CController						( const CController& );
	virtual      ~CController	    ( void );
	CController& operator=			( const CController& );

	CSGD_DirectInput*	m_pDI;

	XINPUT_STATE		m_XpadState;
	int					m_nPlayer1;
	int					m_nPrimeInput;
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

