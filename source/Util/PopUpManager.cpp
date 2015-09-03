#include "PopUpManager.h"
#include "../objects/Popup_DashingPunch.h"
#include "../objects/PopUp_SPGain.h"
#include "../objects/PopUp_Fireball_special.h"
#include "../objects/PopUp_RetreatingKick.h"


#include "../TinyXML/tinyxml.h"
#include "../CSGD/SGD_String.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/SGD_Util.h"

#include "../Game.h"

#include "../objects/PopUp.h"
#include "../CSGD/CSGD_EventSystem.h"

CPopUpManager* CPopUpManager::GetInstance()
{
	static CPopUpManager Manager;
	return &Manager;
}

CPopUpManager::CPopUpManager(void)
{
	m_pCurrPopUp = nullptr;
	m_nSFXid	 = -1;
	Register_Completed();
}
CPopUpManager::~CPopUpManager(void)
{
}

void CPopUpManager::Register_Completed()
{
	CSGD_EventSystem::GetInstance()->RegisterClient("Pop_Up_Completed", this);
}


void CPopUpManager::Initialize( std::string filepath )
{
	TiXmlDocument x_doc;
	x_doc.LoadFile( filepath.c_str());

	TiXmlElement* x_pRoot = x_doc.RootElement();

	SPopUp* pPopUp;
	TiXmlElement* x_PopUp = x_pRoot->FirstChildElement("PopUp");

	std::string file;

	while(x_PopUp != nullptr)
	{
		pPopUp = new SPopUp;
		file = x_PopUp->Attribute("file");
		x_PopUp->Attribute("type", &pPopUp->m_nType );
		x_PopUp->Attribute("posX", &pPopUp->m_nPosX );
		x_PopUp->Attribute("posY", &pPopUp->m_nPosY );
		x_PopUp->Attribute("width", &pPopUp->m_nWidth );
		x_PopUp->Attribute("height", &pPopUp->m_nHeight );

		TOSTRINGSTREAM wss;
		wss.str(L"");
		wss << file.c_str();
		pPopUp->m_nImageid = CSGD_TextureManager::GetInstance()->LoadTexture(wss.str().c_str());

		m_vPopUps.push_back(pPopUp);

		x_PopUp = x_PopUp->NextSiblingElement("PopUp");
	}

}

void CPopUpManager::Shutdown( void )
{
	for(unsigned int i = 0; i < m_vPopUps.size(); ++i)
	{
			SAFE_DELETE(m_vPopUps[i]);
	}
	m_vPopUps.clear();

	for(unsigned int i = 0; i < m_vAniPopUps.size(); ++i)
	{
		SAFE_DELETE(m_vAniPopUps[i]);
	}
	m_vAniPopUps.clear();

	m_pCurrPopUp = nullptr;
	m_pAnimPopUp = nullptr;
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CPopUpManager::Update( float fElaspedTime )
{
	if(m_pAnimPopUp != nullptr)
		m_pAnimPopUp->Update(fElaspedTime);
}

void CPopUpManager::Render( void )
{
	if(m_pAnimPopUp != nullptr)
	{
		m_pAnimPopUp->Render();
	}
}


void CPopUpManager::Create( int _type )
{
	m_pCurrPopUp = m_vPopUps[_type];
}

void CPopUpManager::Destroy( void )
{

	SAFE_DELETE(m_pAnimPopUp);

}


void CPopUpManager::HandleEvent( CEvent* pEvent )
{
	if (pEvent->GetEventID() == "Pop_Up_Completed")
	{
		Destroy();
	}
}

void CPopUpManager::Load( unsigned int nType  )
{
	switch (nType)
	{
	case CPopUp::DASHING_PUNCH:
		{
			m_pAnimPopUp = new CPopUp_DashingPunch;
		}
		break;
	case CPopUp::SP_GAIN:
		{
			m_pAnimPopUp = new CPopUp_SPGain;
		}
		break;
	case CPopUp::FIREBALL_SPECIAL:
		{
			m_pAnimPopUp = new CPopUp_Fireball_special;

		}
		break;
	case CPopUp::RETREATING_KICK:
		{
			m_pAnimPopUp = new CPopUp_RetreatingKick;

		}
		break;
	default:
		break;
	}

}
