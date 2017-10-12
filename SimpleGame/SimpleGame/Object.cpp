#include "stdafx.h"
#include "Object.h"




void Player::update()
{
}

void Player::render()
{
}

bool Player::colisionchk(RECT b)
{
	if (m_Position.x > b.right || m_Position.x < b.left) {
		setVelocity(m_Velocity.x *= -1, m_Velocity.y, m_Velocity.z);
	}
	if (m_Position.y > b.top || m_Position.y < b.bottom) {
		setVelocity(m_Velocity.x, m_Velocity.y *= -1 , m_Velocity.z);
	}
	return true;
}

