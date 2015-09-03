/*
File:		RemoveEntityMessage.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CRemoveEntityMessage class is sent to the
			Message System to remove an entity from the
			Entity Manager.
*/

#include "RemoveEntityMessage.h"
#include "../objects/Entity.h"
#include <cassert>



CRemoveEntityMessage::CRemoveEntityMessage(CEntity* ptr)
	: CMessage( MSG_REMOVE_ENTITY )
{
	// Validate the parameter
	assert( ptr != nullptr && "CDestroyEntityMessage - cannot destroy null" );


	// Store the parameter & hold a reference
	m_pEntity = ptr;
	m_pEntity->AddRef();
}


CRemoveEntityMessage::~CRemoveEntityMessage(void)
{
	// Release the reference to the entity
	m_pEntity->Release();
	m_pEntity = nullptr;
}


