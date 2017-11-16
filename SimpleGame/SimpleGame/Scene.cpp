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
	m_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	// Init Building Setting
	for (int i = 0; i < 3; ++i)
	{
		m_Building[i] = new Building(100, Vector3D<float>{-150 + 150 * i, 300, 0});
		m_Building[i]->setLifetime(10.f);
		m_Building[i]->setLife(500.f);
		m_Building[i]->setDefaultColor(COLOR{ 0,1,0,1 });
		m_Building[i]->setTeam(TEAM::RED);
	}
	for (int i = 3; i < 6; ++i)
	{
		m_Building[i] = new Building(100, Vector3D<float>{-150 + 150 * (i - 3), -300, 0});
		m_Building[i]->setLifetime(10.f);
		m_Building[i]->setLife(500.f);
		m_Building[i]->setDefaultColor(COLOR{ 0,1,0,1 });
		m_Building[i]->setTeam(TEAM::BLUE);
	}
	BuildingTex[0] = m_Renderer->CreatePngTexture("Assets/HOS.png");
	BuildingTex[1] = m_Renderer->CreatePngTexture("Assets/LOL.png");

	// Init Unit Setting
	Player* dummy = new Player(OBJTYPE::OBJ_CHARACTER, 0, Vector3D<float>{-500, -500, 0});
	dummy->setLife(-1);
	m_Player.push_back(dummy);

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
		
	case 'a':
		m_Building[0]->setLife(-10);
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
		if (m_BlueTeamCreateTimer > 3) {
			m_BlueTeamCreateTimer = 0.f;
			Vector3D<float> pos = { x, y, 0 };
			Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 10, pos);
			p->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
			p->setSpeed(300);
			p->setDefaultColor(COLOR{ 0,0,1,1 });
			p->setLifetime(10.f);
			p->setLife(10.f);
			p->setID(playerid++);
			p->setTeam(TEAM::BLUE);
			m_Player.push_back(p);
		}
		else
			cout << "CoolTime : " << 3 - m_BlueTeamCreateTimer << endl;
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
			p->wallchk();
			p->update(timeElapsed);
		}
	}

	for (auto a : m_Arrow)
	{
		if (a->isAlive())
		{
			a->wallchk();
			a->update(timeElapsed);
		}
	}

	for (auto b : m_Bullet)
	{
		if (b->isAlive())
		{
			b->wallchk();
			b->update(timeElapsed);
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		if (m_Building[i]->isAlive())
		{
			if (m_Building[i]->cooltimeChk(timeElapsed))
			{
				Projectile* m = m_Building[i]->ShootBullet();
				m_Bullet.push_back(m);
			}
		}
		else
		{
			m_Building[i]->setPosition(Vector3D<float>(-500, -1000, 0));
		}

		//for (auto b : m_Bullet)
		//{
		//	if (b->getTeam() != m_Building[i]->getTeam())
		//	{
		//		if (m_Building[i]->isIntersect(b))
		//		{
		//			m_Building[i]->decreaseLife(b->getLife());
		//			m_Bullet.remove(b);
		//			break;
		//		}
		//	}
		//}

		for (auto arrow : m_Arrow)
		{
			if (arrow->getTeam() != m_Building[i]->getTeam())
			{
				if (arrow->isIntersect(m_Building[i]))
				{
					m_Building[i]->decreaseLife(arrow->getLife());
					m_Arrow.remove(arrow);
					break;
				}
			}
		}
	}
	
	for (auto it = m_Player.begin(); it !=m_Player.end();)
	{
		Player* p = *it++;
 		if (p->isAlive())
		{
			for (int i = 0; i < 6; ++i)
			{
				if (m_Building[i]->getTeam() != p->getTeam()) 
				{
					if (m_Building[i]->isAlive() && m_Building[i]->isIntersect(p))
					{
						m_Building[i]->decreaseLife(p->getLife());
						m_Player.remove(p);
						continue;
					}
				}
			}
		}
		for (auto a : m_Arrow)
		{
			if (a->getTeam() != p->getTeam())
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
		for (auto b : m_Bullet)
		{
			if (b->getTeam() != p->getTeam())
			{
				if (p->isIntersect(b))
				{
					p->decreaseLife(b->getLife());
					if (!p->isAlive())
					{
						m_Player.remove(p);
						m_Bullet.remove(b);
						break;
					}
				}
			}
		}
		if (p->cooltimeChk(timeElapsed) && p->isAlive())
		{
			Projectile* a = p->ShootArrow();
			a->setOwner(p->getID());
			m_Arrow.push_back(a);
		}
	}

	m_RedTeamCreateTimer += timeElapsed;
	m_BlueTeamCreateTimer += timeElapsed;
	if (m_RedTeamCreateTimer > 5)
	{
		m_RedTeamCreateTimer = 0.f;
		Vector3D<float> pos = { -220 + rand() % 440, 250 - rand() % 200 , 0 };
		Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 10, pos);
		p->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
		p->setSpeed(300);
		p->setDefaultColor(COLOR{ 1,0,0,1 });
		p->setLifetime(10.f);
		p->setLife(10.f);
		p->setID(playerid++);
		p->setTeam(TEAM::RED);
		m_Player.push_back(p);
	}
}

void Scene::render()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_Building[i]->isAlive())
			m_Building[i]->render(m_Renderer, BuildingTex[0]);
	}
	for (int i = 3; i < 6; ++i)
	{
		if (m_Building[i]->isAlive())
			m_Building[i]->render(m_Renderer, BuildingTex[1]);
	}

	for (auto p : m_Player)
		p->render(m_Renderer);
	
	for (auto bullet : m_Bullet)
		bullet->render(m_Renderer);
	
	for (auto arrow : m_Arrow)
		arrow->render(m_Renderer);
}

