#pragma once

struct COLOR {
	float r;
	float g;
	float b;
	float a;
};

class Object
{
protected:
	int					m_id;
	Vector3D<float>		m_Position;
	Vector3D<float>		m_Velocity;
	int					m_Size;
	RECT				m_BoundingBox;
	COLOR				m_Color;

	Object*				m_TargetBind = NULL;

public:
	Object() {};
	Object(int id, int size, Vector3D<float> pos) : m_id(id), m_Position(pos),m_Size(size) {};
	~Object() {};
	void releaseObject();

	Vector3D<float>	getPosition() { return m_Position; }
	Vector3D<float>	getVelocity() { return m_Velocity; }
	int				getSize() { return m_Size; }

	void setPosition(float x, float y, float z) { m_Position = { x, y, z }; }
	void setPosition(Vector3D<float> pos) { m_Position = pos; }
	void setVelocity(float x, float y, float z) { m_Velocity = { x, y, z }; }
	void setOOBB(RECT boundingbox) { m_BoundingBox = boundingbox; }
	void setColor(COLOR color) { m_Color = color; }

	void move() { m_Position += m_Velocity; }
	void move(Vector3D<float> vel) { m_Position += vel; }

	void setTarget(Object* target) { m_TargetBind = target; }
	Object* getTarget() { return m_TargetBind; }
	void releaseTarget() { m_TargetBind = NULL; }

	bool collisionchk(RECT b);
	bool isIntersect(Object* target);

	virtual void update() = 0;
	virtual void render(Renderer* g_render) = 0;
};


class Player : public Object
{
private:

public:
	Player() {};
	Player(int id, int size, Vector3D<float> pos) :Object(id, size, pos) {};
	~Player() {};

	virtual void update();
	virtual void render(Renderer* g_render);
};


void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, COLOR color);