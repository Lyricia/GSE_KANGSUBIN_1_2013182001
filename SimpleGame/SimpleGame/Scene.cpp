#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::buildScene()
{
	for (int i = 0; i < MAX_OBJECT; ++i) {
		Vector3D<float> pos = { rand() % 500 - 250,rand() % 500 - 250 ,rand() % 500 - 250 };
		Player* tmp = new Player(i, 10, pos);
		tmp->setVelocity(rand() % 10 - 5, rand() % 10 - 5, 0);
		tmp->setColor(COLOR{ 1,0,0,0 });
		m_object[i] = tmp;
	}

	screenOOBB.bottom = -WINDOW_HEIGHT_HALF;
	screenOOBB.top = WINDOW_HEIGHT_HALF;
	screenOOBB.left = -WINDOW_WIDTH_HALF;
	screenOOBB.right = WINDOW_WIDTH_HALF;
}

void Scene::releaseScene()
{
	if (m_object) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			if (m_object[i])
				m_object[i]->releaseObject();
		}
		delete[] m_object;
	}
}



void Scene::keyinput(unsigned char key)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
}

void Scene::keyspcialinput(int key)
{
	switch (key) {
	case 1:
	default:
		break;
	}
}

// 멀티태스킹을 가정하고 업 다운 글로벌을 가지고 있어야한다.
// 밖에서 누르고 안에서 업 할 수도 있기 때문에
void Scene::mouseinput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (dummyon)		dummyon = false;
		else if (!dummyon)	dummyon = true;
	}
}

void Scene::CollisionChk(Player* id1, Player* id2)
{
	if (id1->isIntersect(id2->getPosition())) {

	}
}

void Scene::update()
{
	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		for (int j = 0; j < MAX_OBJECT; ++j) {
			if (i != j)
				CollisionChk(m_object[i], m_object[j]);
		}
		m_object[i]->collisionchk(screenOOBB);
		m_object[i]->update();
	}
}



void Scene::render()
{
	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		m_object[i]->render(g_renderer);
	}
}