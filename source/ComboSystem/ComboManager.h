/*
File:		ComboManager.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	ComboManager class keeps track of the currently possible combos
*/

#ifndef COMBO_MANAGER_H
#define COMBO_MANAGER_H

#include <vector>
#include <string>

class CCombo;
class CPlayer;
class CAudioManager;


// success, hit, fail
enum COMBO_RESULT { CR_SUCCESS, CR_HIT, CR_FAIL, };

class ComboManager
{

public:
	ComboManager(void);
	~ComboManager(void);

	bool HandleInput(CPlayer* pPlayer, int nKeyHit, int& nDepth, const char * szAttack);


	void ResetPossibleCombos();

private:

	int						m_nPrevKey;
	const char *			m_szComboName;
	CAudioManager*			m_pAudio;
	std::vector<CCombo>		allCombos;

	void			LoadComboData	(const char* szFilename);
	const char *	LookForCombo	(std::vector<int> vec);
	COMBO_RESULT	Compare			(int nKey, int nDepth);


};


#endif
