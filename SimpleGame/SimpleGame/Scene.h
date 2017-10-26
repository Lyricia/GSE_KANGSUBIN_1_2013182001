#pragma once
#include "Object.h"

#define MAX_OBJECT 10

class Timer;

class Scene
{
private:
	Player*		m_object[MAX_OBJECT];
	//Player*		m_Player;

	RECT		screenOOBB;
	bool dummyon = false;

	Renderer*	g_renderer = nullptr;
	Timer*		g_Timer = nullptr;

	int			m_objptr = 0;
	int			livecounter = 0;


public:
	Scene();
	~Scene();
	void releaseScene();

	void buildScene();
	void setRenderer(Renderer* g_render) { g_renderer = g_render; }
	void setTimer(Timer* t) { g_Timer = t; }

	void keyinput(unsigned char key);
	void keyspcialinput(int key);
	void mouseinput(int button, int state, int x, int y);

	void update();
	void render();
};