#include "Perform_SubZero_Special_MSG.h"


CPerform_SubZero_Special_MSG::CPerform_SubZero_Special_MSG(float x, float y, int level, int owner):CMessage(MSG_PERFORM_SUBZERO)
{
	m_nPosX = x;
	m_nPosY = y;
	m_nLevel = level;
	m_nOwner = owner;
}


CPerform_SubZero_Special_MSG::~CPerform_SubZero_Special_MSG(void)
{

}
