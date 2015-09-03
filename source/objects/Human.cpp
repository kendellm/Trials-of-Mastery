/*
File:		Human.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CHuman dervies from CEntity and functionality neccessary for
playerable characters and enemies, but not other CEntities.
*/

#include "Human.h"

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


CHuman::CHuman(void)
{
	m_eType = ENT_HUMAN;

	m_dwRenderColor		= 0xFFFFFFFF;

	m_szWalkingAnim		= nullptr;
	m_szFlinchAnim		= nullptr;
	m_szFrozenAnim		= nullptr;
	m_szKnockDownAnim	= nullptr;
	m_szDeadAnim		= nullptr;
}


CHuman::~CHuman(void)
{

}

/*virtual*/ void CHuman::Update( float fElapsedTime )	
{

}

/*virtual*/ void CHuman::Render( void )	
{

}


/*virtual*/ void CHuman::HandleCollision( const IEntity* pOther, const  RECT rOverlap )	
{

}

/*virtual*/ void CHuman::HandleActiveRectCollision ( const IEntity* pOther )
{

}

void CHuman::SetRenderColor(int a, int r, int g, int b)
{ 
	m_dwRenderColor = D3DCOLOR_ARGB(a, r, g, b); 
}

/*virtual*/ const char * CHuman::GetAnimations	( const char* name ) const
{	
	if (name == "Idle")
		return CEntity::GetAnimations(name);	
	else if (name == "Walk")
		return m_szWalkingAnim;
	else if (name == "Flinch")
		return m_szFlinchAnim;
	else if (name == "Frozen")
		return m_szFrozenAnim;
	else if (name == "Knocked Down")
		return m_szKnockDownAnim;
	else if (name == "Dead")
		return m_szDeadAnim;

	return "";

}

void CHuman::SetBaseAnimations(EntityType eType)
{
	switch(eType)
	{

	case ENT_PLAYER:
		{
			SetIdleAnim("Player_Idle");
			m_szWalkingAnim		= "Player_Walk";
			m_szFlinchAnim		= "Player_Flinch";
			m_szFrozenAnim		= "Player_Frozen";
			m_szKnockDownAnim	= "Player_Knocked_Down";
			m_szDeadAnim		= "Player_Dead";
		}
		break;

	case ENT_ENEMY:
		{
			CEnemy* obj =  dynamic_cast<CEnemy*>(this);

			switch(obj->GetEnemyType())
			{

			case CEnemy::Types::Grunt:
				SetIdleAnim("Grunt_Walk_Animation");
				m_szFlinchAnim		=  "Grunt_Flinch_Animation";
				m_szFrozenAnim		=  "Grunt_Frozen_Animation";
				m_szKnockDownAnim	=  "Grunt_Knocked_Down_Animation";
				m_szDeadAnim		=  "Grunt_Death_Animation";
				break;

			case CEnemy::Types::Dummy:
				SetIdleAnim(nullptr);
				m_szFlinchAnim		=  nullptr;
				m_szKnockDownAnim	=  nullptr;
				m_szDeadAnim		=  nullptr;
				break;

			case CEnemy::Types::Fighter:
				SetIdleAnim("Fighter_Walk_Animation");
				m_szFlinchAnim		=	"Fighter_Flinch_Animation";
				m_szFrozenAnim		=	"Fighter_Frozen_Animation";
				m_szKnockDownAnim	=	"Fighter_Knocked_Down_Animation";
				m_szDeadAnim		=	"Fighter_Death_Animation";
				break;
			case CEnemy::Types::Tank:
				SetIdleAnim("Jugg_Walk_Animation");
				m_szFlinchAnim		=  "Jugg_Flinch_Animation";
				m_szFrozenAnim		=  "Jugg_Frozen_Animation";
				m_szKnockDownAnim	=  "Jugg_Knocked_Down_Animation";
				m_szDeadAnim		=  "Jugg_Death_Animation";
				break;

			case CEnemy::Types::EarthBoss:
				SetIdleAnim("Boss1_Walk_Animation");
				m_szFlinchAnim		=  nullptr;
				m_szKnockDownAnim	=  nullptr;
				m_szDeadAnim		=  "Boss1_Death_Animation";
				break;

			case CEnemy::Types::Thrower:
				SetIdleAnim("Thrower_Idle");
				m_szFlinchAnim		=  "Thrower_Flinch_Animation";
				m_szFrozenAnim		=  "Thrower_Frozen_Animation";
				m_szKnockDownAnim	=  "Thrower_Knocked_Down_Animation";
				m_szDeadAnim		=  "Thrower_Dead";
				break;

			case CEnemy::Types::FireBoss:
				SetIdleAnim("FireBoss_Walk_Animation");
				m_szFlinchAnim		=  "FireBoss_Flinch_Animation";
				m_szKnockDownAnim	=  nullptr;
				m_szDeadAnim		=  "FireBoss_Death_Animation";
				break;

			case CEnemy::Types::IceBoss:
				SetIdleAnim("IceBoss_Walk_Animation");
				m_szFlinchAnim		=  "IceBoss_Flinch_Animation";
				m_szKnockDownAnim	=  nullptr;
				m_szDeadAnim		=  "IceBoss_Death_Animation";
				break;

			case CEnemy::Types::WindBoss:
				SetIdleAnim("WindBoss_Walk_Animation");
				m_szFlinchAnim		=  "WindBoss_Flinch_Animation";
				m_szKnockDownAnim	=  nullptr;
				m_szDeadAnim		=  "WindBoss_Death_Animation";
				break;

			}
		}
		break;

	default:
		break;

	}
}
