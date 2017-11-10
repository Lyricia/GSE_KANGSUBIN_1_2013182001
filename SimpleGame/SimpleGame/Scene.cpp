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

	m_Building[0] = new Building(50, Vector3D<float>{0, 0, 0});
	m_Building[0]->setLifetime(10.f);
	m_Building[0]->setLife(500.f);
	m_Building[0]->setDefaultColor(COLOR{ 0,1,0,1 });

	Player* dummy = new Player(OBJTYPE::OBJ_CHARACTER, 0, Vector3D<float>{-500, -500, 0});
	dummy->setLife(-1);
	m_Player.push_back(dummy);

	tex1 = m_Renderer->CreatePngTexture("Assets/HOS.png");
}

void Scene::releaseScene()
{
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
		Vector3D<float> pos = { x, y, 0 };
		Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 15, pos);
		p->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
		p->setSpeed(300);
		p->setDefaultColor(COLOR{ 1,0,0,1 });
		p->setLifetime(10.f);
		p->setLife(10.f);
		p->setID(playerid++);
		m_Player.push_back(p);
	}
}

void Scene::update()
{
	g_Timer->getTimeset();
	double timeElapsed = g_Timer->getTimeElapsed();

	for (auto p : m_Player)
	{
		if (p->isAlive())
		{
			p->wallchk(screenOOBB);
			p->update(timeElapsed);
		}
	}

	for (auto a : m_Arrow)
	{
		if (a->isAlive())
		{
			a->wallchk(screenOOBB);
			a->update(timeElapsed);
		}
	}

	for (auto m : m_Missle)
	{
		if (m->isAlive())
		{
			m->wallchk(screenOOBB);
			m->update(timeElapsed);
		}
	}

	if (m_Building[0]->isAlive())
	{
		if (m_Building[0]->cooltimeChk(timeElapsed))
		{
			Projectile* m = m_Building[0]->ShootMissle();
			m_Missle.push_back(m);
		}
	}

	for (auto arrow : m_Arrow)
	{
		if (arrow->isIntersect(m_Building[0]))
		{
			m_Building[0]->decreaseLife(arrow->getLife());
			m_Arrow.remove(arrow);
			break;
		}
	}

	for (auto it = m_Player.begin(); it !=m_Player.end();)
	{
		Player* p = *it++;
 		if (p->isAlive())
		{
			if (m_Building[0]->isAlive() && m_Building[0]->isIntersect(p))
			{
				m_Building[0]->decreaseLife(p->getLife());
				m_Player.remove(p);
				continue;
			}
		}
		for (auto a : m_Arrow)
		{
			if (a->getOwner() != p->getID())
			{
				if (p->isIntersect(a))
				{
					p->decreaseLife(a->getLife());
					if (!p->isAlive()) {
						m_Player.remove(p);
						m_Arrow.remove(a);
						break;
					}
				}
			}
		}
		for(auto m : m_Missle)
		{
			if (p->isIntersect(m))
			{
				p->decreaseLife(m->getLife());
				if (!p->isAlive())
				{
					m_Player.remove(p);
					m_Missle.remove(m);
					break;
				}
			}
		}
		if (p->cooltimeChk(timeElapsed) && p->isAlive())
		{
			Projectile* a = p->shoot();
			a->setOwner(p->getID());
			m_Arrow.push_back(a);
		}
	}
}

void Scene::render()
{
	if (m_Building[0]->isAlive())
		m_Building[0]->render(m_Renderer, tex1);
	
	for (auto p : m_Player)
		p->render(m_Renderer);
	
	for (auto missle : m_Missle)
		missle->render(m_Renderer);
	
	for (auto arrow : m_Arrow)
		arrow->render(m_Renderer);

}