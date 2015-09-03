/*
File:		Iceball.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CIceball creates an iceball for the ice boss
*/

#ifndef ICEBALL_H
#define ICEBALL_H


#include "projectile.h"

class CIceball : public CProjectile
{
public:
	CIceball();
	virtual ~CIceball(void);

	virtual void Update(float fElapsedTime)		override;
	virtual void HandleEvent(CEvent* pEvent)	override;
};



#endif 