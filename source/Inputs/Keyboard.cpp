/*
File:		Keyboard.cpp
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CKeyboard handles the players input
*/

#include "Keyboard.h"


#include "../CSGD/CSGD_DirectInput.h"

#define DPAD_UP     2
#define DPAD_DOWN   3
#define DPAD_LEFT   0
#define DPAD_RIGHT  1
#define XPAD_RTRIG  -1012
#define XPAD_LTRIG  1019
#define XPAD_BACK   6
#define XPAD_START  7
#define XPAD_LSTICK 8
#define XPAD_RSTICK 9



CKeyboard* CKeyboard::GetInstance( void )
{
	static CKeyboard s_Instance;
	return &s_Instance;
}




CKeyboard::CKeyboard(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_nPlayer1 = 0;

	Light_Atk   = 0;
	Med_Atk	    = 0;
	Heavy_Atk   = 0;
	Spec_Atk    = 0;
	prev_spec   = 0;
	next_spec   = 0;
	confirm     = 0;
	cancel      = 0;
	m_fTimer	= 0;
}



CKeyboard::~CKeyboard(void)
{

}

void CKeyboard::Update( float fElaspedTime ) 
{
	m_fTimer = max(m_fTimer - fElaspedTime, 0);
}			 

bool CKeyboard::CursorMoveUp()
{
	if(
		m_pDI->KeyPressed(DIK_W) == true	||
		m_pDI->KeyPressed(DIK_UP) == true			
		)
		return true;
	else
		return false;
}

bool CKeyboard::CursorMoveDown()
{

	if( 
		m_pDI->KeyPressed(DIK_S) == true	||
		m_pDI->KeyPressed(DIK_DOWN) == true			 
		)
		return true;
	else
		return false;
}

bool CKeyboard::CursorMoveRight()
{

	if( 
		m_pDI->KeyPressed(DIK_D) == true	||
		m_pDI->KeyPressed(DIK_RIGHT) == true			  
		)
		return true;
	else
		return false;
}

bool CKeyboard::CursorMoveLeft()
{
	if( 
		m_pDI->KeyPressed(DIK_A) == true	||
		m_pDI->KeyPressed(DIK_LEFT) == true			  
		)
		return true;
	else
		return false;
}

bool CKeyboard::MoveRight()
{

	if( 
		m_pDI->KeyDown(DIK_D) == true				||
		m_pDI->KeyDown(DIK_RIGHT) == true	
		)
		return true;
	else
		return false;
}

bool CKeyboard::MoveLeft()
{

	if( 
		m_pDI->KeyDown(DIK_A) == true				||
		m_pDI->KeyDown(DIK_LEFT) == true		  
		)
		return true;
	else
		return false;
}

bool CKeyboard::MoveUp()
{

	if(
		m_pDI->KeyDown(DIK_W) == true				 ||
		m_pDI->KeyDown(DIK_UP) == true	
		)
		return true;
	else
		return false;
}

bool CKeyboard::MoveDown()
{
	if( 
		m_pDI->KeyDown(DIK_S) == true		    	||
		m_pDI->KeyDown(DIK_DOWN) == true   
		)
		return true;
	else
		return false;
}

bool CKeyboard::LightAtk()
{

	if(m_pDI->KeyPressed(DIK_NUMPAD1) == true)
		return true;
	else
		return false;
}

bool CKeyboard::MedAtk()
{

	if(m_pDI->KeyPressed(DIK_NUMPAD2) == true)
		return true;
	else
		return false;
}

bool CKeyboard::HeavyAtk()
{
	if(m_pDI->KeyPressed(DIK_NUMPAD3) == true)
		return true;
	else
		return false;
}


bool CKeyboard::Confirm()
{
	if(m_pDI->KeyPressed(DIK_RETURN) == true)
		return true;
	else
		return false;
}

bool CKeyboard::Cancel( void )
{
	if(
		m_pDI->KeyPressed(DIK_ESCAPE) == true     ||
		m_pDI->KeyPressed(DIK_BACKSPACE) == true  
		)
		return true;
	else
		return false;
}

bool CKeyboard::Escape( void )
{
	if(m_pDI->KeyPressed(DIK_ESCAPE) == true)
		return true;
	else
		return false;
}

bool CKeyboard::Pause( void )
{


	if( m_pDI->KeyPressed( DIK_ESCAPE )== true  || 
		m_pDI->KeyPressed(DIK_RETURN) == true
		)
		return true;
	else
		return false;
}

// MoveKeysPressed
// - returns true if any of the movement keys were pressed this frame
bool CKeyboard::MoveKeysPressed()
{

	if ( 
		CursorMoveUp()	||
		CursorMoveDown()  || 
		CursorMoveLeft()  ||
		CursorMoveRight() 
		)
		return true;
	else 
		return false;
}

// MoveKeysReleased
// - returns true if any of the movement keys were released this frame
bool CKeyboard::MoveKeysReleased( void )
{
	if (
		m_pDI->KeyReleased(DIK_W)                 || 
		m_pDI->KeyReleased(DIK_UP)                ||
		m_pDI->KeyReleased(DIK_S)                 ||
		m_pDI->KeyReleased(DIK_DOWN)              || 
		m_pDI->KeyReleased(DIK_A)                 ||
		m_pDI->KeyReleased(DIK_LEFT)              || 
		m_pDI->KeyReleased(DIK_D)                 || 
		m_pDI->KeyReleased(DIK_RIGHT)             
		)
		return true;
	else
		return false;
}

// MoveKeysDown
// - returns true if any of the movement keys are currently held down
bool CKeyboard::MoveKeysDown( void )
{	
	if ( 
		m_pDI->KeyDown (DIK_W)					    ||
		m_pDI->KeyDown (DIK_UP)						||
		m_pDI->KeyDown (DIK_S)					    ||
		m_pDI->KeyDown (DIK_DOWN)				    || 
		m_pDI->KeyDown (DIK_A)					    || 
		m_pDI->KeyDown (DIK_LEFT)				    || 
		m_pDI->KeyDown (DIK_D)					    || 
		m_pDI->KeyDown (DIK_RIGHT)				    
		)
		return true;
	else
		return false;
}

bool CKeyboard::Fireball( )
{
	if(m_pDI->KeyPressed(DIK_NUMPAD4) == true)
		return true;
	else
		return false;
}

bool CKeyboard::SubZero( )
{
	if(m_pDI->KeyPressed(DIK_NUMPAD5) == true)
		return true;
	else
		return false;
}

bool CKeyboard::SweepingWind( )
{

	if(m_pDI->KeyPressed(DIK_NUMPAD6) == true)
		return true;
	else
		return false;
}

bool CKeyboard::ShockWave( )
{
	if	(m_pDI->KeyPressed(DIK_NUMPAD0) == true)
		return true;
	else
		return false;
}

bool CKeyboard::SpecAtk()
{

	if(
		m_pDI->KeyPressed(DIK_SPACE)				  ||
		m_pDI->JoystickButtonPressed( Spec_Atk )	  ||
		m_pDI->JoystickGetRStickXAmount(XPAD_RTRIG)	  ||
		m_pDI->JoystickGetRStickXAmount(XPAD_LTRIG)
		)
	{
		return true;
	}
	else
		return false;

}

