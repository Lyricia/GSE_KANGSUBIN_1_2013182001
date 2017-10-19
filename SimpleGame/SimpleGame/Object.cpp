#include "stdafx.h"
#include "Renderer.h"
#include "Object.h"

void Object::releaseObject()
{
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

bool Player::collisionchk(RECT b)
{
	if (m_Position.x > b.right || m_Position.x < b.left) {
		setVelocity(m_Velocity.x *= -1, m_Velocity.y, m_Velocity.z);
	}
	if (m_Position.y > b.top || m_Position.y < b.bottom) {
		setVelocity(m_Velocity.x, m_Velocity.y *= -1 , m_Velocity.z);
	}
	return true;
}

bool Player::isIntersect(Vector3D<float> pos)
{
	//Vector3D<float> center = { float(b.left + b.right) / 2, float(b.top + b.bottom) / 2, 0.f };
	if ((m_Position.x - pos.x)*(m_Position.x - pos.x) < (m_Size * 2)*(m_Size * 2) &&
		(m_Position.y - pos.y)*(m_Position.y - pos.y) < (m_Size * 2)*(m_Size * 2))
		return true;
	else
		return false;
}

void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, COLOR color)
{
	Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size, color.r, color.g, color.b, color.a);
}

