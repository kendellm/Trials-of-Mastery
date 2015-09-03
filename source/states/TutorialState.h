#pragma once
#include "IGamestate.h"
class CTutorialState : public IGameState
{
public:


	static CTutorialState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
			void SetInstruction(int nType);

private:

	CTutorialState						( void );
	CTutorialState						( const CTutorialState& );
	virtual         ~CTutorialState		( void );
	CTutorialState&   operator=			( const CTutorialState& );


	int			m_nCurrInstrut;
	int			m_aInstruction[6];
};


