#include "AudioManager.h"

#include "../CSGD/CSGD_XAudio2.h"
#include "../TinyXML/tinyxml.h"
#include "../CSGD/SGD_Util.h"
#include "../CSGD/CSGD_EventSystem.h"


CAudioManager* CAudioManager::GetInstance( void )
{
	static CAudioManager s_Instance;
	return &s_Instance;
}


CAudioManager::CAudioManager(void)
{
	m_pXA = nullptr;
	m_nPausedid = -1;
	CSGD_EventSystem::GetInstance()->RegisterClient("Play Sound", this);
}


CAudioManager::~CAudioManager(void)
{
}

void CAudioManager::Initialize( std::string audio_xml )
{
	m_pXA = CSGD_XAudio2::GetInstance();

	TiXmlDocument x_doc;
	x_doc.LoadFile( audio_xml.c_str() );
	//assert(x_doc.LoadFile( audio_xml.c_str()) != false && " Font XML file could not be loaded");
	TiXmlElement* x_pRoot = x_doc.RootElement();
	//assert( x_pRoot != nullptr && "XML root is NULL");
	TiXmlElement* x_pBGM = x_pRoot->FirstChildElement("BGM");
	TiXmlElement* x_pXWM = x_pBGM->FirstChildElement("XWM");

	XWM* song;
	std::string file;
	TOSTRINGSTREAM wss;

	while(x_pXWM != nullptr )
	{
		song = new XWM;
		song->file = x_pXWM->Attribute("file");

		wss.str(L"");
		wss << song->file.c_str();
		song->id =  m_pXA->MusicLoadSong( wss.str().c_str() );

		m_vBGM.push_back(song);
		x_pXWM = x_pXWM->NextSiblingElement("XWM");
	}


	TiXmlElement* x_pSFX = x_pBGM->NextSiblingElement("SFX");
	TiXmlElement* x_pWAV = x_pSFX->FirstChildElement("WAV");

	WAV* sound;

	while( x_pWAV != nullptr )
	{
		sound = new WAV;
		sound->file = x_pWAV->Attribute("file");

		wss.str(L"");
		wss << sound->file.c_str();
		sound->id = m_pXA->SFXLoadSound( wss.str().c_str() );

		m_vSFX.push_back(sound);
		x_pWAV = x_pWAV->NextSiblingElement("WAV");
	}
}

void CAudioManager::Terminate( void )
{
	Stop();

	for( int i = m_vSFX.size()-1; i > -1; --i)
	{
		SAFE_DELETE(m_vSFX[i]);
	}
	m_vSFX.clear();

	for( int i = m_vBGM.size()-1; i > -1; --i)
	{
		SAFE_DELETE(m_vBGM[i]);
	}
	m_vBGM.clear();

	m_pXA = nullptr;
}

void CAudioManager::Play( BGM music )
{
	Stop();
	m_pXA->MusicPlaySong(m_vBGM[music]->id, true);
}

void CAudioManager::Play(  SFX sound )
{
	m_pXA->SFXPlaySound(m_vSFX[sound]->id);
}

void CAudioManager::Stop( BGM music )
{
	m_pXA->MusicStopSong(m_vBGM[music]->id);

}

void CAudioManager::Stop( SFX sound )
{
	m_pXA->SFXStopSound(m_vSFX[sound]->id);
}

void CAudioManager::Stop()
{
	m_pXA->SilenceAll();
}

bool CAudioManager::SFXIsPlaying ( SFX sound )
{
	return m_pXA->SFXIsSoundPlaying(m_vSFX[sound]->id);
}

/*virtual*/ void CAudioManager::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetEventID() == "Play Sound")
	{
		Play((SFX)(int)pEvent->GetParam());
	}
}






