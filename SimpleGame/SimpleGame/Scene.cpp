#include "stdafx.h"
#include "Renderer.h"
#include "Timer.h"
#include "Sound.h"
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
		m_Building[i]->setDefaultColor(COLOR{ 1,1,1,1 });
		m_Building[i]->setTeam(TEAM::RED);
	}
	for (int i = 3; i < 6; ++i)
	{
		m_Building[i] = new Building(100, Vector3D<float>{-150 + 150 * (i - 3), -300, 0});
		m_Building[i]->setLifetime(10.f);
		m_Building[i]->setLife(500.f);
		m_Building[i]->setDefaultColor(COLOR{ 1,1,1,1 });
		m_Building[i]->setTeam(TEAM::BLUE);
	}

	BuildingTex[0] = m_Renderer->CreatePngTexture("Assets/Image/LOL.png");
	BuildingTex[1] = m_Renderer->CreatePngTexture("Assets/Image/HOS.png");
	PlayerTex[0] = m_Renderer->CreatePngTexture("Assets/Image/spinning-scythe.png");
	PlayerTex[1] = m_Renderer->CreatePngTexture("Assets/Image/vampire-bat.png");
	BackGroundTex = m_Renderer->CreatePngTexture("Assets/Image/BackGround.png");
	ParticleTex = m_Renderer->CreatePngTexture("Assets/Image/effect.png");
	SnowParticle = m_Renderer->CreatePngTexture("Assets/Image/SnowParticle.png");

	// Init Unit Setting
	Player* dummy = new Player(OBJTYPE::OBJ_CHARACTER, 0, Vector3D<float>{-500, -500, 0});
	dummy->setLife(-1);
	m_Player.push_back(dummy);

	GameStatus = GAMESTATUS::RUNNING;

	m_Sound = new Sound();
	m_SoundIdx[SOUNDINDEX::BGM] = m_Sound->CreateSound("Assets/Sound/Cheetahmen.mp3");
	m_SoundIdx[SOUNDINDEX::WIN] = m_Sound->CreateSound("Assets/Sound/WinSound.wav");
	m_SoundIdx[SOUNDINDEX::LOSE] = m_Sound->CreateSound("Assets/Sound/LoseSound.wav");
	m_SoundIdx[SOUNDINDEX::CRASHEFFECT] = m_Sound->CreateSound("Assets/Sound/CrashEffect.mp3");
	m_SoundIdx[SOUNDINDEX::PAUSESOUND] = m_Sound->CreateSound("Assets/Sound/PauseSound.wav");
	m_Sound->PlaySound(m_SoundIdx[0], true, 0.5f);
}

void Scene::releaseScene()
{
	m_Sound->StopAllSound();
	if (m_Sound) {
		for (int i = 0; i < 5; ++i)
			m_Sound->DeleteSound(i);
	}
	
	m_Player.clear();
	m_Bullet.clear();
	m_Arrow.clear();

	delete		m_Renderer;
	delete		g_Timer;
	delete[]	m_Building;
}


void Scene::keyinput(unsigned char key)
{
	switch (key)
	{
	case 'p':
	case 'P':

		if (GameStatus == GAMESTATUS::RUNNING)
		{
			m_Sound->PauseAllSound(true);
			GameStatus = GAMESTATUS::PAUSE;
		}
		else if (GameStatus == GAMESTATUS::PAUSE)
		{
			m_Sound->PauseAllSound(false);
			GameStatus = GAMESTATUS::RUNNING;
		}
		m_Sound->PlaySound(m_SoundIdx[SOUNDINDEX::PAUSESOUND], false, 0.5f);
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && GAMESTATUS::RUNNING)
	{
		if (m_BlueTeamCreateTimer > 0 && GameStatus == GAMESTATUS::RUNNING) {
			m_BlueTeamCreateTimer = 0.f;
			Vector3D<float> pos = { x, y, 0 };
			Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 30, pos);
			p->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
			p->setSpeed(300);
			p->setDefaultColor(COLOR{ 0,0,1,1 });
			p->setLifetime(10.f);
			p->setLife(100.f);
			p->setID(playerid++);
			p->setTeam(TEAM::BLUE);
			p->SetSeq(7, 0, 10, 4);
			m_Player.push_back(p);
		}
	}
}

void Scene::update()
{
	if (GameStatus == GAMESTATUS::RUNNING) {
		int reddeadcount = 0;
		int bluedeadcount = 0;

		g_Timer->getTimeset();
		double timeElapsed = g_Timer->getTimeElapsed();

		for (auto p : m_Player)
		{
			if (p->isAlive())
			{
				p->update(timeElapsed);
				p->wallchk();
			}
		}

		for (auto a : m_Arrow)
		{
			if (a->isAlive())
			{
				a->update(timeElapsed);
				if (a->wallchk()) {
					m_Arrow.remove(a);
					break;
				}
			}
		}

		for (auto b : m_Bullet)
		{
			if (b->isAlive())
			{
				b->update(timeElapsed);
				if (b->wallchk()) {
					m_Bullet.remove(b);
					break;
				}
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
				if (i < 3)	reddeadcount++;
				else		bluedeadcount++;

				if (reddeadcount == 3)
				{
					GameStatus = GAMESTATUS::BLUEWIN;
					m_Sound->StopAllSound();
					m_Sound->PlaySound(m_SoundIdx[SOUNDINDEX::WIN], false, 0.5f);
				}
				else if (bluedeadcount == 3)
				{
					GameStatus = GAMESTATUS::REDWIN;
					m_Sound->StopAllSound();
					m_Sound->PlaySound(m_SoundIdx[SOUNDINDEX::LOSE], false, 0.5f);
				}
			}

			for (auto b : m_Bullet)
			{
				if (b->getTeam() != m_Building[i]->getTeam())
				{
					if (m_Building[i]->isIntersect(b))
					{
						m_ShakeTime = 3.f;
						m_ShakeStrength = 10;
						m_Building[i]->decreaseLife(b->getLife());
						m_Bullet.remove(b);
						
						m_Sound->PlaySound(m_SoundIdx[SOUNDINDEX::CRASHEFFECT], false, 0.3f);
						break;
					}
				}
			}

			for (auto arrow : m_Arrow)
			{
				if (arrow->getTeam() != m_Building[i]->getTeam())
				{
					if (arrow->isIntersect(m_Building[i]))
					{
						m_ShakeTime = 3.f;
						m_ShakeStrength = 10;
						m_Building[i]->decreaseLife(arrow->getLife());
						m_Arrow.remove(arrow);
						
						m_Sound->PlaySound(m_SoundIdx[SOUNDINDEX::CRASHEFFECT], false, 0.3f);
						break;
					}
				}
			}
		}

		for (auto it = m_Player.begin(); it != m_Player.end();)
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
							m_ShakeTime = 4.f;
							m_ShakeStrength = 20;
							m_Building[i]->decreaseLife(p->getLife());
							m_Player.remove(p);

							m_Sound->PlaySound(m_SoundIdx[SOUNDINDEX::CRASHEFFECT], false, 0.5f);
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
		if (m_RedTeamCreateTimer > 2)
		{
			m_RedTeamCreateTimer = 0.f;
			Vector3D<float> pos = { -220 + rand() % 440, 250 - rand() % 200 , 0 };
			Player* p = new Player(OBJTYPE::OBJ_CHARACTER, 30, pos);
			p->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
			p->setSpeed(300);
			p->setDefaultColor(COLOR{ 1,0,0,1 });
			p->setLifetime(10.f);
			p->setLife(100.f);
			p->setID(playerid++);
			p->setTeam(TEAM::RED);
			p->SetSeq(3, 1, 4, 4);
			m_Player.push_back(p);
		}

		m_AnimationTime += timeElapsed;
		m_ClimateTime += timeElapsed;
		if (m_AnimationTime > 0.1)
		{
			m_AnimationTime = 0.f;
			if (p1AnimationSeqX++ > 3)
			{
				p1AnimationSeqX = 0;
				if (p1AnimationSeqY++ > 1)
					p1AnimationSeqY = 0;
			}

			if (p2AnimationSeqX++ > 7)
			{
				p2AnimationSeqX = 0;
			}
		}

		if (m_ShakeTime > 0) {
			m_ShakeTime -= timeElapsed * 10;
			m_ShakeStrength -= timeElapsed * 10;
			m_Renderer->SetSceneTransform(cos(m_ShakeTime * 10) * m_ShakeStrength, cos(m_ShakeTime * 25) * m_ShakeStrength, 1, 1);
		}
		else
		{
			m_Renderer->SetSceneTransform(0, 0, 1, 1);
		}
	}
	else if (GameStatus == GAMESTATUS::PAUSE)
	{
		g_Timer->getTimeset();
		double timeElapsed = g_Timer->getTimeElapsed();
	}
}

void Scene::render()
{
	m_Renderer->DrawTexturedRect(0, 0, 0, 820, 0.5f, 0.5f, 0.5f, 1.0f, BackGroundTex, 0.9);
	m_Renderer->DrawParticleClimate(0, 0, 0, 1, 1, 1, 1, 1, -0.5, -0.5, SnowParticle, m_ClimateTime, 0.01);

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
	{
		if (p->getTeam() == TEAM::RED) 
			p->render(m_Renderer, PlayerTex[0]);

		else if (p->getTeam() == TEAM::BLUE)
			p->render(m_Renderer, PlayerTex[1]);
	}

	for (auto bullet : m_Bullet)
		bullet->render(m_Renderer, ParticleTex);
	
	for (auto arrow : m_Arrow)
		arrow->render(m_Renderer, ParticleTex);
	
	if (GameStatus == GAMESTATUS::BLUEWIN)
		m_Renderer->DrawTextW(-10, 300, GLUT_BITMAP_HELVETICA_18, 1, 0, 1, "You Win!");
	else if(GameStatus == GAMESTATUS::REDWIN)
		m_Renderer->DrawTextW(-10, -300, GLUT_BITMAP_HELVETICA_18, 1, 0, 1, "You LOSE!");
	else if(GameStatus == GAMESTATUS::PAUSE)
		m_Renderer->DrawTextW(-10, 100, GLUT_BITMAP_HELVETICA_18, 1, 0, 1, "PAUSE");
}

