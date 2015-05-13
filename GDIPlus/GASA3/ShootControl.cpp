/*
* FILE : ShootControl.h
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-28
* DESCRIPTION: implementation of the ShootControl class
*/
#include "stdafx.h"
#include "ShootControl.h"


ShootControl::ShootControl(ShootModel *pModel)
{
	m_pModel = pModel;
}

ShootControl::~ShootControl()
{
}

// implements
/*
* FUNCTION : ProcessShoot
*
* DESCRIPTION : Call the HitTest function of ShootModel class.
*
* PARAMETERS : CPoint pt : LButton Douwn Position(X, Y)
*
* RETURNS : BOOL TRUE: Hit, FALSE: Miss
*/
BOOL ShootControl::ProcessShoot(CPoint pt)
{
	if (m_pModel == NULL)
		return FALSE;
	return m_pModel->HitTest(pt);
}


/*
* FUNCTION : HitTest
*
* DESCRIPTION : call the SetHit functions of ShootModel class
*
* PARAMETERS :	nothing
*
* RETURNS : nothing
*/
void ShootControl::SetHit()
{
	if (m_pModel != NULL)
	{
		m_pModel->SetHit();
	}
}