//	Game Class Using OpenGL & Glut (Header), ver. 0.6
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma once

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <windows.h>

#include "Hud.h"
#include "Sphere.h"

#include "Wave.h"

class CGame
{
public:
	static int nInstanceCnt;
	static CGame *pInstance;

	int m_nW, m_nH;
	int m_nPosX, m_nPosY;
	bool m_bCreated;

	float m_fBgColor[4];
	double m_dLookAt[3][3];
	
	int m_nFrame;
	int m_nCurrentTime, m_nBaseTime, m_nPreviousTime;
	bool m_bVSync, m_bFullscreen;
	double m_dFps;

	bool m_bPause;
	bool collide = false;
	bool bindobj = false;
	bool xycontrol = false;
	bool attack = false;
	bool revblock = false;
	bool revcontrol = false;
	bool newblock = false;
	bool rotateobj = false;
	float enemyx = 2.0f;
	float enemyz = 0.0f;
	float enemyy = 5.0f;
	float objlocx = -4.0f;
	float objlocy = 5.0f;
	float objlocz = -0.5f;
	float m_fRotationAngle;
	int m_nSlice, m_nStack;
	float m_dDistace;
	float m_dAngleZ;
	double m_dEyeY;

	GLuint m_Texture[5];

	CWave m_Wave[10];

	CHud m_Hud;

	CGame(int nW, int nH, int nPosX, int nPosY);
	~CGame();

	static void SetVSync(bool bSync);
	static void DrawBitmapText(char *string, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, int nW, int nH, bool bFixed2D);

	static void DrawCube();
	void Drawblock();
	static void DrawCoordinates();
	
	static void RenderSceneStatic();
	static void KeyDownStatic(unsigned char key, int x, int y);
	static void SpecialInputStatic(int key, int x, int y);
	static void ResizeStatic(int width, int height);
	static void MouseStatic(int button, int state, int x, int y);
	static void ShutdownAppStatic();

	void Create(int arg, char **argc, float *fBgColor, double (*dLookAt)[3], bool bVSync);

	void RenderScene();
	void KeyDown(unsigned char key, int x, int y);
	void SpecialInput(int key, int x, int y);
	void Resize(int width, int height);
	void Mouse(int button, int state, int x, int y);

	bool InitializeApp();
	void ShutdownApp();
};

