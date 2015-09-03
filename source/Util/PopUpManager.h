#pragma once

#include <vector>
#include "../CSGD/IListener.h"

class CPopUp;
class CPopUpManager : public IListener
{
public:

	static CPopUpManager* GetInstance();

	struct SPopUp
	{
		int			m_nType;
		int			m_nImageid;
		int			m_nPosX;
		int			m_nPosY;
		int			m_nWidth;
		int			m_nHeight;
	};
	
	void Initialize ( std::string filepath );
	void Shutdown	(void);
	void Update	(float fElaspedTime);
	void Render (void)				;


	void Create ( int _type );

	void Destroy(void);
	void HandleEvent( CEvent* pEvent );
	void Register_Completed();
	void Load( unsigned int nType );

private:

	float						 m_fTimer;
	int						     m_nSFXid;
	SPopUp*						 m_pCurrPopUp;
	CPopUp*						 m_pAnimPopUp;
	std::vector<SPopUp*>		 m_vPopUps;
	std::vector<CPopUp*>		 m_vAniPopUps;

	static CPopUpManager		 m_Instance;	
	CPopUpManager(void);
	~CPopUpManager(void);


};

