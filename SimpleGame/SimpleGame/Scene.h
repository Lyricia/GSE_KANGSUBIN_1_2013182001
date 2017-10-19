#pragma once
#include "Object.h"

#define MAX_OBJECT 50

class Scene
{
private:
	Player*		m_object[MAX_OBJECT];

	Renderer*	g_renderer = nullptr;
	RECT		screenOOBB;

	bool dummyon = false;

public:
	Scene();
	~Scene();
	void releaseScene();

	void buildScene();
	void getRenderer(Renderer* g_render) { g_renderer = g_render; }
	
	void CollisionChk(Player* id1, Player* id2);

	void keyinput(unsigned char key);
	void keyspcialinput(int key);
	void mouseinput(int button, int state, int x, int y);

	void update();
	void render();
};