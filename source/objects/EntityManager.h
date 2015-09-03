/*************************************************************
	File:		EntityManager.h
	Author:		Doug Monroe (dmonroe@fullsail.com)
	Course:		SGD 1305
	Purpose:	CEntityManager class stores & maintains
				all game entities.
*************************************************************/


#pragma once

#include <vector>

class IEntity;

//////////////////////////////////////////////////////////////
// CEntityManager class
//	- stores a vector of vectors of IEntity* (similar to a hash table)
//	- updates & renders objects
class CEntityManager
{
public:
	//////////////////////////////////////////////////////////
	CEntityManager(void);
	~CEntityManager(void);
	enum EM_Layers{PLAYER_LAYER, ENEMY_LAYER, PROJECTILE_LAYER, TRAP_LAYER,};

	//////////////////////////////////////////////////////////7
	// Entity Management
	void AddEntity		( IEntity* pEntity, unsigned int unType );
	void RemoveEntity	( IEntity* pEntity );
	void RemoveEntity	( IEntity* pEntity, unsigned int unType );
	void RemoveAll		( unsigned int unType );
	void RemoveAll		( void );

	//////////////////////////////////////////////////////////
	// Entity Upkeep
	void UpdateAll		( float fElapsedTime );
	void RenderAll		( void );
	void RenderAllZBuffered ( void );
	void CheckAllCollisions	( unsigned int unType1, unsigned int unType2 );
	void CheckCollision ( IEntity* );

private:
	//////////////////////////////////////////////////////////
	// Use typedefs to clarify names
	typedef std::vector< IEntity* >		EntityVector;
	typedef EntityVector::iterator		EVIterator;
	typedef std::vector< EntityVector >	EntityTable;

	EntityTable		m_tEntities;	// table of IEntity*
	bool			m_bIterating;	// lock to protect the table
};

