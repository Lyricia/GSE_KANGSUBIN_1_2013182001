#pragma once
#include "Object.h"



class Scene
{
private:
	Player* dummy;
	Renderer* g_renderer = nullptr;


public:
	void Init();

	Scene();
	~Scene();

	void getRenderer(Renderer* g_render) { g_renderer = g_render; }

	void keyinput(unsigned char key);
	void keyspcialinput(int key);
	void mouseinput(int button, int state, int x, int y);

	void update();
	void render();
};

void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, float r, float g, float b, float a);