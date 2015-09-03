#pragma once

#include "Message.h"
#include <vector>


class CCreate_ScreenLock_Message : public CMessage
{
private:
	std::vector<std::vector<int>> Waves;

public:
	CCreate_ScreenLock_Message(std::vector<std::vector<int>> waves);
	~CCreate_ScreenLock_Message(void);

	std::vector<std::vector<int>> GetWaves() {return Waves;}
};

