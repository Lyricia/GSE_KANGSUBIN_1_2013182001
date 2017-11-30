#pragma once
#include "Object.h"

#define MAX_OBJECT 100

class Timer;

class Scene
{
private:
	Building*			m_Building[6];
	list<Player*>		m_Player;
	list<Projectile*>	m_Bullet;
	list<Projectile*>	m_Arrow;

	Renderer*	m_Renderer = nullptr;
	Timer*		g_Timer = nullptr;

	int			playerid = 0;
	int			BuildingTex[2];
	int			PlayerTex[2];
	int			BackGroundTex;
	int			ParticleTex;

	double		m_RedTeamCreateTimer = 0.f;
	double		m_BlueTeamCreateTimer = 0.f;
	double		m_AnimationTime = 0.f;
	int			p1AnimationSeqX = 0, p1AnimationSeqY = 0;
	int			p2AnimationSeqX = 0, p2AnimationSeqY = 0;
public:
	Scene();
	~Scene();
	void releaseScene();

	void buildScene();
	//void setRenderer(Renderer* g_render) { g_renderer = g_render; }
	void setTimer(Timer* t) { g_Timer = t; }

	void keyinput(unsigned char key);
	void keyspcialinput(int key);
	void mouseinput(int button, int state, int x, int y);

	void update();
	void render();
};