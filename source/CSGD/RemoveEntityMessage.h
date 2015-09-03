/*
File:		RemoveEntityMessage.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CRemoveEntityMessage class is sent to the
			Message System to remove an entity from the
			Entity Manager.
*/

#ifndef REMOVE_ENTITY_MESSAGE_H
#define REMOVE_ENTITY_MESSAGE_H

#include "Message.h"

class CEntity;

class CRemoveEntityMessage : public CMessage
{
public:
	CRemoveEntityMessage(CEntity* ptr);
	virtual ~CRemoveEntityMessage(void);

	CEntity*	GetEntity( void ) const		{ return m_pEntity; }

private:
	CEntity* m_pEntity;
};

#endif