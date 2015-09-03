#pragma once

#include "IGamestate.h"

class CComingSoonState : public IGameState
{
public:


	static CComingSoonState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;

private:

	CComingSoonState						( void );
	CComingSoonState						( const CComingSoonState& );
	virtual         ~CComingSoonState		( void );
	CComingSoonState&   operator=			( const CComingSoonState& );

};
