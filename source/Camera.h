#pragma once
class CEntity;
class CCamera
{
private:
	float PosX;
	float PosY;
	float Width;
	float Height;
	bool Locked;
	CCamera(void);
	~CCamera(void);

public:
	static CCamera* GetInstance( void );

	float GetPosX() {return PosX;}
	float GetPosY() {return PosY;}
	float GetWidth() {return Width;}
	float GetHeight() {return Height;}

	void SetPosX(float x){PosX = x;}
	void SetPosY(float y){PosY = y;}
	void SetWidth(float w) {Width = w;}
	void SetHeight(float h){ Height = h;}

	void ChangePosX(float x){PosX = PosX + x;}
	void ChangePosY(float y){PosY = PosY + y;}

	void SetLocked(bool b){Locked = b;}

	void Update(CEntity* Player);
};

