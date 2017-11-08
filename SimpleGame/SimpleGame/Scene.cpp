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
	// Initialize Renderer
	m_Renderer = new Renderer(500, 500);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	for (int i = 0; i < MAX_OBJECT - 1; ++i) 
	{
		m_Player[i] = new Player();
	}

	m_Building[0] = new Building(50, Vector3D<float>{0, 0, 0});
	m_Building[0]->setLifetime(10.f);
	m_Building[0]->setLife(500.f);
	m_Building[0]->setColor(COLOR{ 0,1,0,0 });
}

void Scene::releaseScene()
{
	if (m_Player) 
	{
		for (int i = 0; i < MAX_OBJECT; ++i)
		{
			if (m_Player[i])
				m_Player[i]->releaseObject();
		}

		delete[] m_Player;
	}
	delete[] m_Building;
}



void Scene::keyinput(unsigned char key)
{
	switch (key)
	{
	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
}

void Scene::keyspcialinput(int key)
{
	switch (key)
	{
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		if (m_objptr != -1)
		{
			Vector3D<float> pos = { x, y, 0 };
			Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 10, pos);
			p->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
			p->setSpeed(300);
			p->setColor(COLOR{ 1,0,0,0 });
			p->setLifetime(10.f);
			p->setLife(10.f);
			m_Player[m_objptr] = p;
		}
	}
}

void Scene::update()
{
	g_Timer->getTimeset();
	double timeElapsed = g_Timer->getTimeElapsed();

	if (m_Building[0]->isAlive()) 
	{
		Missle* m = m_Building[0]->ShootMissle(timeElapsed);
		if (m != NULL)
		{
			m_Missle.push_back(m);
		}
	}

	for (auto missle : m_Missle)
	{
		missle->wallchk(screenOOBB);
		if (missle->isAlive())
			missle->update(timeElapsed);
		else
		{
			m_Missle.remove(missle);
			break;
		}
	}

	for (int i = 0; i < MAX_OBJECT - 1; ++i)
	{
		m_Player[i]->update(timeElapsed);
		m_Player[i]->wallchk(screenOOBB);
		if (m_Player[i]->isAlive() && m_Building[0]->isAlive())
		{
			if (m_Building[0]->isIntersect(m_Player[i]))
			{
				m_Building[0]->setColor(COLOR{ 1,1,0,0 });
				m_Building[0]->decreaseLife(m_Player[i]->getLife());
				m_Player[i]->resetObject();
			}
			else
				m_Building[0]->setColor(COLOR{ 0,1,0,0 });
		}
		for (auto missle : m_Missle)
		{
			if (m_Player[i]->isIntersect(missle)) 
			{
				m_Player[i]->decreaseLife(missle->getLife());
				if (!m_Player[i]->isAlive())
					m_Player[i]->resetObject();
				m_Missle.remove(missle);
				break;
			}
		}
	}


	for (int i = 0; i < MAX_OBJECT - 1; ++i)
	{
		if (!m_Player[i]->isAlive())
		{
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
			m_Player[i]->render(m_Renderer);
	}
	if (m_Building[0]->isAlive())
		m_Building[0]->render(m_Renderer);

	for (auto missle : m_Missle)
	{
		missle->render(m_Renderer);
	}
}