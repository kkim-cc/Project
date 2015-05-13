/*
* FILE : ShootControl.h
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-28
* DESCRIPTION: interface of the ShootControl class
*/

#include "ShootModel.h"

#pragma once
class ShootControl
{
private:
	ShootModel *m_pModel;
public:
	ShootControl(ShootModel *pModel);
	~ShootControl();
	// implements
	BOOL ProcessShoot(CPoint pt);
	void SetHit();
};

