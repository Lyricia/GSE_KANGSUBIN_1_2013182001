#include "stdafx.h"
#include "Renderer.h"
#include "Object.h"

void Object::releaseObject()
{
}

bool Object::collisionchk(RECT b)
{
	if (m_Position.x > b.right || m_Position.x < b.left) {
		setVelocity(m_Velocity.x *= -1, m_Velocity.y, m_Velocity.z);
	}
	if (m_Position.y > b.top || m_Position.y < b.bottom) {
		setVelocity(m_Velocity.x, m_Velocity.y *= -1, m_Velocity.z);
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

//////////////////////////////////////////////////////////////////////////////////////////////////
void Player::update()
{
	move();
}

void Player::render(Renderer* g_render)
{
	DrawSolidRectByMatrix(m_Position, g_render, m_Size, m_Color);
}

void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, COLOR color)
{
	Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size, color.r, color.g, color.b, color.a);
}

