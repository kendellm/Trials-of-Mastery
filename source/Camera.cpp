#include "Camera.h"
#include "objects\Entity.h"
#include "TileSystem\TileManager.h"
CCamera* CCamera::GetInstance( void )
{
	static CCamera s_Instance;
	return &s_Instance;
}
CCamera::CCamera(void)
{
	PosX = 0.0f;
	PosY = 0.0f;
	Height = 0.0f;
	Width = 0.0f;
	Locked = false;
}
CCamera::~CCamera(void)
{
}
void CCamera::Update(CEntity* Player)
{
	if(!Locked)
	{
		TileManager* m_pTileM = TileManager::GetInstance();
		if(Player->GetPosX() > GetPosX() + GetWidth() - 300)
		{
			ChangePosX(Player->GetPosX() + 300 - GetPosX() - GetWidth() );
			if(GetPosX() + GetWidth() > m_pTileM->GetMapEdge())
				SetPosX(m_pTileM->GetMapEdge() - GetWidth());
		}
		if(Player->GetPosX() < GetPosX() + 200)
		{
			ChangePosX(Player->GetPosX() - GetPosX() - 200);
			if(GetPosX() < 0)
				SetPosX(0);
		}
	}
}