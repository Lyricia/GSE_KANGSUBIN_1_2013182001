#include "stdafx.h"
#include "Renderer.h"
#include "Timer.h"
#include "Scene.h"

using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::buildScene()
{
	for (int i = 0; i < MAX_OBJECT - 1; ++i) {
		m_Player[i] = new Player();
	}

	m_Building[0] = new Building(40, Vector3D<float>{0, 0, 0});
	m_Building[0]->setLifetime(10.f);
	m_Building[0]->setColor(COLOR{ 0,1,0,0 });

	screenOOBB.bottom = -WINDOW_HEIGHT_HALF;
	screenOOBB.top = WINDOW_HEIGHT_HALF;
	screenOOBB.left = -WINDOW_WIDTH_HALF;
	screenOOBB.right = WINDOW_WIDTH_HALF;
}

void Scene::releaseScene()
{
	if (m_Player) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			if (m_Player[i])
				m_Player[i]->releaseObject();
		}

		delete[] m_Player;
	}
	delete[] m_Building;
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
		if (m_objptr != -1) {
			Vector3D<float> pos = { x, y, 0 };
			Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 25, pos);
			p->setDirection(Vector3D<float>((rand() % 5 - 2.5f), (rand() % 5 - 2.5f), 0.f).Normalize());
			p->setSpeed(300);
			p->setColor(COLOR{ 1,0,0,0 });
			p->setLifetime(10.f);
			m_Player[m_objptr] = p;
		}
	}
}

void Scene::update()
{
	g_Timer->getTimeset();
	double timeElapsed = g_Timer->getTimeElapsed();

	m_Building[0]->update(timeElapsed);

	for (int i = 0; i < MAX_OBJECT-1; ++i)
	{
		m_Player[i]->update(timeElapsed);
		m_Player[i]->wallchk(screenOOBB );

		if (m_Player[i]->isAlive()) {
			for (int j = 0; j < MAX_OBJECT - 1; ++j) {
				if (m_Player[j]->isAlive()) {
					if (i != j) {
						if (m_Player[i]->getTarget() == NULL || m_Player[i]->getTarget() == m_Player[j])
							m_Player[i]->isIntersect(m_Player[j]);
					}
				}
			}
			
			// wall check
			
		}
		else
			m_Player[i]->resetObject();
	}

	for (int i = 0; i < MAX_OBJECT-1; ++i) {
		if (m_Player[i]->isAlive() && m_Building[0]->isAlive()) {
			if (m_Building[0]->isIntersect(m_Player[i])) {
				m_Building[0]->setColor(COLOR{ 1,1,0,0 });
				m_Building[0]->decreaseLife();
				m_Player[i]->resetObject();
			}
			else
				m_Building[0]->setColor(COLOR{ 0,1 ,0,0 });
		}
	}


	for (int i = 0; i < MAX_OBJECT - 1; ++i) {
		if (!m_Player[i]->isAlive()) {
			m_objptr = i;
			break;
		}
		else
			m_objptr = -1;
	}
}

void Scene::render()
{
	for (int i = 0; i < MAX_OBJECT-1; ++i)
	{
		if (m_Player[i]->isAlive())
			m_Player[i]->render(g_renderer);
	}
	if (m_Building[0]->isAlive())
		m_Building[0]->render(g_renderer);

}