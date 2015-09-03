#include "TileManager.h"
#include "../TinyXML/tinystr.h"
#include "../TinyXML/tinyxml.h"
#include "../objects/Player.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Messages/Create_PopUp_Message.h"
#include "../CSGD/Create_ScreenLock_Message.h"
#include "../CSGD/Create_Trap_Message.h"
#include "../Camera.h"
#include "../GameInfo.h"

TileManager* TileManager::GetInstance()
{
	static TileManager Manager;
	return &Manager;
}

TileManager::TileManager(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();	
}

TileManager::~TileManager(void)
{

}
void TileManager::Terminate()
{
	//right now there is nothing in any layer that needs to be removed specifically.
}
void TileManager::LoadLevels(std::string file)
{
	TiXmlDocument doc;
	doc.LoadFile(file.c_str());
	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pEle = pRoot->FirstChildElement("Dimensions");
	Map m1 = { };
	m1.Layers.resize(7);
	Tiles tile;

	//The Tile informations
	pEle->Attribute("Map_count", &m1.m_nTileCount);
	pEle->Attribute("Map_collums", &m1.m_nCollums);
	pEle->Attribute("tile_width", &m1.m_nTileWidth);
	pEle->Attribute("tile_height", &m1.m_nTileHeight);
	pEle->Attribute("T_Set_count", &m1.m_nTileSetCount);
	pEle->Attribute("T_Set_collums", &m1.m_nTileSetCollums);
	const char* pText;
	pText = pEle->Attribute("file_path");

	//Convert path to a TChar *
	std::string temp1 = "resource/";
	std::string temp2 = pText;
	std::string temp = temp1 + temp2;
	std::wstringstream wss;
	wss << temp.c_str();

	//Load Texture from xml
	if(pText != nullptr)
		m1.tile_Set_ID = m_pTM->LoadTexture(wss.str().c_str());
	pRoot = pEle->NextSiblingElement("Tiles");
	pEle = pRoot->FirstChildElement("Tile");


	//Loading in the tiles
	for(int i = 0; i < m1.m_nTileCount; i++)
	{
		pEle->Attribute("ID", &tile.type);
		tile.x = i % m1.m_nCollums;
		tile.y = i / m1.m_nCollums;
		m1.Layers[0].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement();
	}

	//The Layers information
	pRoot = pRoot->NextSiblingElement("Layers");
	pRoot->Attribute("colide_count", &m1.m_nColider_Count);
	pRoot->Attribute("trap_count", &m1.m_nTrap_Count);
	pRoot->Attribute("spawn_count", &m1.m_nSpawner_Count);
	pRoot->Attribute("CP_count", &m1.m_nCP_Count); //Check points
	pRoot->Attribute("SL_count", &m1.m_nSL_Count); //Screen Locks
	pRoot->Attribute("Message_count", &m1.m_nMessage_Count); //Pop-ups

	//Loading in the Colliders
	pRoot = pRoot->NextSiblingElement("Coliders");
	pEle = pRoot->FirstChildElement("Colide");
	for(int i = 0; i < m1.m_nColider_Count; i++)
	{
		pEle->Attribute("col", &tile.x);
		pEle->Attribute("row", &tile.y );
		m1.Layers[1].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement("Colide");
	}

	//Loading in the Traps
	pRoot = pRoot->NextSiblingElement("Traps");
	pEle = pRoot->FirstChildElement("Trap");
	for(int i = 0; i < m1.m_nTrap_Count; i++)
	{
		pEle->Attribute("col", &tile.x);
		pEle->Attribute("row", &tile.y);
		pEle->Attribute("type", &tile.type);
		m1.Layers[2].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement("Trap");
	}

	//Loading in the Spawners
	pRoot = pRoot->NextSiblingElement("Spawners");
	pEle = pRoot->FirstChildElement("Spawn");
	for(int i = 0; i < m1.m_nSpawner_Count; i++)
	{
		pEle->Attribute("col", &tile.x);
		pEle->Attribute("row", &tile.y);
		pEle->Attribute("type", &tile.type);
		m1.Layers[3].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement("Spawn");
	}

	//Loading in the CheckPoints
	pRoot = pRoot->NextSiblingElement("Checkpoints");
	pEle = pRoot->FirstChildElement("Checkpoint");
	for(int i = 0; i < m1.m_nCP_Count; i++)
	{
		pEle->Attribute("col", &tile.x);
		pEle->Attribute("row", &tile.y);
		m1.Layers[4].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement("Checkpoint");
	}

	//Loading in the ScreenLocks
	pRoot = pRoot->NextSiblingElement("Screen_Locks");
	pEle = pRoot->FirstChildElement("Lock");
	TiXmlElement* pSec_Ele;
	TiXmlElement* pThird_Ele;
	int size = 0;
	int type = 0;
	std::vector<int> w;
	for(int i = 0; i < m1.m_nSL_Count; i++)
	{
		pEle->Attribute("col", &tile.x);
		pEle->Attribute("row", &tile.y);
		pEle->Attribute("wave_count", &tile.wave_count);
		pSec_Ele = pEle->FirstChildElement("Wave");
		for(int j = 0; j < tile.wave_count; j++)
		{
			pSec_Ele->Attribute("size", &size);
			pThird_Ele = pSec_Ele->FirstChildElement("Member");
			for(int k = 0; k < size; k++)
			{
				pThird_Ele->Attribute("type", &type);
				w.push_back(type);
				pThird_Ele = pThird_Ele->NextSiblingElement("Member");
			}
			tile.Waves.push_back(w);
			w.clear();
			pSec_Ele = pSec_Ele->NextSiblingElement("Wave");
		}
		m1.Layers[5].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement("Lock");
	}
	pSec_Ele = nullptr;
	pThird_Ele = nullptr;

	//Loading the Pop_Ups
	pRoot = pRoot->NextSiblingElement("Messages");
	pEle = pRoot->FirstChildElement("Message");
	for(int i = 0; i < m1.m_nMessage_Count; i++)
	{
		pEle->Attribute("col", &tile.x);
		pEle->Attribute("row", &tile.y);
		pEle->Attribute("type", &tile.type);
		m1.Layers[6].Layer_Content.push_back(tile);
		tile.Reset();
		pEle = pEle->NextSiblingElement("Message");
	}
	
	Current_Map = m1;
	m_nMapEdge = Current_Map.m_nCollums * Current_Map.m_nTileWidth;

	CreateTraps();
}
void TileManager::CreateTraps()
{
	CLayer Layer = Current_Map.Layers[2];
	float topTile = (float)((Current_Map.m_nTileCount / Current_Map.m_nCollums - 1) * Current_Map.m_nTileHeight);
	float h = 600; //Camera Height
	for(int i = 0; i < Current_Map.m_nTrap_Count; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x *Current_Map.m_nTileWidth);
		float tileY = h- topTile + (float)(Layer.Layer_Content[i].y * Current_Map.m_nTileHeight);
		Create_Trap_Message* Temp = new Create_Trap_Message(Layer.Layer_Content[i].type, tileX, tileY);
		CSGD_MessageSystem::GetInstance()->SendMsg(Temp);
		Temp = nullptr;
		Layer.Layer_Content.erase(Layer.Layer_Content.begin()+i);
		Current_Map.m_nTrap_Count--;
		i--;
	}
	Current_Map.Layers[2] = Layer;
}
void TileManager::CheckTiles(CPlayer* player)
{

	CheckSpawners(player);
	CheckCP(player);
	CheckScreenLocks(player);
	CheckPopUps(player);
	CheckWin(player);
}
bool TileManager::CheckBlockedTiles(float px, float py, float vx, float vy) //returns true if player is allowed to move, false if that is a blocked tile
{
	CLayer Layer = Current_Map.Layers[1];
	float topTile = (float)((Current_Map.m_nTileCount / Current_Map.m_nCollums - 1) * Current_Map.m_nTileHeight);
	float h = 600; //Camera Height
	for(int i = 0; i < Current_Map.m_nColider_Count; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x *Current_Map.m_nTileWidth);
		float tileY = h - topTile + (float)(Layer.Layer_Content[i].y * Current_Map.m_nTileHeight);
		if(tileX <= px + vx && tileX + Current_Map.m_nTileWidth >= px + vx )
		{
			if(tileY <= py + vy && tileY + Current_Map.m_nTileHeight >= py + vy)
			{
				return false;
			}
		}
	}
	return true;
}

void TileManager::CheckWin(CPlayer* player)
{
	float tileX = (float)(Current_Map.m_nCollums * Current_Map.m_nTileWidth);
	if(tileX <= player->GetPosX())
	{
		bool bWin = true;
		CSGD_EventSystem::GetInstance()->SendEventNow( "GAME_OVER", &bWin );
	}
}
void TileManager::CheckSpawners(CPlayer* player)
{
	//Spawner Layer Maps[#].Layer[3];
	CLayer Layer = Current_Map.Layers[3];
	for(int i = 0; i < Current_Map.m_nSpawner_Count; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x * Current_Map.m_nTileWidth);
		if(tileX <= player->GetPosX())
		{
			Create_Enemy_Message* CE_Message = new Create_Enemy_Message(Layer.Layer_Content[i].type, player);
			CSGD_MessageSystem::GetInstance()->SendMsg(CE_Message);
			CE_Message = nullptr;
			//to remove this spawner so it does not trigger again
			Layer.Layer_Content.erase(Layer.Layer_Content.begin() + i, Layer.Layer_Content.begin() + i + 1);
			Current_Map.m_nSpawner_Count--;
			i--;
		}
	}
	Current_Map.Layers[3] = Layer;
}
void TileManager::CheckCP(CPlayer* player)
{
	CLayer Layer = Current_Map.Layers[4];
	for(int i = 0; i < Current_Map.m_nCP_Count; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x * Current_Map.m_nTileWidth);
		if(tileX <= player->GetPosX())
		{
			//Set the player's Checkpoint to this Checkpoint's X & Y * tile sizes
			player->SetCheckPoint(player->GetPosX(), player->GetPosY());
			//to remove this Checkpoint so it does not trigger and move the CP if the player backtracks
			Layer.Layer_Content.erase(Layer.Layer_Content.begin()+i);
			Current_Map.m_nCP_Count--;
			i--;
		}
	}
	Current_Map.Layers[4] = Layer;
}
void TileManager::CheckScreenLocks(CPlayer* player)
{
	CLayer Layer = Current_Map.Layers[5];

	for(int i = 0; i < Current_Map.m_nSL_Count; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x * Current_Map.m_nTileWidth);
		if(tileX <= player->GetPosX())
		{
			//Create a Screen Lock event
			//Sending the Waves: Layer.Layer_Content[i].Waves
			//And if needed Wave Count
			CCreate_ScreenLock_Message* CE_Message = new CCreate_ScreenLock_Message(Layer.Layer_Content[i].Waves);
			CSGD_MessageSystem::GetInstance()->SendMsg(CE_Message);
			CE_Message = nullptr;
			Layer.Layer_Content[i].Waves;
			//to remove this Screen lock so it does not trigger again
			Layer.Layer_Content.erase(Layer.Layer_Content.begin()+i);
			Current_Map.m_nSL_Count--;
			i--;
		}
	}
	Current_Map.Layers[5] = Layer;
}
void TileManager::CheckPopUps(CPlayer* player)
{
	CLayer Layer = Current_Map.Layers[6];

	for(int i = 0; i < Current_Map.m_nMessage_Count; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x * Current_Map.m_nTileWidth);
		if(tileX <= player->GetPosX())
		{
			Create_PopUp_Message* CPopUp_Message = new Create_PopUp_Message(Layer.Layer_Content[i].type);
			CSGD_MessageSystem::GetInstance()->SendMsg(CPopUp_Message);
			CPopUp_Message = nullptr;
			std::vector<bool> temp = player->GetPopUps();
			temp[Layer.Layer_Content[i].type] = true;
			player->SetPopUps(temp);

			//to remove this pop_up so it does not trigger again
			Layer.Layer_Content.erase(Layer.Layer_Content.begin()+i);
			Current_Map.m_nMessage_Count--;
			i--;
		}
	}
	Current_Map.Layers[6] = Layer;
}

void TileManager::Render()
{
	CCamera* Game_Camera = CCamera::GetInstance();
	//Get the Camera's Top left corner as x & y
	float x = Game_Camera->GetPosX();
	float y = Game_Camera->GetPosY();
	//Get Camera width and height
	float w = Game_Camera->GetWidth();
	float h = Game_Camera->GetHeight();
	//Tile Layer Maps[#].Layer[0];
	CLayer Layer = Current_Map.Layers[0];
	int rows = Current_Map.m_nTileCount / Current_Map.m_nCollums;
	int temp = (int)(x / Current_Map.m_nTileWidth);
	for(int i = temp; i < Current_Map.m_nTileCount; i++)
	{
		float tileX = (float)(Layer.Layer_Content[i].x * Current_Map.m_nTileWidth);
		float tileY = (float)(Layer.Layer_Content[i].y * Current_Map.m_nTileHeight);

		float topTile = (float)((Current_Map.m_nTileCount / Current_Map.m_nCollums - 1) * Current_Map.m_nTileHeight);
		if(tileX >= x - Current_Map.m_nTileWidth && tileX <= x + w)
		{
			if(Layer.Layer_Content[i].type < Current_Map.m_nTileSetCount && Layer.Layer_Content[i].type >= 0 )
			{
				RECT r;
				r.left = Layer.Layer_Content[i].type%Current_Map.m_nTileSetCollums * Current_Map.m_nTileWidth;
				r.right = r.left + Current_Map.m_nTileWidth;
				r.top = Layer.Layer_Content[i].type/Current_Map.m_nTileSetCollums * Current_Map.m_nTileHeight;
				r.bottom = r.top + Current_Map.m_nTileHeight;
				m_pTM->Draw(Current_Map.tile_Set_ID, (int)(tileX - Game_Camera->GetPosX()), int(h - topTile + tileY - Game_Camera->GetPosY()), 1.0f, 1.0f, &r);
			}
		}
		else
		{

		}
	}
}