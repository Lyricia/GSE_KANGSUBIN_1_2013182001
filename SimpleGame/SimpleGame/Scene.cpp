#include "stdafx.h"
#include "Object.h"
#include "Renderer.h"
#include "Scene.h"

void Scene::Init()
{
	dummy = new Player(1, 50, Vector3D<float>(0, 0, 0));
}

Scene::Scene()
{
	Init();
}


Scene::~Scene()
{
}

void Scene::keyinput(unsigned char key)
{
	switch (key) {
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
}

void Scene::keyspcialinput(int key)
{
	switch (key) {
	
	default:
		break;
	}
}

void Scene::mouseinput(int button, int state, int x, int y)
{
	std::cout << x << "," << y << std::endl;
	dummy->setPosition(x, y, 0);
}

void Scene::update()
{
}

void Scene::render()
{
	int dummysize = dummy->getSize();
	//g_renderer->DrawSolidRect(dummy->getPosition().x, dummy->getPosition().y, dummy->getPosition().z, 50, 1, 0, 0, 0);

	DrawSolidRectByMatrix(dummy->getPosition(), g_renderer, dummysize, 1, 0, 1, 0);
	g_renderer->DrawSolidRect(WINDOW_WIDTH_HALF - dummysize / 2, WINDOW_HEIGHT_HALF - dummysize / 2, 0, dummysize, 1, 0.1, 0, 0);
	g_renderer->DrawSolidRect(WINDOW_WIDTH_HALF - dummysize / 2, -WINDOW_HEIGHT_HALF + dummysize / 2, 0, dummysize, 1, 1, 0, 0);
	g_renderer->DrawSolidRect(-WINDOW_WIDTH_HALF + dummysize / 2, WINDOW_HEIGHT_HALF - dummysize / 2, 0, dummysize, 1, 1, 0, 0);
	g_renderer->DrawSolidRect(-WINDOW_WIDTH_HALF + dummysize / 2, -WINDOW_HEIGHT_HALF + dummysize / 2, 0, dummysize, 1, 0.1, 0, 0);
}


void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, float r, float g, float b, float a)
{
	Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size, r, g, b, a);
}




