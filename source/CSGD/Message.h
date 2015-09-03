/*************************************************************
	File:		Message.h
	Author:		Doug Monroe (dmonroe@fullsail.com)
	Course:		SGD 1305
	Purpose:	CMessage class is the concrete base class
				for messages sent to the Message System.
*************************************************************/

#pragma once

#include "IMessage.h"


//////////////////////////////////////////////////////////////
// Enumeration of Message Types
enum MessageType {
					MSG_REMOVE_ENTITY        , MSG_CREATE_ENEMY       , MSG_CREATE_POPUP       ,
					MSG_TOGGLE_CAMERA_LOCK   , MSG_CREATE_SCREEN_LOCK , MSG_CREATE_TRAP        ,
					MSG_CREATE_PLAYER        , MSG_DROP_EXP           , MSG_CREATE_PROJECTILE  ,
					MSG_PERFORM_SUBZERO      , MSG_PERFORM_SHOCKWAVE  , MSG_PERFORM_FIREBALL   ,
					MSG_PERFORM_SWEEPING_WIND, MSG_CREATE_MENU_POPUP  , MSG_CREATE_WRECKING_BALL,
					MSG_FIREBOSS_FIREBALL	 , MSG_FIRE_SPECIAL		  , MSG_ICE_SPECIAL			,
					MSG_WIND_SPECIAL		 , MSG_ICEBOSS_ICEBALL	  , MSG_CREATE_ROCK_FALL	,
					MSG_WIND_FAN			 , MSG_WIND_TORNADO		  , MSG_ROCK_SPECIAL	    ,
					MSG_ICEBALL,
				 };


//////////////////////////////////////////////////////////////
// CMessage class
//	- concrete base class for messages
//	- stores the message ID
class CMessage : public IMessage
{
public:
	//////////////////////////////////////////////////////////
	// Constructor REQUIRES the message type
	CMessage( MessageType type )				{	m_eMsgType = type; 	}
	virtual ~CMessage( void )					{	}
	
	//////////////////////////////////////////////////////////
	// Accessor
	virtual MSGID GetMessageID( void ) final	{	return m_eMsgType;	}
	void SetMessageID(MessageType i)			{	m_eMsgType = i;		}

private:
	//////////////////////////////////////////////////////////
	// Message ID
	MessageType		m_eMsgType;
};

