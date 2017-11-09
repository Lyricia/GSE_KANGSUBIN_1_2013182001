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
	OBJTYPE				m_type;
	Vector3D<float>		m_Position;
	Vector3D<float>		m_Direction;
	float				m_Speed;
	int					m_Size;
	RECT				m_BoundingBox;
	COLOR				m_DefaultColor;
	COLOR				m_Color;

	Object*				m_TargetBind = NULL;

	double				m_Life = 0;
	double				m_Lifetime = 0;

public:
	Object() {};
	Object(OBJTYPE type, int size, Vector3D<float> pos) : m_type(type), m_Position(pos),m_Size(size) {};
	~Object() {};
	void releaseObject();

	Vector3D<float>	getPosition() { return m_Position; }
	int				getSize() { return m_Size; }
	bool			isAlive() { if (m_Life <= 0 || m_Lifetime <= 0) return false; else return true; }

	void setPosition(float x, float y, float z) { m_Position = { x, y, z }; }
	void setPosition(Vector3D<float> pos) { m_Position = pos; }
	void setDirection(Vector3D<float> dir) { m_Direction = dir; }
	void setDirection(float x, float y, float z) { m_Direction = { x, y, z }; }
	void setSpeed(float speed) { m_Speed = speed; }
	void setOOBB(RECT boundingbox) { m_BoundingBox = boundingbox; }
	void setColor(COLOR color) { m_Color = color; }
	void setID(int id) { m_id = id; }
	void setLifetime(double lifetime) { m_Lifetime = lifetime; }
	void setLife(double life) { m_Life = life; }
	
	void decreaseLife(double dmg) { m_Life -= dmg; }

	void move(const double timeElapsed) { m_Position += m_Direction * m_Speed * timeElapsed; };

	void setTarget(Object* target) { m_TargetBind = target; }
	Object* getTarget() { return m_TargetBind; }
	OBJTYPE getType() { return m_type; }

	double getLife() { return m_Life; }
	double getLifetime() { return m_Lifetime; }
	const int getID() { return m_id; }
	void releaseTarget() { m_TargetBind = nullptr; }

	bool wallchk(RECT b);
	bool isIntersect(Object* target);

	virtual void update(const double timeElapsed) = 0;
	virtual void render(Renderer* g_render) = 0;

	void resetObject();
};

class Projectile : public Object
{
private:
	int		m_Owner;

public:
	Projectile() {};
	Projectile(OBJTYPE type, int size, Vector3D<float> pos) : Object(type, size, pos) {};
	~Projectile() {};

	void setOwner(int id) { m_Owner = id; }
	int getOwner() { return m_Owner; }
	virtual void update(const double timeElapsed);
	virtual void render(Renderer* g_render);
};

class Player : public Object
{
private:
	float	m_cooltime = 0;

public:
	Player() {};
	Player(OBJTYPE type, int size, Vector3D<float> pos) :Object(type, size, pos) {};
	~Player() {};

	virtual void update(const double timeElapsed);
	virtual void render(Renderer* g_render);
	Projectile* shoot();
	bool cooltimeChk(const double timeElapsed);
};

class Building : public Object
{
private:
	double		m_cooltime = 0.f;
	double		m_damagedtime = 0.f;

public:
	Building() {};
	Building(int size, Vector3D<float> pos) : Object(OBJTYPE::OBJ_BUILDING, size, pos) {}
	~Building() {};

	Projectile* ShootMissle();
	bool cooltimeChk(const double timeElapsed);
	virtual void update(const double timeElapsed);
	virtual void render(Renderer* g_render);
};

void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, COLOR color);