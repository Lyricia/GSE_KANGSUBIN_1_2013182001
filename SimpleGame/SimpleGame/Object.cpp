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
	if ((m_Position.x - target->getPosition().x)*(m_Position.x - target->getPosition().x) +
		(m_Position.y - target->getPosition().y)*(m_Position.y - target->getPosition().y) <
		(m_Size+target->getSize())*(m_Size + target->getSize())*0.25)
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
	if (m_Lifetime > 0)
		//m_Lifetime -= timeElapsed;
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

Missle* Building::ShootMissle(const double timeElapsed)
{
	m_shoottime += timeElapsed;
	if (m_shoottime > 0.5) {
		m_shoottime = 0;
		Missle* m = new Missle(OBJTYPE::OBJ_BULLET, 2, Vector3D<float>{0, 0, 0});
		m->setLifetime(5);
		m->setLife(20);
		m->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
		m->setSpeed(300);
		return m;
	}
	return nullptr;
}

void Building::update(const double timeElapsed)
{
}

void Building::render(Renderer * g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
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
