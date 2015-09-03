/*************************************************************
	File:			GameInfo.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Contains main gameplay attributes for player
*************************************************************/
#pragma once

class GameInfo
{
public:

	enum UPGRADES {HEALTH, SPECIAL, LIVES, FIRE, WIND, ICE, EARTH };
	GameInfo(void);
	GameInfo(
	int level, int Exp, int health, int special, int lives,
	int fire, int wind, int ice, int earth, int fullscreen,
	int language, int Bgm, int Sfx);
	~GameInfo(void);

	//Accessors
	int		Stage		( void ) const  {return m_nLevel		;}
	int		EXP 		( void ) const  {return m_nExpPts		;}
	int		Health		( void ) const  {return m_nHealth		;}
	int		Special		( void ) const  {return m_nSpecial		;}
	int		Lives		( void ) const  {return m_nLives		;}
	int		Fire		( void ) const  {return m_nFireSpl		;}
	int		Wind		( void ) const  {return m_nWindSpl		;}
	int		Ice		    ( void ) const  {return m_nIceSpl		;}
	int		Earth		( void ) const  {return m_nEarthSpl	    ;}
	int		Fullscreen	( void ) const  {return m_nFullscreen   ;}
	int		Language	( void ) const  {return m_nLanguage     ;}
	int		BGM		    ( void ) const  {return m_nBGM          ;}
	int		SFX		    ( void ) const  {return m_nSFX          ;}

	//Mutators
	void    Stage		( int nLevel		 )	{  m_nLevel		   = nLevel		    ;}          
	void    EXP 		( int nExpPts		 )	{  m_nExpPts 	   = nExpPts		;}             	
	void    Health		( int nHealth		 )	{  m_nHealth	   = nHealth		;}                 	
	void    Special		( int nSpecial	     )	{  m_nSpecial	   = nSpecial	    ;}          	
	void    Lives		( int nLives		 )	{  m_nLives		   = nLives		    ;}         
	void    Fire		( int nFireSpl	     )	{  m_nFireSpl	   = nFireSpl	    ;}          	
	void    Wind		( int nWindSpl  	 )	{  m_nWindSpl	   = nWindSpl 	    ;}          	
	void    Ice		    ( int nIceSpl		 )	{  m_nIceSpl	   = nIceSpl		;}           	
	void    Earth		( int nEarthSpl 	 )	{  m_nEarthSpl	   = nEarthSpl	    ;}      
	void    Fullscreen	( int nFullscreen    )  {  m_nFullscreen   = nFullscreen    ;}               
	void    Language	( int nLanguage      )  {  m_nLanguage     = nLanguage    	;}
	void    BGM		    ( int nBGM           )  {  m_nBGM          = nBGM           ;}           
	void    SFX		    ( int nSFX           )  {  m_nSFX          = nSFX           ;}         

	void    Display     ( int x, int y);
	void    Upgrade     ( unsigned int nType  );
	bool    CheckGrade  ( unsigned int nType, int Lvl);
private:

	int			m_nLevel		 ;
	int			m_nExpPts		 ;
	int			m_nHealth		 ;
	int			m_nSpecial		 ;
	int			m_nLives		 ;
	int			m_nFireSpl		 ;
	int			m_nWindSpl		 ;
	int			m_nIceSpl		 ;
	int			m_nEarthSpl		 ;
	int			m_nFullscreen    ;
	int			m_nLanguage      ;
	int			m_nBGM           ;
	int			m_nSFX           ;

	int			m_nfireImg		 ;
	int			m_nWindImg		 ;
	int			m_nIceImg		 ;
	int			m_nEarthImg		 ;

	int			m_nMenuArtid	 ;

};

