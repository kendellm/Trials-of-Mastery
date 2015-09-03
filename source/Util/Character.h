#pragma once
class CCharacter
{
public:
	CCharacter(void);
	CCharacter(int x, int y, int w, int h,
				int xOffset, int yOffset, int Xadvance);
	~CCharacter(void);


	void X			(int _x)		{m_nPosX     = _x;}
	void Y			(int _y)		{m_nPosY     = _y;}
	void Width		(int _w)		{m_nWidth    = _w;}
	void Height		(int _h)		{m_nHeight   = _h;}
	void XOffset	(int _xoff)		{m_nXOffset  = _xoff;}
	void YOffset	(int _yoff)		{m_nYOffset  = _yoff;}
	void XAdvance	(int _xadv)		{m_nXAdvance = _xadv;}

	int  X			(void)		    {return m_nPosX;      }
	int  Y			(void)			{return m_nPosY;      } 
	int  Width		(void)			{return m_nWidth;     } 
	int  Height		(void)			{return m_nHeight;    } 
	int  XOffset	(void)			{return m_nXOffset;   } 
	int  YOffset	(void)			{return m_nYOffset;	  }
	int  XAdvance	(void)			{return m_nXAdvance;  } 

private:	
	unsigned int m_nPosX;
	unsigned int m_nPosY;
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned int m_nXOffset;
	unsigned int m_nYOffset;
	unsigned int m_nXAdvance;


};

