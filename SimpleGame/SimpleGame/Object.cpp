#include "stdafx.h"
#include "Renderer.h"
#include "Object.h"

void Object::releaseObject()
{
}

bool Object::wallchk(RECT b)
{
	if (m_Position.x > 250 || m_Position.x < -250) {
		setDirection(m_Direction.x *= -1, m_Direction.y, m_Direction.z);
	}
	if (m_Position.y > 250 || m_Position.y < -250) {
		setDirection(m_Direction.x, m_Direction.y *= -1, m_Direction.z);
	}
	return true;
}

bool Object::isIntersect(Object* target)
{
	if (abs(m_Position.x - target->getPosition().x) < m_Size &&
		abs(m_Position.y - target->getPosition().y) < m_Size)
	{
		setTarget(target);
		target->setTarget(this);
		setColor(COLOR{ 1,0,1,0 });
		return true;
	}
	else {
		releaseTarget();
		target->releaseTarget();
		setColor(COLOR{ 1,0,0,0 });
	}
	return false;
}

void Object::resetObject()
{
	m_Position = { -500, -500, 0 };
	m_Direction = { 0,0,0 };
	m_Speed = 0;
	m_TargetBind = nullptr;
	m_Life = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Player::update(const double timeElapsed)
{
	if (m_Life > 0)
		m_Life -= timeElapsed;
	move(timeElapsed);
}

void Player::render(Renderer* g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
}

void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, COLOR color)
{
	Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size, color.r, color.g, color.b, color.a);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void Building::ShootMissle()
{
	Missle* m = new Missle(OBJTYPE::OBJ_BULLET, 10, Vector3D<float>{0, 0, 0});
	m->setLifetime(20);
	m->setDirection(Vector3D<float>((rand() % 5 - 2.5f), (rand() % 5 - 2.5f), 0.f).Normalize());
	m->setSpeed(300);
	m_Missle[m_MissleCounter] = m;
	m_MissleCounter++;
}

void Building::update(const double timeElapsed)
{
	m_shoottime += timeElapsed;
	if (m_shoottime > 1) {
		m_shoottime = 0;
		ShootMissle();
	}
	for (int i = 0; i < m_MissleCounter; ++i) {
		if (m_Missle[i]->isAlive()) {
			m_Missle[i]->update(timeElapsed);
		}
	}
}

void Building::render(Renderer * g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
	for (int i = 0; i < m_MissleCounter; ++i) {
		m_Missle[i]->render(g_render);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////


void Missle::update(const double timeElapsed)
{
	move(timeElapsed);
}

void Missle::render(Renderer * g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);

}
