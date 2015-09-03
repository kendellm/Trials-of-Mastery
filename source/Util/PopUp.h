#pragma once


class CAnimInfo;
class AnimationManager;
class CEntity;
class CPopUp 
{
public:

	enum PopUp_Type { DASHING_PUNCH };


	CPopUp( PopUp_Type type);
	virtual ~CPopUp(void);

	virtual void Update	(float fElaspedTime);
	virtual void Render (void)				;



	//Accessors
	int		Type   ( void ) const { return m_nType	     ;}
	int		Image  ( void ) const { return m_nImageid    ;}
	int     Sound  ( void ) const { return m_nSFXid      ;}
	float	PosX   ( void ) const { return m_nPosX	     ;}
	float	PosY   ( void ) const { return m_nPosY	     ;}
	int		Width  ( void ) const { return m_nWidth	     ;}
	int		Height ( void ) const { return m_nHeight	 ;}
	float   Timer  ( void ) const { return m_fTimer		 ;}
	bool    Active ( void ) const { return m_bActive     ;}


	CAnimInfo*		GetAnimInfo		( void ) const		{	return m_pAnimInfo;		}


	//Mutators
	void	Type  	( 	int		ntype	 )  { m_nType	 = ntype	;} 
	void	Image  	( 	int		nImageid )	{ m_nImageid = nImageid ;}	
	void	PosX   	( 	float	nPosX	 )	{ m_nPosX	 = nPosX	;} 	
	void	PosY   	( 	float	nPosY	 )	{ m_nPosY	 = nPosY	;} 	
	void	Width  	( 	int		nWidth	 )  { m_nWidth	 = nWidth	;} 
	void	Height 	( 	int		nHeight	 )  { m_nHeight  = nHeight	;} 
	void	Timer 	( 	float	fTime	 )  { m_fTimer   = fTime	;} 	
	void    Active  (   bool    bActive  )  { m_bActive  = bActive  ;}


private:			  

	int			m_nType 	;
	int			m_nImageid	;
	int         m_nSFXid    ;
	float		m_nPosX		;
	float		m_nPosY		;
	int			m_nWidth	;
	int			m_nHeight	;
	float		m_fTimer	;
	bool		m_bActive   ;

	//DWORD		m_dwRenderColor;

protected:
	CAnimInfo*				m_pAnimInfo;
	AnimationManager*		m_pAM;
	CEntity*				m_pOwner;
};

