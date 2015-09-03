/*************************************************************
File:			Controller.cpp
Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
Purpose:		Serves as basic interface between multiple input
systems (keyboard, x360)  and game states. 
*************************************************************/

#include "Controller.h"
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



CController* CController::GetInstance( void )
{
	static CController s_Instance;
	return &s_Instance;
}




CController::CController(void)
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



CController::~CController(void)
{
}


bool CController::isConnected( void )
{
	memset(&m_XpadState, 0, sizeof(XINPUT_STATE)); 
	DWORD result = XInputGetState(m_nPlayer1, &m_XpadState); 
	return  result == ERROR_DEVICE_NOT_CONNECTED ? false : true;  
}


void CController::Update( float fElaspedTime ) 
{

	m_fTimer = max(m_fTimer - fElaspedTime, 0);
	if(isConnected())
	{
		if(m_fTimer <=0)
			Rumble(0);
		if(isConnected())
			SetXpad(true);
		else
			SetXpad(false);
	}
}

void CController::SetXpad( bool b )
{
	if(b)
	{  //if 360 controller is plugged in
		Light_Atk =  2;
		Med_Atk	  =  3;
		Heavy_Atk =  1;
		Spec_Atk  =  0;
		prev_spec =  4;
		next_spec =  5;
		confirm   =  0;
		cancel    =  1;
	}
	else
	{ //if controller is NOT plugged in, switch to Arcade controls
		Light_Atk =  3;
		Med_Atk	  =  4;
		Heavy_Atk =  5;
		Spec_Atk  =  0;
		prev_spec =  1;
		next_spec =  2;
		confirm   =  0;
		cancel    =  6;
	}

}				 


bool CController::CursorMoveUp()
{

	if(
		m_pDI->KeyPressed(DIK_W) == true			||
		m_pDI->KeyPressed(DIK_UP) == true			||
		m_pDI->JoystickGetLStickDirPressed(DIR_UP)  ||
		m_pDI->JoystickDPadPressed(2)
		)
		return true;
	else
		return false;
}

bool CController::CursorMoveDown()
{

	if( 
		m_pDI->KeyPressed(DIK_S) == true				 ||
		m_pDI->KeyPressed(DIK_DOWN) == true			 ||
		m_pDI->JoystickGetLStickDirPressed( DIR_DOWN)  ||
		m_pDI->JoystickDPadPressed(DPAD_DOWN)
		)
		return true;
	else
		return false;
}

bool CController::CursorMoveRight()
{

	if( 
		m_pDI->KeyPressed(DIK_D) == true				  ||
		m_pDI->KeyPressed(DIK_RIGHT) == true			  ||
		m_pDI->JoystickGetLStickDirPressed(DIR_RIGHT)    ||
		m_pDI->JoystickDPadPressed(DPAD_RIGHT)
		)
		return true;
	else
		return false;
}

bool CController::CursorMoveLeft()
{
	if( 
		m_pDI->KeyPressed(DIK_A) == true			  ||
		m_pDI->KeyPressed(DIK_LEFT) == true			  ||
		m_pDI->JoystickGetLStickDirPressed(DIR_LEFT)  ||
		m_pDI->JoystickDPadPressed(DPAD_LEFT)
		)
		return true;
	else
		return false;
}

bool CController::MoveRight()
{

	if( 
		m_pDI->KeyDown(DIK_D) == true				||
		m_pDI->KeyDown(DIK_RIGHT) == true			||
		m_pDI->JoystickGetLStickDirDown(DIR_RIGHT)	||
		m_pDI->JoystickDPadDown( DPAD_RIGHT)
		)
		return true;
	else
		return false;
}

bool CController::MoveLeft()
{

	if( 
		m_pDI->KeyDown(DIK_A) == true				||
		m_pDI->KeyDown(DIK_LEFT) == true		    ||
		m_pDI->JoystickGetLStickDirDown(DIR_LEFT)   ||
		m_pDI->JoystickDPadDown(DPAD_LEFT)
		)
		return true;
	else
		return false;
}

bool CController::MoveUp()
{

	if(
		m_pDI->KeyDown(DIK_W) == true				 ||
		m_pDI->KeyDown(DIK_UP) == true		 		 ||
		m_pDI->JoystickGetLStickDirDown(DIR_UP)		 ||
		m_pDI->JoystickDPadDown(DPAD_UP)
		)
		return true;
	else
		return false;
}

bool CController::MoveDown()
{
	if( 
		m_pDI->KeyDown(DIK_S) == true		    	||
		m_pDI->KeyDown(DIK_DOWN) == true   		||
		m_pDI->JoystickGetLStickDirDown(DIR_DOWN)	||
		m_pDI->JoystickDPadDown(DPAD_DOWN)
		)
		return true;
	else
		return false;
}

bool CController::LightAtk()
{

	if(
		m_pDI->KeyPressed(DIK_NUMPAD1) == true		||
		m_pDI->JoystickButtonPressed( Light_Atk )
		)
		return true;
	else
		return false;
}

bool CController::MedAtk()
{

	if(
		m_pDI->KeyPressed(DIK_NUMPAD2) == true		||
		m_pDI->JoystickButtonPressed( Med_Atk )
		)
		return true;
	else
		return false;
}

bool CController::HeavyAtk()
{
	if(
		m_pDI->KeyPressed(DIK_NUMPAD3) == true		||
		m_pDI->JoystickButtonPressed( Heavy_Atk )
		)
		return true;
	else
		return false;

}


bool CController::Confirm()
{
	if(
		m_pDI->KeyPressed(DIK_RETURN) == true		||
		m_pDI->JoystickButtonPressed( confirm )
		)
		return true;
	else
		return false;
}

bool CController::Cancel( void )
{
	if(
		m_pDI->KeyPressed(DIK_ESCAPE) == true     ||
		m_pDI->KeyPressed(DIK_BACKSPACE) == true  ||
		m_pDI->JoystickButtonPressed( cancel )   
		)
		return true;
	else
		return false;
}

bool CController::Escape( void )
{
	if(
		m_pDI->KeyPressed(DIK_ESCAPE) == true
		)
		return true;
	else
		return false;
}

bool CController::Pause( void )
{


	if( m_pDI->KeyPressed( DIK_ESCAPE )== true  || 
		m_pDI->KeyPressed(DIK_RETURN) == true   ||
		m_pDI->JoystickButtonPressed(XPAD_BACK)	||
		m_pDI->JoystickButtonPressed(XPAD_START) 
		)
		return true;
	else
		return false;
}

// MoveKeysPressed
// - returns true if any of the movement keys were pressed this frame
bool CController::MoveKeysPressed()
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
bool CController::MoveKeysReleased( void )
{
	if (
		m_pDI->KeyReleased(DIK_W)                 || 
		m_pDI->KeyReleased(DIK_UP)                ||
		m_pDI->KeyReleased(DIK_S)                 ||
		m_pDI->KeyReleased(DIK_DOWN)              || 
		m_pDI->KeyReleased(DIK_A)                 ||
		m_pDI->KeyReleased(DIK_LEFT)              || 
		m_pDI->KeyReleased(DIK_D)                 || 
		m_pDI->KeyReleased(DIK_RIGHT)             ||
		m_pDI->JoystickDPadReleased(DPAD_UP)	   ||
		m_pDI->JoystickDPadReleased(DPAD_DOWN)	   ||
		m_pDI->JoystickDPadReleased(DPAD_LEFT)	   ||
		m_pDI->JoystickDPadReleased(DPAD_RIGHT)   ||
		m_pDI->JoystickButtonReleasedEx(DIR_UP)     ||
		m_pDI->JoystickButtonReleasedEx(DIR_DOWN)   ||
		m_pDI->JoystickButtonReleasedEx(DIR_LEFT)   ||
		m_pDI->JoystickButtonReleasedEx(DIR_RIGHT)
		)
		return true;
	else
		return false;
}

// MoveKeysDown
// - returns true if any of the movement keys are currently held down
bool CController::MoveKeysDown( void )
{	
	if ( 
		m_pDI->KeyDown (DIK_W)					    ||
		m_pDI->KeyDown (DIK_UP)						||
		m_pDI->KeyDown (DIK_S)					    ||
		m_pDI->KeyDown (DIK_DOWN)				    || 
		m_pDI->KeyDown (DIK_A)					    || 
		m_pDI->KeyDown (DIK_LEFT)				    || 
		m_pDI->KeyDown (DIK_D)					    || 
		m_pDI->KeyDown (DIK_RIGHT)				    ||
		m_pDI->JoystickDPadDown(DPAD_UP)		    ||
		m_pDI->JoystickDPadDown(DPAD_DOWN)		    ||
		m_pDI->JoystickDPadDown(DPAD_LEFT)		    ||
		m_pDI->JoystickDPadDown(DPAD_RIGHT)			||
		m_pDI->JoystickGetLStickDirDown(DIR_UP)    ||
		m_pDI->JoystickGetLStickDirDown(DIR_DOWN)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_LEFT)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_RIGHT) 	
		)
		return true;
	else
		return false;
}

bool CController::Fireball( bool b )
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

bool CController::SubZero( bool b )
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

bool CController::SweepingWind( bool b )
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

bool CController::ShockWave( bool b )
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

void CController::Rumble( int left, int right )
{
	if(right = 1)
		right = left;
	if(left >0 && right > 0)
		m_fTimer = 1;

	XINPUT_VIBRATION Vibration;
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	Vibration.wLeftMotorSpeed = left;
	Vibration.wRightMotorSpeed = right;
	XInputSetState(m_nPlayer1, &Vibration);
	XInputGetState(m_nPlayer1, &m_XpadState);
}

void CController::Rumble( MODE setting )
{
	switch(setting)
	{
	case HIGH:	 Rumble(65535);
		break;
	case MEDIUM: Rumble(45535);
		break;	
	case LOW:	 Rumble(25535);
		break;
	}
}

bool CController::Prev_Special( void )
{
	if(	
		m_pDI->JoystickButtonPressed( prev_spec )
		)
		return true;
	else
		return false;
}

bool CController::Next_Special( void )
{
	if(
		m_pDI->JoystickButtonPressed( next_spec )  
		)
		return true;
	else
		return false;
}

bool CController::SpecAtk()
{

	if(
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # 44 / 56
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		//m_pDI->KeyPressed(DIK_SPACE)				  ||

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # 44 / 56
		//////////////////////////////////////////////////////////////////////////
		m_pDI->JoystickButtonPressed( Spec_Atk )	  ||
		m_pDI->JoystickGetRStickXAmount(XPAD_RTRIG)	  ||
		m_pDI->JoystickGetRStickXAmount(XPAD_LTRIG)
		)
	{
		Rumble(LOW);
		return true;
	}
	else
		return false;

}

bool CController::IsJoystickDown( void )
{
	if (m_pDI->JoystickGetLStickDirDown(DIR_UP)    ||
		m_pDI->JoystickGetLStickDirDown(DIR_DOWN)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_LEFT)  ||
		m_pDI->JoystickGetLStickDirDown(DIR_RIGHT) 
		)
		return true;
	else
		return false;
}




