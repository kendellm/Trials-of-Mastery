#pragma once
#include "Tile.h"
#include <Windows.h>
#include <vector>
#include <tchar.h>
#include <string>
#include "Layer.h"
#include "../CSGD/CSGD_TextureManager.h"
#include <sstream>
#include "../CSGD/CSGD_MessageSystem.h"
#include "../Messages/Create_Enemy_Message.h"

class CPlayer;
struct Map
{
public:
	int tile_Set_ID;
	int m_nTileSetCount;
	int m_nTileSetCollums;
	int m_nTileCount;
	int m_nCollums;
	int m_nTileWidth;
	int m_nTileHeight;
	
	int m_nMessage_Count;
	int m_nSL_Count;
	int m_nCP_Count;	
	int m_nSpawner_Count;
	int m_nTrap_Count;
	int m_nColider_Count;
	
	/* Layers
	0 - Tiles
	1 - Coliders
	2 - Traps
	3 - Spawners
	4 - Checkpoints
	5 - ScreenLocks
	6 - Pop Ups
	*/
	std::vector<CLayer> Layers;
};

class TileManager
{
private:
	Map Current_Map;
	CSGD_TextureManager* m_pTM;
	void CheckSpawners(CPlayer* player);
	void CheckCP(CPlayer* player);
	void CheckScreenLocks(CPlayer* player);
	void CheckPopUps(CPlayer* player);
	void CheckWin(CPlayer* player);
	void CreateTraps();

	TileManager(void);
	~TileManager(void);
	static TileManager	m_Instance;	

	int m_nMapEdge;

public:
	static TileManager* GetInstance();

	void Terminate();

	void LoadLevels(std::string file);
	void CheckTiles(CPlayer* player);
	bool CheckBlockedTiles(float px, float py, float vx, float vy);
	int GetMapEdge(){return m_nMapEdge;}
	void Render();
};

