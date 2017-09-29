#pragma once

class Object
{
private:
	int					m_id;
	Vector3D<float>		m_Position;
	int					m_Size;

public:
	Object() {};
	Object(int id, int size, Vector3D<float> pos) : m_id(id), m_Position(pos),m_Size(size) {};
	~Object() {};

	Vector3D<float>	getPosition() { return m_Position; }
	int				getSize() { return m_Size; }

	void setPosition(int x, int y, int z) { m_Position = { x, y, z }; }
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

};