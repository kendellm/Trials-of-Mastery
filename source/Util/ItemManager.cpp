#include "ItemManager.h"
#include "../TinyXML/tinyxml.h"
#include "../GameInfo.h"
#include "../Game.h"
#include "../CSGD/SGD_Util.h"
CItemManager::CItemManager(void)
{
	m_nExpPts = 0;
}


CItemManager::~CItemManager(void)
{
	Release();
}

void CItemManager::Initialize( std::string filepath, GameInfo* pInfo  )
{
	Release();
	TiXmlDocument x_doc;
	x_doc.LoadFile( filepath.c_str());

	TiXmlElement* x_pRoot = x_doc.RootElement();

	m_nExpPts = pInfo->EXP();
	int HPlv  = pInfo->Health();
	int SPlv  = pInfo->Special();
	int lifLv = pInfo->Lives();
	int frLv  = pInfo->Fire();
	int icLv  = pInfo->Ice();
	int earLv = pInfo->Earth();
	int WndLv = pInfo->Wind();

	int nLevel = 0;

#pragma region HEALTH

	TiXmlElement* x_pHealth = x_pRoot->FirstChildElement("Health");
	TiXmlElement* x_pItem = x_pHealth->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(HPlv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	SItem* pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

#pragma region SPECIAL


	TiXmlElement* x_pSpecial = x_pHealth->NextSiblingElement("Special");
	x_pItem = x_pSpecial->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(SPlv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

#pragma region LIVES


	TiXmlElement* x_pLives = x_pSpecial->NextSiblingElement("Lives");
	x_pItem = x_pLives->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(SPlv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

#pragma region FIRE

	TiXmlElement* x_pFire = x_pLives->NextSiblingElement("Fire");
	x_pItem = x_pFire->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(frLv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

#pragma region WIND

	TiXmlElement* x_pWind = x_pFire->NextSiblingElement("Wind");
	x_pItem = x_pWind->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(WndLv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

#pragma region ICE

	TiXmlElement* x_pIce = x_pWind->NextSiblingElement("Ice");
	x_pItem = x_pIce->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(icLv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

#pragma region EARTH

	TiXmlElement* x_pEarth = x_pIce->NextSiblingElement("Earth");
	x_pItem = x_pEarth->FirstChildElement("item");
	x_pItem->Attribute("level", &nLevel);

	while( nLevel != ( min(earLv + 1, 3) ) )
	{
		x_pItem = x_pItem->NextSiblingElement("item");
		nLevel++;
	}
	pItem = new SItem;
	pItem->szName = x_pItem->Attribute("name");
	x_pItem->Attribute("level", &pItem->m_nLevel);
	x_pItem->Attribute("price", &pItem->m_nPrice);

	m_vItems.push_back(pItem);

#pragma endregion

}

void CItemManager::Release( void )
{
	for(unsigned int i = 0; i < m_vItems.size();++i)
	{
		SAFE_DELETE(m_vItems[i]);
	}
	m_vItems.clear();
}


void CItemManager::Buy( int type )
{
	GameInfo* pInfo = CGame::GetInstance()->GetProfile();

	pInfo->Upgrade(type);
	m_nExpPts  = max( m_nExpPts - m_vItems[type]->m_nPrice, 0);
	pInfo->EXP(m_nExpPts);
	Initialize("config/shop.xml", pInfo);
	
}

void CItemManager::Item( int nItem )
{
	m_sCurItem = m_vItems[nItem];
}

bool CItemManager::PriceCheck( int type )
{
	if( CGame::GetInstance()->GetProfile()->CheckGrade(type , m_vItems[type]->m_nLevel ) )
		return ( m_vItems[type]->m_nPrice > m_nExpPts ) ? false : true;
	else 
		return false;
}
