#pragma once
#include "../CSGD/SGD_String.h"
#include <vector>
class GameInfo;
class CItemManager
{
private:

	struct SItem
	{
		std::string szName	 ; 
		int			m_nLevel ;   
		int			m_nPrice ;
	};

public:

	std::string  Name		  ( void )				{ return m_sCurItem->szName  ;}
	int          Level		  ( void )				{ return m_sCurItem->m_nLevel;}
	int          Price		  ( void )				{ return m_sCurItem->m_nPrice;}
	int          EXP		  ( void )			    { return m_nExpPts;}

	void	     Name		  ( std::string name )	{m_sCurItem->szName   = name  ;}
	void	     Level		  ( int nLvl	 )		{m_sCurItem->m_nLevel = nLvl  ;}
	void	     Price        ( int nPrice	 )		{m_sCurItem->m_nPrice = nPrice;}

	void         Initialize   ( std::string filepath, GameInfo* pInfo  );
	void         Release      ( void );
	void         Buy		  ( int type );
	bool         PriceCheck   ( int type);
	void         Item		  ( int nItem );
	//SItem*      GetItem		  ( void )	 {return m_sCurItem;}


	CItemManager(void);
	~CItemManager(void);

private:

	int					m_nExpPts;
	SItem*				m_sCurItem  ;
	std::vector<SItem*> m_vItems	;

};

