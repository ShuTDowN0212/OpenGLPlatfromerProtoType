#pragma once

class CGame;

class CSphere
{
public:
	float m_fPosition[3];
	float m_fColor[3];
	float m_fVelocity[3];
	double m_dRadius;

	GLUquadric *m_Quad;
	int m_nSlice, m_nStack;

	CSphere();
	CSphere(float *fPosition, float *fColor, float *fVelocity);
	~CSphere();

	void SetPosition(float fX, float fY, float fZ);
	void SetColor(float fR, float fG, float fB);
	void SetVelocity(float fVx, float fVy, float fVz);
	void SetRadius(double radius);
	void Move(int nElapsedTime);
	void Render();
};

