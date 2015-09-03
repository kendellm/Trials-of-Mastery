#pragma once
#include "Boss_Fire_Special.h"


CBoss_Special::CBoss_Special( float x, float y, string anim, string name, float speed ) : CProjectile( anim, name, speed )
{
	SetPosX( x );
	SetPosY( y );
//	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
}
CBoss_Special::~CBoss_Special( void )
{

}
void CBoss_Special::Update(float fElapsedTime)
{
	CProjectile::Update( fElapsedTime );
}
void CBoss_Special::Render(void)
{
	CProjectile::Render();
}
RECT CBoss_Special::GetCollisionRect	( void ) const			
{
	return CProjectile::GetCollisionRect();
}
Rect CBoss_Special::GetActiveRect		( void ) const			
{
	return CProjectile::GetActiveRect();
}
void CBoss_Special::HandleEvent(CEvent* pEvent)			
{

	CProjectile::HandleEvent( pEvent );
	/*if (pEvent->GetEventID() == "SELF_DESTRUCT")
	{
		if (pEvent->GetDestination() == this)
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}

	if (pEvent->GetEventID() == "Self Destruct")
	{
		if (pEvent->GetDestination() == this)
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}*/
}
void CBoss_Special::HandleActiveRectCollision ( const IEntity* pOther ) 
{
	CProjectile::HandleActiveRectCollision( pOther );
}