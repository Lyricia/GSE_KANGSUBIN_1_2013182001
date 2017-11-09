#include "stdafx.h"
#include "Renderer.h"
#include "Object.h"


void Object::releaseObject()
{
}

bool Object::wallchk(RECT b)
{
	if (m_Position.x > 240 || m_Position.x < -240) {
		setDirection(m_Direction.x *= -1, m_Direction.y, m_Direction.z);
	}
	if (m_Position.y > 240 || m_Position.y < -240) {
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
		setColor(COLOR{ 1,0,1,1 });
		return true;
	}
	else {
		releaseTarget();
		target->releaseTarget();
		setColor(COLOR{ 1,0,0,1 });
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
	move(timeElapsed);
}

void Player::render(Renderer* g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
}

bool Player::cooltimeChk(const double timeElapsed)
{
	m_cooltime += timeElapsed;
	if (m_cooltime > 1)
	{
		m_cooltime = 0;
		return true;
	}
	return false;
}

Projectile* Player::shoot()
{
	Projectile* a = new Projectile(OBJTYPE::OBJ_ARROW, 20, Vector3D<float>{0, 0, 0});
	a->setPosition(getPosition());
	a->setColor(COLOR{ 0, 0, 1,1 });
	a->setLifetime(5);
	a->setLife(20);
	a->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
	a->setSpeed(300);
	return a;
}

void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, COLOR color)
{
	Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size, color.r, color.g, color.b, color.a);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Projectile* Building::ShootMissle()
{
	Projectile* m = new Projectile(OBJTYPE::OBJ_BULLET, 20, Vector3D<float>{0, 0, 0});
	m->setLifetime(5);
	m->setLife(20);
	m->setDirection(Vector3D<float>((rand() % 9 - 4.5f), (rand() % 9 - 4.5f), 0.f).Normalize());
	m->setSpeed(300);
	m->setColor(COLOR{ 0,0,1,1 });
	return m;
}

bool Building::cooltimeChk(const double timeElapsed)
{
	m_cooltime += timeElapsed;
	if (m_cooltime > 0.5)
	{
		m_cooltime = 0;
		return true;
	}
	return false;
}

void Building::update(const double timeElapsed)
{
}

void Building::render(Renderer * g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
}

//////////////////////////////////////////////////////////////////////////////////////////////////


void Projectile::update(const double timeElapsed)
{
	move(timeElapsed);
}

void Projectile::render(Renderer * g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
}
