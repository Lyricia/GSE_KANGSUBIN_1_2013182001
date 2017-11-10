#pragma once
#include "Object.h"

#define MAX_OBJECT 100

class Timer;

class Scene
{
private:
	Building*	m_Building[1];
	list<Player*>		m_Player;
	list<Projectile*>	m_Missle;
	list<Projectile*>	m_Arrow;

	RECT		screenOOBB = { -250,250,250,-250 };

	Renderer*	m_Renderer = nullptr;
	Timer*		g_Timer = nullptr;

	int			playerid = 0;
	
	int			tex1;
public:
	Scene();
	~Scene();
	void releaseScene();

	void buildScene();
	//void setRenderer(Renderer* g_render) { g_renderer = g_render; }
	void setTimer(Timer* t) { g_Timer = t; }

	void keyinput(unsigned char key);
	void keyspcialinput(int key);
	void mouseinput(int button, int state, int x, int y);

	void update();
	void render();
};