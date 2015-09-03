/*
File:		Combo.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CCombo contains the key inputs for the combos
*/

#ifndef COMBO_H
#define COMBO_H

#include <vector>

class CCombo
{
	bool m_bIsPossible;
	char m_szName[30];

	std::vector<int> m_Inputs;

public:
	CCombo(std::vector<int> l, const char * szName);
	~CCombo(void) { m_Inputs.clear(); };

	// Accessors
	bool IsPossible() { return m_bIsPossible; }
	std::vector<int>& GetInputs() { return m_Inputs; }
	const char * CCombo::GetComboName() const { return m_szName; }

	// Mutators
	void SetIsPossible(bool b) { m_bIsPossible = b; }
};

CCombo::CCombo(std::vector<int> l, const char * szName)
{ 
	m_bIsPossible = true;
	m_Inputs = l;
	strcpy_s(m_szName, szName);
}




#endif
