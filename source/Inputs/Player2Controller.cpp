/*************************************************************
File:			Controller.cpp
Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
Purpose:		Serves as basic interface between multiple input
systems (keyboard, x360)  and game states. 
*************************************************************/

#include "Player2Controller.h"
#include "../CSGD/CSGD_DirectInput.h"


CPlayer2Controller* CPlayer2Controller::GetInstance( void )
{
	static CPlayer2Controller s_Instance;
	return &s_Instance;
}




CPlayer2Controller::CPlayer2Controller(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();

		Light_Atk =  3;
		Med_Atk	  =  4;
		Heavy_Atk =  5;
		Spec_Atk  =  0;
		prev_spec =  1;
		next_spec =  2;
		m_fTimer = 0.0f;
}

CPlayer2Controller::~CPlayer2Controller(void)
{
}


void CPlayer2Controller::Update( float fElaspedTime ) 
{

	m_fTimer = max(m_fTimer - fElaspedTime, 0);
}

bool CPlayer2Controller::CursorMoveUp()
{

	if(m_pDI->JoystickGetLStickDirPressed(DIR_UP, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::CursorMoveDown()
{

	if(m_pDI->JoystickGetLStickDirPressed( DIR_DOWN, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::CursorMoveRight()
{

	if(m_pDI->JoystickGetLStickDirPressed(DIR_RIGHT, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::CursorMoveLeft()
{
	if(m_pDI->JoystickGetLStickDirPressed(DIR_LEFT, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::MoveRight()
{

	if(m_pDI->JoystickGetLStickDirDown(DIR_RIGHT, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::MoveLeft()
{

	if(m_pDI->JoystickGetLStickDirDown(DIR_LEFT, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::MoveUp()
{

	if(m_pDI->JoystickGetLStickDirDown(DIR_UP, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::MoveDown()
{
	if(m_pDI->JoystickGetLStickDirDown(DIR_DOWN, 1))
		return true;
	else
		return false;
}

bool CPlayer2Controller::LightAtk()
{

	if(m_pDI->JoystickButtonPressed( Light_Atk, 1 ))
		return true;
	else
		return false;
}

bool CPlayer2Controller::MedAtk()
{

	if(m_pDI->JoystickButtonPressed( Med_Atk, 1 ))
		return true;
	else
		return false;
}

bool CPlayer2Controller::HeavyAtk()
{
	if(m_pDI->JoystickButtonPressed( Heavy_Atk, 1 ))
		return true;
	else
		return false;
}

// MoveKeysPressed
// - returns true if any of the movement keys were pressed this frame
bool CPlayer2Controller::MoveKeysPressed()
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
bool CPlayer2Controller::MoveKeysReleased( void )
{
	if (
		m_pDI->JoystickButtonReleasedEx(DIR_UP, 1)     ||
		m_pDI->JoystickButtonReleasedEx(DIR_DOWN, 1)   ||
		m_pDI->JoystickButtonReleasedEx(DIR_LEFT, 1)   ||
		m_pDI->JoystickButtonReleasedEx(DIR_RIGHT, 1)
		)
		return true;
	else
		return false;
}

// MoveKeysDown
// - returns true if any of the movement keys are currently held down
bool CPlayer2Controller::MoveKeysDown( void )
{	
	if (
		m_pDI->JoystickGetLStickDirDown(DIR_UP, 1)    ||
		m_pDI->JoystickGetLStickDirDown(DIR_DOWN, 1)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_LEFT, 1)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_RIGHT, 1) 	
		)
		return true;
	else
		return false;
}

bool CPlayer2Controller::Fireball( bool b )
{
	if(b)
		return true;
	else
	{	
		if(m_pDI->KeyPressed(DIK_NUMPAD4) == true)
			return true;
		else
			return false;
	}
}

bool CPlayer2Controller::SubZero( bool b )
{
	if(b)
		return true;
	else
	{
		if(m_pDI->KeyPressed(DIK_NUMPAD5) == true)
			return true;
		else
			return false;
	}
}

bool CPlayer2Controller::SweepingWind( bool b )
{
	if(b)
		return true;
	else
	{
		if(m_pDI->KeyPressed(DIK_NUMPAD6) == true)
			return true;
		else
			return false;
	}
}

bool CPlayer2Controller::ShockWave( bool b )
{
	if(b)
		return true;
	else
	{
		if(m_pDI->KeyPressed(DIK_NUMPAD0) == true)
			return true;
		else
			return false;
	}
}

bool CPlayer2Controller::Prev_Special( void )
{
	if(
		m_pDI->KeyPressed(DIK_Q)				  ||	
		m_pDI->JoystickButtonPressed( prev_spec )
		)
		return true;
	else
		return false;
}

bool CPlayer2Controller::Next_Special( void )
{
	if(
		m_pDI->KeyPressed(DIK_E)				    ||
		m_pDI->JoystickButtonPressed( next_spec )  
		)
		return true;
	else
		return false;
}

bool CPlayer2Controller::SpecAtk()
{

	if(m_pDI->JoystickButtonPressed( Spec_Atk ))
	{
		return true;
	}
	else
		return false;

}

bool CPlayer2Controller::IsJoystickDown( void )
{
	if (m_pDI->JoystickGetLStickDirDown(DIR_UP, 1)    ||
		m_pDI->JoystickGetLStickDirDown(DIR_DOWN, 1)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_LEFT, 1)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_RIGHT, 1) 
		)
		return true;
	else
		return false;
}




