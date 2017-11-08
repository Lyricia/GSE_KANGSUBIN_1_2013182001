#pragma once
#include "Object.h"

#define MAX_OBJECT 100

class Timer;

class Scene
{
private:
	Player*		m_Player[MAX_OBJECT];
	Building*	m_Building[1];
	//Missle*		m_Missle[1000];

	list<Missle*> m_Missle;

	RECT		screenOOBB = { -250,250,250,-250 };

	bool dummyon = false;

	Renderer*	m_Renderer = nullptr;
	Timer*		g_Timer = nullptr;

	int			m_objptr = 0;
	int			livecounter = 0;

	int			m_MissleCounter = 0;
	int			m_Missleidx = 0;

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