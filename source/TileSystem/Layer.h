#pragma once
#include <Windows.h>
#include <vector>
#include <tchar.h>
#include <string>

class Tiles //generic name
{
public:
	int x; //used by Coliders, Checkpoints, Traps, Spawners, Screenlocks, and Pop_ups
	int y;
	int type; // for traps and spawners
	int wave_count; //for screenlocks
	std::vector<std::vector<int>> Waves; // for screen locks
	std::string filepath; //for  popups
	void Reset()
	{
		x = y = type = wave_count = -1;
		filepath = "";
		Waves.clear();
	}
};
class CLayer
{
private:
	int count;
	
public:
	std::vector<Tiles> Layer_Content;

	CLayer(void);
	~CLayer(void);

	void SetCount(int c){count = c;}
	const int GetCount(){return count;}
};

