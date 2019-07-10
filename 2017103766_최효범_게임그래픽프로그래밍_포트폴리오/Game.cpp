//	Game Class Using OpenGL & Glut (Implementation Main), ver. 0.6
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma warning(disable: 4996)

#include "Game.h"
#include "Bmp.h"
#include "lodepng.h"

CGame::CGame(int nW, int nH, int nPosX, int nPosY)
{
	m_nW = nW;
	m_nH = nH;
	
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	m_bPause = false;

	m_bCreated = false;

	if(nInstanceCnt == 0) pInstance = this;

	nInstanceCnt++;
}

CGame::~CGame()
{
	if(m_bCreated)
		ShutdownApp();

	nInstanceCnt--;
}

void CGame::Create(int arg, char **argc, float *fBgColor, double (*dLookAt)[3], bool bVSync)
{
	glutInitWindowSize(m_nW, m_nH);
	glutInitWindowPosition(m_nPosX, m_nPosY);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInit(&arg, argc);
	glutCreateWindow("Platformer");

	m_nFrame = 0;
	m_nBaseTime = glutGet(GLUT_ELAPSED_TIME); 
	m_nPreviousTime = m_nBaseTime;

	glutDisplayFunc(RenderSceneStatic);
	glutReshapeFunc(ResizeStatic);
	glutKeyboardFunc(KeyDownStatic);
	glutSpecialFunc(SpecialInputStatic);
	glutMouseFunc(MouseStatic);

	atexit(ShutdownAppStatic);

	m_bVSync = bVSync;
	SetVSync(m_bVSync);

	int i;
	int r, c;
	for(i = 0 ; i < 4 ; i++)
		m_fBgColor[i] = fBgColor[i];

	for(r = 0 ; r < 3 ; r++)
		for(c = 0 ; c < 3 ; c++)
			m_dLookAt[r][c] = dLookAt[r][c];

	m_dFps = 0.;

	m_Hud.SetPosition(10.0f, 10.0f);
	m_Hud.SetColor(1.0f, 0.0f, 0.0f);

	m_fRotationAngle = 0.0f;
	m_dDistace = 15.0;
	m_dAngleZ = 0.0;
	m_dEyeY = 0;

	m_bFullscreen = false;

	if (InitializeApp() == true)
	{
		m_bCreated = true;
		glutMainLoop();
	}
}

void CGame::RenderScene()
{
	m_nCurrentTime = glutGet(GLUT_ELAPSED_TIME);
	int nElapsedTime = m_nCurrentTime-m_nPreviousTime;
	
	if(nElapsedTime < 16.67) 
	{
		glutPostRedisplay();
		return;
	}

	m_nPreviousTime = m_nCurrentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_dLookAt[0][0] = m_dDistace*sin(m_dAngleZ);
	m_dLookAt[0][1] = m_dEyeY;
	m_dLookAt[0][2] = m_dDistace*cos(m_dAngleZ);

	gluLookAt(m_dLookAt[0][0], m_dLookAt[0][1], m_dLookAt[0][2],
		m_dLookAt[1][0], m_dLookAt[1][1], m_dLookAt[1][2],
		m_dLookAt[2][0], m_dLookAt[2][1], m_dLookAt[2][2]);

	DrawCoordinates();

	m_nSlice = 20;
	//m_fRotationAngle = 0;
	m_nStack = m_nSlice;

	if(!m_bPause)
	{
	}

	
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-15.0f, -12.0f, -4.0f);
	glScalef(35.0f, 25.0f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	if (attack == false)
	{
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glTranslatef(objlocx, objlocz, objlocy);
		glScalef(1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[1]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	else
	{
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glTranslatef(objlocx, objlocz, objlocy);
		glScalef(1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[3]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (collide != true)
	{
		float enemyz = 0.0f;
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glTranslatef(enemyx, enemyz, enemyy);
		glScalef(1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[4]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (attack == true && sqrt((abs(objlocx) - abs(enemyx)) * (abs(objlocx) - abs(enemyx))
		+ (abs(objlocz) - abs(enemyz)) * (abs(objlocz) - abs(enemyz)) + 
		(abs(objlocy) - abs(enemyy)) * (abs(objlocy) - abs(enemyy))) < 2 * sqrt(0.25f))
	{
		collide = true;
	}

	float switchx = 4.0f;
	float switchy = -5.0f;
	float cubex = -4.0f;
	float cubey = 5.0f;
	float cubez = -1.5f;

	if (newblock != true && revblock == false)
	{
		for (int i = 0; i < 9; i++)
		{
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glTranslatef(cubex + i, cubez, cubey);
			glScalef(1.f, 1.f, 1.f);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture[2]);
			DrawCube();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	else if (newblock == true && revblock == false)
	{
		for (int i = 0; i < 11; i++)
		{
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glTranslatef(-cubex, cubez - 1, cubey - i);
			glScalef(1.f, 1.f, 1.f);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture[2]);
			DrawCube();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	else if(newblock != true && revblock != false)
	{
		for (int i = 0; i < 11; i++)
		{
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glTranslatef(cubex + i, cubez, -cubey);
			glScalef(1.f, 1.f, 1.f);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture[2]);
			DrawCube();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	else if(newblock == true && revblock != false)
	{
		for (int i = 0; i < 11; i++)
		{
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glTranslatef(cubex, cubez - 1, cubey - i);
			glScalef(1.f, 1.f, 1.f);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture[2]);
			DrawCube();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	if(objlocx == switchx)
	{
		bindobj = true;
		if (xycontrol == false)
		{
			if (m_dAngleZ < 3.14159 / 2)			
			{
				m_dAngleZ += 0.025;
			}

			else
			{
				newblock = true;
				objlocz--;
				m_dLookAt[1][1] -= 1.5f;
				xycontrol = true;
			}
		}
		switchx = -switchx;
		bindobj = false;
	}

	else if (objlocx == switchx && revblock == true)
	{
		bindobj = true;
		if (xycontrol == false)
		{
			if (m_dAngleZ < 3.14159 * 3 / 2)
			{
				m_dAngleZ += 0.025;
			}
			else
			{
				newblock = true;
				objlocz--;
				m_dLookAt[1][1] -= 1.5f;
				xycontrol = true;
			}
		}
		switchx = -switchx;
		bindobj = false;
	}

	if (objlocy == switchy)
	{
		bindobj = true;
		if (xycontrol == true)
		{
			if (m_dAngleZ < -3.14159)
			{
				m_dAngleZ += 0.025;
			}
			else
			{
				newblock = false;
				m_dLookAt[1][1] -= 1.5f;
				objlocz--;
				xycontrol = false;
			}
		}
		switchy = -switchy;
		bindobj = false;
		revblock = true;
	}

	else if (objlocy == switchy && revblock == true)
	{
		bindobj = true;
		if (xycontrol == true)
		{
			if (m_dAngleZ < 3.14159 * 2)
			{
				m_dAngleZ += 0.025;
			}
			else
			{	
				newblock = false;
				m_dLookAt[1][1] -= 1.5f;
				objlocz++;
				xycontrol = false;
			}
		}
		switchy = -switchy;
		bindobj = false;
		revblock = false;
	}

	m_nFrame++;
	m_nCurrentTime = glutGet(GLUT_ELAPSED_TIME);
	if (m_nCurrentTime - m_nBaseTime > 1000)
	{
		m_dFps = m_nFrame*1000.0/(m_nCurrentTime-m_nBaseTime);
		m_nBaseTime = m_nCurrentTime; 
		m_nFrame = 0;
	}

	char FpsText[100];
	sprintf(FpsText, "FPS: %4.2f, Pause: %s", m_dFps, m_bPause?"On":"Off");
	
	m_Hud.SetText(FpsText);
	m_Hud.DisplayHud(this);
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void CGame::KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'v':
		m_bVSync = !m_bVSync;
		SetVSync(m_bVSync);
		break;

	case 'p':
		m_bPause = !m_bPause;
		break;

	case 'a':
		if (xycontrol == false)
		{
			if (revcontrol == false)
			{
				objlocx += -0.5;
			}
			else
			{
				objlocx += 0.5;
			}
		}
		else
		{
			if (revcontrol == false)
			{
				objlocy += 0.5;
			}
			else
			{
				objlocy += -0.5;
			}
		}
		break;

	case 'd':
		if (xycontrol == false)
		{
			if (revcontrol == false)
			{
				objlocx += 0.5;
			}
			else
			{
				objlocx += -0.5;
			}
		}
		else
		{
			if (revcontrol == false)
			{
				objlocy += -0.5;
			}
			else
			{
				objlocy += 0.5;
			}
		}
		break;
	case 't':
		if (attack == false)
		{
			attack = true;
			PlayWave(&m_Wave[1]);
		}
		else
			attack = false;
	}
}

void CGame::SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		m_dDistace += 0.1;
		break;

	case GLUT_KEY_DOWN:
		m_dDistace -= 0.1;
		break;

	case GLUT_KEY_RIGHT:
		m_dAngleZ += 0.05;
		break;

	case GLUT_KEY_LEFT:
		m_dAngleZ -= 0.05;
		break;

	case GLUT_KEY_F11:
		if (!m_bFullscreen){
			glutFullScreen();
			
			m_bFullscreen = true;
		}
		else if (m_bFullscreen){
			glutReshapeWindow(m_nW, m_nH);
			glutPositionWindow(m_nPosX, m_nPosY);

			m_bFullscreen = false;
		}

		break;
	}
}

void CGame::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)width / (double)height, 0.1, 20.0);
}

void CGame::Mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(y > m_nH/2)
			m_dEyeY -= 0.1;
		else
			m_dEyeY += 0.1;
	}
}

bool CGame::InitializeApp()
{
	glClearColor(0.9f, 1.0f, 0.9f, 1.0f);
	glShadeModel(GL_SMOOTH);//
	glEnable(GL_DEPTH_TEST);//

	unsigned char *Image;
	int nW, nH;
	Image = ReadBmp("castle.bmp", &nW, &nH);

	if(!Image) return false;

	glGenTextures(5, m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nW, nH, GL_BGR, GL_UNSIGNED_BYTE, Image);

	delete [] Image;

	Image = ReadBmp("player1.bmp", &nW, &nH);

	if (!Image) return false;

	glBindTexture(GL_TEXTURE_2D, m_Texture[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nW, nH, GL_BGR, GL_UNSIGNED_BYTE, Image);

	delete[] Image;

	Image = ReadBmp("dirt_3.bmp", &nW, &nH);

	if (!Image) return false;

	glBindTexture(GL_TEXTURE_2D, m_Texture[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nW, nH, GL_BGR, GL_UNSIGNED_BYTE, Image);

	delete[] Image;
	
	Image = ReadBmp("slash.bmp", &nW, &nH);

	if (!Image) return false;

	glBindTexture(GL_TEXTURE_2D, m_Texture[3]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nW, nH, GL_BGR, GL_UNSIGNED_BYTE, Image);

	delete[] Image;

	Image = ReadBmp("eyeball.bmp", &nW, &nH);

	if (!Image) return false;

	glBindTexture(GL_TEXTURE_2D, m_Texture[4]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nW, nH, GL_BGR, GL_UNSIGNED_BYTE, Image);

	delete[] Image;

	
	printf("%d\n", LoadMp3("stage.mp3", &m_Wave[0]));
	printf("%d\n", LoadWave("sword.wav", &m_Wave[1]));

	PlayWave(&m_Wave[0]);

	return true;
}

void CGame::ShutdownApp()
{
}
