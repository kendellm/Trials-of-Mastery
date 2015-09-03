/*
File:		ComboManager.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	ComboManager class keeps track of the currently possible combos
*/

#include "ComboManager.h"
#include "Combo.h"

#include "../objects/Player.h"

#include "../TinyXML/tinyxml.h"
#include "../Inputs/AudioManager.h"

#include <vector>
#include <string>
using namespace std;

#define state CEntityState::ENTITY_STATE


ComboManager::ComboManager(void)
{
	LoadComboData("config/combodata.xml");

	m_nPrevKey = -1;

	m_szComboName = nullptr;

	m_pAudio = CAudioManager::GetInstance();


}


ComboManager::~ComboManager(void)
{
	allCombos.clear();
}

void ComboManager::LoadComboData(const char* szFilename)
{
	TiXmlDocument doc;

	if (doc.LoadFile(szFilename) == false)
		return;

	TiXmlElement* pRoot = doc.RootElement();

	if (pRoot == nullptr)
		return;

	TiXmlElement* pCombo = pRoot->FirstChildElement("Combo");

	while (pCombo != nullptr)
	{
		string ComboName = pCombo->Attribute("name");

		int length;
		pCombo->Attribute("length", &length);


		string pKeys = pCombo->GetText();
		vector<int> keys;
		if (pKeys != "")
		{
			for (int i = 0; i < length; ++i)
			{
				if (pKeys[i] == 49)
					keys.push_back(1);
				else if (pKeys[i] == 50)
					keys.push_back(2);
				else
					keys.push_back(3);
			}
		}

		allCombos.push_back(CCombo(keys, ComboName.c_str()));

		pCombo = pCombo->NextSiblingElement();
	}

}

bool ComboManager::HandleInput( CPlayer* pPlayer, int nKeyHit, int& nDepth, const char * szAttack)
{

	COMBO_RESULT cr = Compare(nKeyHit, nDepth);

	switch (cr)
	{
	case CR_SUCCESS:
		{
			ResetPossibleCombos();
			nDepth = 0;
			pPlayer->GetButtonPresses().clear();

			// Queue the combo animation
				pPlayer->GetAnimInfo()->QueueAnimation(m_szComboName);

			// Reset the combo name
			m_szComboName = nullptr;

			// Set the timers and the bools
			pPlayer->SetSuccessfulCombo(true);
			pPlayer->SetComboTimers(1.0f, -1.0f, 0.0f);
			return false;
		}
		break;
	case CR_HIT:
		{
			// Check the prev key
			if (m_nPrevKey != -1)
			{
				switch(m_nPrevKey)
				{
					// ------------------------------------
				case 1:
					{
						switch (nKeyHit)
						{

						case 1:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Double_Punch");
							break;
						case 2:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Roundhouse");
							break;

						case 3:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Double_Kick");	
							break;

						}
					}
					break;

				case 2:
					{
						switch (nKeyHit)
						{
						case 1:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_High_Kick");
							break;
						case 2:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Launching_Kick");
							break;
						case 3:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Sliding_Kick");
							break;
						}
					}
					break;

				case 3:
					{
						switch (nKeyHit)
						{
						case 1:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Jump_Kick");
							break;
						case 2:
							pPlayer->GetAnimInfo()->QueueAnimation("Player_Sliding_Kick");
							break;
						}
					}
					break;
					// --------------------------------------
				}
			}
			else
			{
				pPlayer->GetAnimInfo()->QueueAnimation(szAttack);

				if (szAttack == "Player_Light_Punch")
					m_pAudio->Play(SWING_PUNCH_L);
				else if (szAttack == "Player_Medium_Kick")
					m_pAudio->Play(SWING_KICK_M);
				else
					m_pAudio->Play(SWING_KICK_H);

			}

			m_nPrevKey = nKeyHit;
			nDepth += 1;
			pPlayer->SetInputTimer(0.5f);
			return true;
		}
		break;
	case CR_FAIL:
		{
			if (m_nPrevKey == 1 && nKeyHit == 1)
			{
				pPlayer->GetAnimInfo()->QueueAnimation("Player_Double_Punch_Fail");
			}

			m_nPrevKey = -1;
			nDepth = 0;
			pPlayer->GetButtonPresses().clear();
			pPlayer->SetInputTimer(0.0f);
			pPlayer->SetComboFailed(true);
			ResetPossibleCombos();
			return false;
		}
		break;
	}

	return false;
}



COMBO_RESULT ComboManager::Compare(int nKey, int nDepth)
{
	COMBO_RESULT result = CR_FAIL;

	// Get the current possible combos with this key at the current depth
	for (unsigned int i = 0; i < allCombos.size(); ++i)
	{
		if (allCombos[i].IsPossible())
		{
			if (allCombos[i].GetInputs()[nDepth] == nKey
				&& nDepth == (allCombos[i].GetInputs().size() - 1))
			{
				result = CR_SUCCESS;
				m_szComboName = allCombos[i].GetComboName();
			}
			else if (allCombos[i].GetInputs()[nDepth] == nKey)
			{
				result = CR_HIT;
			}
			else
			{
				allCombos[i].SetIsPossible(false);
			}
		}

	}

	//if (result == CR_FAIL)
	//{
	//	ResetPossibleCombos();
	//}

	return result;
}

const char * ComboManager::LookForCombo(std::vector<int> vec)
{
	for (unsigned int i = 0; i < allCombos.size(); ++i)
	{
		if (allCombos[i].IsPossible() == true)
		{
			for (unsigned int k = 0; i < allCombos[i].GetInputs().size(); ++k)
			{
				if (allCombos[i].GetInputs()[k] != vec[k])
					return "";
			}
			return allCombos[i].GetComboName();	
		}
	}
	return false;
}

void ComboManager::ResetPossibleCombos()
{
	for (unsigned int i = 0; i < allCombos.size(); ++i)
	{
		allCombos[i].SetIsPossible(true);
	}

	m_nPrevKey = -1;
}

