/*
	File:		Keyboard.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CKeyboard handles the players input
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

class CSGD_DirectInput; 

class CKeyboard
{
protected:

		enum MODE {LOW, MEDIUM, HIGH};

public:

	static CKeyboard* GetInstance( void );

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

	// Player Attacks
	virtual bool LightAtk           ( void );
	virtual bool MedAtk	            ( void );
	virtual bool HeavyAtk           ( void );

	virtual bool SpecAtk            ( void );
	virtual bool Fireball			( void );
	virtual bool SubZero            ( void );
	virtual bool SweepingWind       ( void );
	virtual bool ShockWave		    ( void );

	// Menu Keys
	virtual bool Confirm            ( void );
	virtual bool Cancel             ( void );
	virtual bool Escape	            ( void );
	virtual bool Pause	            ( void );

	virtual void Update	(float fElaspedTime);

private:

	CKeyboard						( void );
	CKeyboard						( const CKeyboard& );
	virtual      ~CKeyboard			( void );
	CKeyboard& operator=			( const CKeyboard& );

	CSGD_DirectInput* m_pDI;

	int					m_nPlayer1;

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

#endif
