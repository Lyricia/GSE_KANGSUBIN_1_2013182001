#pragma once
class Object
{
private:
	int					m_id;
	Vector3D<int>		m_Position;

public:
	Object() {};
	Object(int id, Vector3D<int> pos) : m_id(id), m_Position(pos) {};
	~Object() {};

	virtual void update() = 0;
	virtual void render() = 0;
};


class Player : public Object
{
private:

public:
	Player() {};
	Player(int id, Vector3D<int> pos) :Object(id, pos) {};
	~Player() {};

	virtual void update();
	virtual void render();

};