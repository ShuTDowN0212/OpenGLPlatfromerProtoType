
#pragma warning(disable: 4996)

#include "Game.h"
#include "Sphere.h"


CSphere::CSphere() {
	m_fPosition[0] = 0.0f;
	m_fPosition[1] = 0.0f;
	m_fPosition[2] = 0.0f;

	m_fColor[0] = 0.0f;
	m_fColor[1] = 0.0f;
	m_fColor[2] = 0.0f;

	m_fVelocity[0] = 0.0f;
	m_fVelocity[1] = 0.0f;
	m_fVelocity[2] = 0.0f;

	m_nSlice = m_nStack = 20;

	m_Quad = gluNewQuadric();
}

CSphere::CSphere(float *fPosition, float *fColor, float *fVelocity)
{
	int i;
	for(i = 0 ; i < 3 ; i++)
		m_fPosition[i] = fPosition[i];

	for(i = 0 ; i < 3 ; i++)
		m_fColor[i] = fColor[i];

	for(i = 0 ; i < 3 ; i++)
		m_fVelocity[i] = fVelocity[i];

	m_nSlice = m_nStack = 20;

	m_Quad = gluNewQuadric();
}

CSphere::~CSphere()
{
	gluDeleteQuadric(m_Quad);
}

void CSphere::SetPosition(float fX, float fY, float fZ)
{
	m_fPosition[0] = fX;
	m_fPosition[1] = fY;
	m_fPosition[2] = fZ;
}

void CSphere::SetColor(float fR, float fG, float fB)
{
	m_fColor[0] = fR;
	m_fColor[1] = fG;
	m_fColor[2] = fB;
}

void CSphere::SetVelocity(float fVx, float fVy, float fVz)
{
	m_fVelocity[0] = fVx;
	m_fVelocity[1] = fVy;
	m_fVelocity[2] = fVz;
}

void CSphere::SetRadius(double radius)
{
	m_dRadius = radius;
}

void CSphere::Move(int nElapsedTime)
{
	m_fPosition[0] += nElapsedTime*m_fVelocity[0];
	m_fPosition[1] += nElapsedTime*m_fVelocity[1];
	m_fPosition[2] += nElapsedTime*m_fVelocity[2];
}

void CSphere::Render()
{
	glPushMatrix();
	glColor3f(m_fColor[0], m_fColor[1], m_fColor[2]);
	glTranslatef(m_fPosition[0], m_fPosition[1], m_fPosition[2]);
	
	gluSphere(m_Quad, m_dRadius, m_nSlice, m_nStack);
	
	glPopMatrix();
}
