#pragma once

class Object
{
protected:
	int					m_id;
	Vector3D<float>		m_Position;
	Vector3D<float>		m_Velocity;
	int					m_Size;


public:
	Object() {};
	Object(int id, int size, Vector3D<float> pos) : m_id(id), m_Position(pos),m_Size(size) {};
	~Object() {};

	Vector3D<float>	getPosition() { return m_Position; }
	Vector3D<float>	getVelocity() { return m_Velocity; }
	int				getSize() { return m_Size; }

	void setPosition(int x, int y, int z) { m_Position = { x, y, z }; }
	void setPosition(Vector3D<float> pos) { m_Position = pos; }
	void setVelocity(int x, int y, int z) { m_Velocity = { x, y, z }; }
	void move() { m_Position += m_Velocity; }
	void move(Vector3D<float> vel) { m_Position += vel; }

	virtual void update() = 0;
	virtual void render() = 0;
};


class Player : public Object
{
private:

public:
	Player() {};
	Player(int id, int size, Vector3D<float> pos) :Object(id, size, pos) {};
	~Player() {};

	virtual void update();
	virtual void render();

	bool colisionchk(RECT b);

};