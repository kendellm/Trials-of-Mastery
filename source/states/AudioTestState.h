#pragma once
#include "IGamestate.h"
class CAudioManager;
class CAudioTestState : public IGameState
{
public:


	static CAudioTestState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;

private:

	CAudioTestState						( void );
	CAudioTestState						( const CAudioTestState& );
	virtual         ~CAudioTestState		( void );
	CAudioTestState&   operator=			( const CAudioTestState& );

	CAudioManager* m_pAUM;
};
