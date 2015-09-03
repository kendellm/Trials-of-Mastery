#pragma once
#include "Entity.h"

class CTrap : public CEntity
{
public:
	
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual void HandleEvent(CEvent* pEvent)			override;
	virtual void HandleActiveRectCollision( const IEntity* pOther)   override;

	void SetTrapType(int t);
	int GetTrapType( void ) const {return Trap_Type;}


	CTrap(void);
	~CTrap(void);

	enum Trap_Types{Pit_Fall, Wrecking_Ball, Conveyor_Up, Conveyor_Down, Conveyor_Left, Conveyor_Right, Rock_Slide};

private:
	int Trap_Type;
	float m_fTime;
};