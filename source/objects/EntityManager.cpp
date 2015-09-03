/*************************************************************
File:		EntityManager.cpp
Author:		Doug Monroe (dmonroe@fullsail.com)
Course:		SGD 1305
Purpose:	CEntityManager class stores & maintains
all game entities.
*************************************************************/


#include "EntityManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../Util/Rect.h"

#include "../CSGD/CSGD_EventSystem.h"
#include "IEntity.h"
#include "Entity.h"
#include "Trap.h"
#include "Player.h"

#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/RemoveEntityMessage.h"

#include <algorithm>

#include <cassert>


//////////////////////////////////////////////////////////////
// Constructor
CEntityManager::CEntityManager( void )
{
	m_bIterating = false;
}

//////////////////////////////////////////////////////////////
// Destructor
CEntityManager::~CEntityManager( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "~CEntityManager - cannot be destroyed while iterating!" );
}

//////////////////////////////////////////////////////////////
// AddEntity
//	- add the entity into the table in the specified bucket
//	  & store a reference to the entity
void CEntityManager::AddEntity( IEntity* pEntity, unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::AddEntity - cannot add while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::AddEntity - pointer cannot be null" );

	// Expand the vector?
	if( unType >= m_tEntities.size() )
		m_tEntities.resize( unType + 1 );


	// Add the entity to the end of the bucket
	m_tEntities[ unType ].push_back( pEntity );
	pEntity->AddRef();
}

//////////////////////////////////////////////////////////////
// RemoveEntity
//	- remove & release the entity from any bucket
void CEntityManager::RemoveEntity( IEntity* pEntity )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );

	// Try to find the entity
	for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
	{
		EntityVector& vec = m_tEntities[ type ];
		for( int i = (int)vec.size() - 1; i >= 0; i-- )
		{
			if( vec[ i ] == pEntity )
			{
				// Remove the entity
				pEntity->Release();
				vec.erase( vec.begin() + i );
				return;
			}
		}
	}
}

//////////////////////////////////////////////////////////////
// RemoveEntity
//	- remove & release an entity from a specific bucket
void CEntityManager::RemoveEntity( IEntity* pEntity, unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );
	assert( unType < m_tEntities.size() && "CEntityManager::RemoveEntity - invalid type" );

	// Try to find the entity
	EntityVector& vec = m_tEntities[ unType ];
	for( int i = (int)vec.size() - 1; i >= 0; i-- )
	{
		if( vec[ i ] == pEntity )
		{
			// Remove the entity
			pEntity->Release();
			vec.erase( vec.begin() + i );
			break;
		}
	}
}

//////////////////////////////////////////////////////////////
// RemoveAll
//	- remove & release all the entities from a specific bucket
void CEntityManager::RemoveAll( unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );

	// Validate the parameter
	assert( unType < m_tEntities.size() && "CEntityManager::RemoveAll - invalid type" );

	// Release each entity in the bucket
	EntityVector& vec = m_tEntities[ unType ];
	for( int i = (int)vec.size() - 1; i >= 0; i-- )
	{
		vec[ i ]->Release();
	}

	// Collapse the vector
	m_tEntities[ unType ].clear();
}

//////////////////////////////////////////////////////////////
// RemoveAll
//	- remove all entities from the table
void CEntityManager::RemoveAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Release();
			}
		}

		// Collapse the table
		m_tEntities.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}

//////////////////////////////////////////////////////////////
// UpdateAll
//	- update each entity
void CEntityManager::UpdateAll( float fElapsedTime )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::UpdateAll - cannot update while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Loop through the buckets
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			// Loop through the entities
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Update( fElapsedTime );
				// call check collision
				CheckCollision(vec[i]);
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

//////////////////////////////////////////////////////////////
// RenderAll
//	- render each entity
void CEntityManager::RenderAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RenderAll - cannot render while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Loop through the buckets
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			// Loop through the entities
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				CEntity* temp = dynamic_cast<CEntity*>(vec[i]);
				if(temp->IsOffScreen() == false)
					vec[ i ]->Render();
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

void CEntityManager::RenderAllZBuffered ( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RenderAll - cannot render while iterating" );

	// Get all of the entities into a single bucket
	vector<CEntity*> buffer;
	vector<CEntity*> background_layer;

	// Loop through all the buckets
	int type = (int)m_tEntities.size();
	int size = type;
	for( size == 4 ? type -= 2 : type -= 1 ; type >= 0; type-- )
	{
		// Push each entity onto the buffer vector
		EntityVector& vec = m_tEntities[ type ];
		for( int i = (int)vec.size() - 1; i >= 0; i-- )
		{
			buffer.push_back((CEntity*)vec[i]);
		}
	}

	// sort the buffer based on the position
	sort(buffer.begin(), buffer.end(), [](CEntity* const l, CEntity* const r) -> bool {
		return l->GetPosY() < r->GetPosY();
	});

	// Lock the iterator
	m_bIterating = true;
	{
		if (size >= 4 )
		{
			EntityVector& background = m_tEntities[ TRAP_LAYER ];
			// Loop through and render the background 
			for( int i = (int)background.size() - 1; i >= 0; i-- )
			{
				background[i]->Render();
			}
		}

		// Loop through and render the buffer
		for( unsigned int i = 0; i < buffer.size(); ++i)
		{
			buffer[i]->Render();
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}



//////////////////////////////////////////////////////////////
// CheckAllCollisions
//	- check collision between the entities within the two buckets
void CEntityManager::CheckCollision ( IEntity* p)
{
	// Loop through the buckets
	for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
	{
		// Loop through the entities
		EntityVector& vec = m_tEntities[ type ];
		for( int i = (int)vec.size() - 1; i >= 0; i-- )
		{
			if (vec[i] != p)
			{
				RECT rOverlap = { };
				if( ::CheckCollision( p, vec[ i ], rOverlap) )
				{
					p->HandleCollision( vec[ i ], rOverlap );
				}


				if (AnimationManager::GetInstance()->GetActiveRect( ((CEntity*)(p))->GetAnimInfo()).IsActive() )
				{

					if ( ::CheckActiveRectCollision( p, vec[i], rOverlap ) )
					{ 
						p->HandleActiveRectCollision(vec[i]);
					}

				}
			}
		}
	}
}



//////////////////////////////////////////////////////////////
// CheckAllCollisions
//	- check collision between the entities within the two buckets
void CEntityManager::CheckAllCollisions( unsigned int unType1, unsigned int unType2 )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RenderAll - cannot render while iterating" );

	// Quietly validate the parameters
	if( unType1 >= m_tEntities.size() 
		|| unType2 >= m_tEntities.size()
		|| m_tEntities[ unType1 ].size() == 0 
		|| m_tEntities[ unType2 ].size() == 0 )
		return;


	// Lock the iterator
	m_bIterating = true;
	{
		// Easy reference to the buckets
		EntityVector& vec1 = m_tEntities[ unType1 ];
		EntityVector& vec2 = m_tEntities[ unType2 ];

		// Which bucket is smaller? 
		//	should be the outer loop for less checks (n0)*(n1+1) + 1
		int size1 = (int)vec1.size();
		int size2 = (int)vec2.size();

		if( size1 <= size2 )
		{
			for( int i = size1-1; i >= 0; i-- )
			{
				for( int j = size2-1; j >= 0; j-- )
				{
					RECT rOverlap = { };
					if( ::CheckCollision( vec1[ i ], vec2[ j ], rOverlap) )
					{
						// Get access to each entities velocities and prevent movement
						vec1[ i ]->HandleCollision( vec2[ j ], rOverlap );
						vec2[ j ]->HandleCollision( vec1[ i ], rOverlap );
					}


					if (AnimationManager::GetInstance()->GetActiveRect( ((CEntity*)vec1[i])->GetAnimInfo()).IsActive() )
					{
						if ( ::CheckActiveRectCollision( vec1[i], vec2[j], rOverlap ) )
						{ 
							vec2[j]->ModifyHealth( AnimationManager::GetInstance()->GetCurrentFrame(((CEntity*)vec1[i])->GetAnimInfo())->GetDamage() );


							//Check the colliding frame for a trigger
							if ( "-1" != AnimationManager::GetInstance()->CheckTrigger(*((CEntity*)vec1[i])->GetAnimInfo()) )
							{
								CSGD_EventSystem::GetInstance()->SendEvent("BURN", nullptr, vec2[j], vec1[i]);
							}

						}
					}
				}
			}
		}
		else 
		{
			for( int j = size2-1; j >= 0; j-- )
			{
				for( int i = size1-1; i >= 0; i-- )
				{
					RECT rOverlap = { };
					if( ::CheckCollision( vec1[ i ], vec2[ j ], rOverlap ) )
					{
						vec1[ i ]->HandleCollision( vec2[ j ], rOverlap );
						vec2[ j ]->HandleCollision( vec1[ i ], rOverlap );
					}
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}
