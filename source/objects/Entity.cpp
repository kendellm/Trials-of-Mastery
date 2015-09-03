#include "Entity.h"

#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"


#include <cassert>
#include "../Util/Rect.h"
#include "../CSGD/SGD_Math.h"
#include "../Camera.h"
#include "../objects/Enemy.h"

//////////////////////////////////////////////////////////////
// Constructor
//	- set ALL data members to 'clean' values
CEntity::CEntity( void )
{
	// Set the entity's type
	m_eType		= ENT_BASE;

	m_fPosX			= 0;
	m_fPosY			= 0;
	m_fVelX			= 0;
	m_fVelY			= 0;
	m_fRotation		= 0;
	m_nWidth		= 0;
	m_nHeight		= 0;

	m_bFlipped		= false;

	m_szIdleAnim	= nullptr;

	m_unRefCount	= 1;	// calling 'new' gives the 'prime' reference

	

	m_pAnimInfo	= new CAnimInfo(this);
	m_pAM		= AnimationManager::GetInstance();
	m_pGC		= CCamera::GetInstance();
}	

//////////////////////////////////////////////////////////////
// Destructor
CEntity::~CEntity( void )
{
	delete m_pAnimInfo;
}


//////////////////////////////////////////////////////////////
// Update
//	- move the entity by velocity each frame
//	  (velocity is pixels per second)
/*virtual*/ void CEntity::Update( float fElapsedTime )
{
	m_fPosX += m_fVelX * fElapsedTime;
	m_fPosY += m_fVelY * fElapsedTime;
}

//////////////////////////////////////////////////////////////
// Render
//	- render the entity's image at its position
/*virtual*/ void CEntity::Render( void )
{
	// Validate the image
}
bool CEntity::IsOffScreen()
{
	CCamera* Game_Camera = CCamera::GetInstance();
	RECT Camera;
	Camera.left = (LONG)Game_Camera->GetPosX();
	Camera.right = (LONG)(Camera.left + Game_Camera->GetWidth());
	Camera.top = (LONG)Game_Camera->GetPosY();
	Camera.bottom = (LONG)(Camera.top + Game_Camera->GetHeight());

	RECT object = GetSourceRect();
	RECT Overlap;
	if(IntersectRect( &Overlap, &Camera, &object) == FALSE)
		return false;
	else 
		return true;
}

//////////////////////////////////////////////////////////////
// GetCollisionRect
//	- return the entity's bounding rectangle
RECT CEntity::GetSourceRect( void ) const
{
	RECT rSource = m_pAM->GetCurrentFrame(m_pAnimInfo)->GetSourceRect().GetRECT();
	CFrame* currFrame = m_pAM->GetCurrentFrame(m_pAnimInfo);

	int width = rSource.right - rSource.left;
	int height = rSource.bottom - rSource.top;

	RECT bounding;

	bounding.left = (LONG)(m_fPosX - currFrame->GetAnchorPointX() - m_pGC->GetPosX() );
	bounding.top = (LONG)(m_fPosY - currFrame->GetAnchorPointY() - m_pGC->GetPosY() );
	bounding.right = bounding.left + width;
	bounding.bottom = bounding.top + height;

	return bounding;
}

//////////////////////////////////////////////////////////////
// GetCollisionRect
//	- return the entity's bounding rectangle
/*virtual*/ RECT CEntity::GetCollisionRect( void ) const
{
	CFrame* currFrame = m_pAM->GetCurrentFrame(m_pAnimInfo);
	RECT rCollision = currFrame->GetCollisionRect().GetRECT();

	if (m_bFlipped == true)
	{
		// Get the width of the collision rect before flipping
		int width = (rCollision.right - rCollision.left);

		// Get the offset from the anchor point to the top left of the collision rect
		tVector2D offset = { (float)(rCollision.left), (float)(rCollision.top ) };

		// Negate the anchor point x value
		rCollision.left  = (LONG)(-offset.fX);
		rCollision.right = (LONG)(-offset.fX);

		// Subtract the width of the collision rect
		rCollision.left  -= width;
	}

	// Add the position of the character in the world to the collision rect
	rCollision.left		+= (int)(m_fPosX - m_pGC->GetPosX());
	rCollision.right	+= (int)(m_fPosX - m_pGC->GetPosX());
	rCollision.top		+= (int)(m_fPosY - m_pGC->GetPosY());
	rCollision.bottom	+= (int)(m_fPosY - m_pGC->GetPosY());

	return rCollision;
}

/*virtual*/ Rect CEntity::GetActiveRect( void ) const
{
	CFrame* currFrame = m_pAM->GetCurrentFrame(m_pAnimInfo);
	Rect rActive = m_pAM->GetActiveRect(m_pAnimInfo);

	if (m_bFlipped == true)
	{
		// Get the width of the active rect before flipping
		int width = (rActive.right - rActive.left);

		// Get the offset from the anchor point to the top left of the collision rect
		tVector2D offset = { (float)(rActive.left), (float)(rActive.top) };

		// negate the anchor point x value
		rActive.left  = (LONG)(-offset.fX);
		rActive.right = (LONG)(-offset.fX);

		// subtract the width of the collision rect
		rActive.left -= width;
	}

	// Add the position of the character in the world to the position of the rect
	rActive.left		+= (int)(m_fPosX - m_pGC->GetPosX());
	rActive.right		+= (int)(m_fPosX - m_pGC->GetPosX());
	rActive.top			+= (int)(m_fPosY - m_pGC->GetPosY());
	rActive.bottom		+= (int)(m_fPosY - m_pGC->GetPosY());

	return rActive;
}

//////////////////////////////////////////////////////////////
// HandleCollision
//	- respond to collision with other entity
/*virtual*/ void CEntity::HandleCollision( const IEntity* pOther, const RECT rOverlap )
{

}

/*virtual*/ void CEntity::HandleActiveRectCollision ( const IEntity* pOther )
{

}


/*virtual*/ void CEntity::ModifyHealth(int damage)
{

}

//////////////////////////////////////////////////////////////
// CheckCollision
//	- returns true if the entities are overlapping
//	- global function does NOT have invoking object
//	  & does NOT have access to CEntity privates
bool ::CheckCollision( const IEntity* pEntity1, const IEntity* pEntity2, RECT& rOverlap )
{
	// Local variables allow for easier debugging
	RECT rEntity1 = pEntity1->GetCollisionRect();
	RECT rEntity2 = pEntity2->GetCollisionRect();
	if( IntersectRect( &rOverlap, &rEntity1, &rEntity2 ) == FALSE )
		return false;
	if(pEntity1->GetType() == CEntity::ENT_ENEMY)
	{
		const CEnemy* temp = dynamic_cast<const CEnemy *>(pEntity1);
		if(temp->GetEnemyType() == CEnemy::Tank)
			return false;
	}
	else if(pEntity2->GetType() == CEntity::ENT_ENEMY)
	{
		const CEnemy* temp = dynamic_cast<const CEnemy *>(pEntity2);
		if(temp->GetEnemyType() == CEnemy::Tank)
			return false;
	}

	return true;
}

bool ::CheckActiveRectCollision( const IEntity* pEntity1, const IEntity* pEntity2, RECT& rOverlap )
{

	RECT rEntity1 = pEntity1->GetActiveRect().GetRECT();
	RECT rEntity2 = pEntity2->GetCollisionRect();

	if ( IntersectRect(&rOverlap, &rEntity1, &rEntity2) == FALSE )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////
// AddRef
//	- increase the reference count
/*virtual*/ void CEntity::AddRef( void )
{
	assert( m_unRefCount != UINT_MAX && "CEntity::AddRef - reference count has exceeded max" );
	++m_unRefCount;
}

//////////////////////////////////////////////////////////////
// Release
//	- decrease the reference count
//	- self-destruct when the count reaches 0
/*virtual*/ void CEntity::Release( void )
{
	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}

void CEntity::SetPosition( float x, float y )
{
	SetPosX(x);
	SetPosY(y);
}

void CEntity::SetVelocity( float vx, float vy )
{
	SetVelX(vx);
	SetVelY(vy);
}

void CEntity::SetSize( int w, int h )
{
	SetWidth(w);
	SetHeight(h);
}

/*virtual*/ const char* CEntity::GetAnimations( const char* name ) const
{
	if (name == "Idle")
		return m_szIdleAnim;
	else
		return "";
}

/*virtual*/ void CEntity::SetBaseAnimations( EntityType eType)
{
	switch(eType)
	{

	case ENT_PLAYER:
		break;

	case ENT_ENEMY:
		break;

	case ENT_FIREBALL:
		{
			SetIdleAnim("FireBall");
		}
		break;

	case ENT_WRECKING_BALL:
		{
			SetIdleAnim("Wrecking_Ball");
		}
		break;
	case ENT_ROCK_FALL:
		{
			SetIdleAnim("Rock_Fall");
		}
		break;
	default:
		break;

	}
}
