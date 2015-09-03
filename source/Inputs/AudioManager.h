#pragma once

#include "../CSGD/SGD_String.h"
#include "../CSGD/IListener.h"

#include <vector>

enum BGM {MAIN, HOME, SHOP, DOJO, MOUNTAIN, CREDITS, BOSS};

enum SFX {
	CURSOR, DENIED, LIGHT_ATK, HEAVY_ATK, COL_1, COL_2, COL_3,
	FIVE_COMBO_SUCCESS, FOUR_COMBO_SUCCESS, FIREBALL_IMPACT, FB_EXPL_1, 
	FB_EXPL_2, FB_EXPL_3, FB_EXPL_START, IB_FREEZE, JUMP_LAND, LARGE_FIREBALL_SPAWN,
	NEW_GAME, PLAYER_FIREBALL_SPAWN, RETREATING_KICK_SOUND, SHOP_UPGRADE, WB_BOSS_PUSH,
	SWING_PUNCH_L, SWING_PUNCH_M, SWING_PUNCH_H, SWING_KICK_L, SWING_KICK_M, SWING_KICK_H,
	SWOOSH, HURRICANE_KICK,

};


class CSGD_XAudio2;

class CAudioManager : public IListener
{
public:
	struct XWM
	{
		int id;
		std::string file;
		bool loop;
	};
	struct WAV
	{
		int id;
		std::string file;
		bool loop;
	};

	static CAudioManager* GetInstance( void );

	// IListener Interface
	virtual void HandleEvent(CEvent* pEvent)	override;

	//void Update		(float fElaspedTime);
	void Initialize ( std::string audio_xml );
	void Terminate	( void );
	void Play	( BGM music );
	void Play	( SFX sound );
	bool SFXIsPlaying ( SFX sound );
	void Stop	( BGM music );
	void Stop	( SFX sound );
	void Stop	();


private:

	CAudioManager						( void );
	CAudioManager						( const CAudioManager& );
	virtual      ~CAudioManager		( void );
	CAudioManager&  operator=			( const CAudioManager& );

	int					m_nPausedid;
	CSGD_XAudio2*		m_pXA;
	std::vector<XWM*>	m_vBGM;
	std::vector<WAV*>	m_vSFX;
};