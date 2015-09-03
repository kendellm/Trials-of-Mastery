/*
	File:		BaseInput.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1308
	Purpose:	IBaseInput is the interface class for all 
*/


#ifndef BASE_INPUT_H
#define BASE_INPUT_H



class CSGD_DirectInput;



class IBaseInput
{	
public:

	virtual ~IBaseInput(void)=0{}

	// Cursor Controls
	virtual bool CursorMoveUp		( void )	=0;
	virtual bool CursorMoveDown		( void )	=0;
	virtual bool CursorMoveRight	( void )	=0;
	virtual bool CursorMoveLeft		( void )	=0;

	// Player Movement
	virtual bool MoveRight          ( void )	=0;
	virtual bool MoveLeft           ( void )	=0;
	virtual bool MoveUp	            ( void )	=0;
	virtual bool MoveDown           ( void )	=0;
	virtual bool MoveKeysPressed    ( void )	=0;
	virtual bool MoveKeysReleased   ( void )	=0;
	virtual bool MoveKeysDown		( void )	=0;
	virtual bool IsJoystickDown		( void )	=0;

	// Player Attacks
	virtual bool LightAtk           ( void )	=0;
	virtual bool MedAtk	            ( void )	=0;
	virtual bool HeavyAtk           ( void )	=0;

	virtual bool SpecAtk            ( void )	=0;
	virtual bool Fireball			( bool b )	=0;
	virtual bool SubZero            ( bool b )	=0;
	virtual bool SweepingWind       ( bool b )	=0;
	virtual bool ShockWave		    ( bool b )	=0;

	virtual bool Prev_Special		( void )	=0;
	virtual bool Next_Special		( void )	=0;

	////Accessors
	//CSGD_DirectInput*	GetDI(void) const	{ return m_pDI; }
	//int		GetLightAtk		 ( void ) const		{ return	Light_Atk;	}
	//int		GetMedAtk		 ( void ) const		{ return	Med_Atk;	}
	//int		GetHeavyAtk		 ( void ) const		{ return	Heavy_Atk;	}
	//int		GetSpectAtk		 ( void ) const		{ return	Spec_Atk;	}
	//int		GetNextSpec		 ( void ) const		{ return	next_spec;	}		
	//int		GetPrevSpec		 ( void ) const		{ return	prev_spec;	}
	//int		GetConfirm		 ( void ) const		{ return	confirm;	}
	//int		GetCancel		 ( void ) const		{ return	cancel;		}
	//float	GetTimer		 ( void ) const		{ return	m_fTimer;	}

	////Mutators
	//void		SetLightAtk		 ( int value )		{ Light_Atk			= value;	}
	//void		SetMedAtk		 ( int value )		{ Med_Atk			= value;	}
	//void		SetHeavyAtk		 ( int value )		{ Heavy_Atk			= value;	}
	//void		SetSpecAtk		 ( int value )		{ Spec_Atk			= value;	}
	//void		SetNextSpec		 ( int value )		{ next_spec			= value;	}
	//void		SetPrevSpec		 ( int value )		{ prev_spec			= value;	}
	//void		SetConfirm		 ( int value )		{ confirm			= value;	}
	//void		SetCancel		 ( int value )		{ cancel			= value;	}
	//void		SetTimer		 ( float value )	{ m_fTimer			= value;	}

};

#endif