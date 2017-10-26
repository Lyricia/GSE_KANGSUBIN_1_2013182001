#include "stdafx.h"
#include "Renderer.h"
#include "Timer.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::buildScene()
{
	//m_Player = new Player(100, 25, Vector3D<float>{0, 0, 0});
	//m_Player->setColor(COLOR{ 1,0,0,0 });
	//m_object[0] = m_Player;
	
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
	//m_Player->setPosition(Vector3D<float>{x, y, 0});
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (livecounter < 10) {
			Vector3D<float> pos = { x, y, 0 };
			Player* p = new Player(-1, 25, pos);
			p->setDirection(Vector3D<float>((rand() % 5 - 2.5f), (rand() % 5 - 2.5f), 0.f).Normalize());
			p->setSpeed(100);
			p->setColor(COLOR{ 1,0,0,0 });
			p->setLifetime(35.f);
			m_object[m_objptr] = p;
			m_objptr = (m_objptr++) % 10;
			livecounter++;
		}
	}
}

void Scene::update()
{
	g_Timer->getTimeset();
	double timeElapsed = g_Timer->getTimeElapsed();

	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		if (m_object[i]) {
			for (int j = 0; j < MAX_OBJECT; ++j) {
				if (m_object[j]) {
					if (i != j) {
						if (m_object[i]->getTarget() == NULL || m_object[i]->getTarget() == m_object[j])
							m_object[i]->isIntersect(m_object[j]);
					}
				}
			}
			m_object[i]->collisionchk(screenOOBB);

			if (!m_object[i]->isAlive()) {
				m_object[i]->setPosition({ -500, -500, 0 });
				livecounter--;
				if (livecounter > 0)
					livecounter = 0;
			}
			else if (m_object[i]->isAlive())
				m_object[i]->update(timeElapsed);
		}
	}
}




void Scene::render()
{
	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		if(m_object[i])
			m_object[i]->render(g_renderer);
	}
	//m_Player->render(g_renderer);
}