#include "stdafx.h"
#include "Object.h"
#include "Renderer.h"
#include "Scene.h"

void Scene::Init()
{
	dummy = new Player(1, 50, Vector3D<float>(0, 0, 0));
	dummy2 = new Player(1, 50, Vector3D<float>(0, 0, 0));
	dummy2->setVelocity(2, 1, 0);

	screenOOBB.bottom = -WINDOW_HEIGHT_HALF;
	screenOOBB.top = WINDOW_HEIGHT_HALF;
	screenOOBB.left = -WINDOW_WIDTH_HALF;
	screenOOBB.right = WINDOW_WIDTH_HALF;
}

Scene::Scene()
{
	Init();
}


Scene::~Scene()
{
}

void Scene::keyinput(unsigned char key)
{
	switch (key) {
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
}

void Scene::keyspcialinput(int key)
{
	switch (key) {
	
	default:
		break;
	}
}

// ��Ƽ�½�ŷ�� �����ϰ� �� �ٿ� �۷ι��� ������ �־���Ѵ�.
// �ۿ��� ������ �ȿ��� �� �� ���� �ֱ� ������
void Scene::mouseinput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (dummyon)		dummyon = false;
		else if (!dummyon)	dummyon = true;
		dummy2->setPosition(dummy->getPosition());
		dummy2->setVelocity(rand() % 6 - 3, rand() % 6 - 3, 0);
	}

	dummy->setPosition(x, y, 0);
}

void Scene::update()
{
	dummy2->colisionchk(screenOOBB);
	dummy2->move();

}

void Scene::render()
{
	int dummysize = dummy->getSize();
	//g_renderer->DrawSolidRect(dummy->getPosition().x, dummy->getPosition().y, dummy->getPosition().z, 50, 1, 0, 0, 0);

	if(dummyon)
		DrawSolidRectByMatrix(dummy->getPosition(), g_renderer, dummysize, 1, 0, 1, 0);

	DrawSolidRectByMatrix(dummy2->getPosition(), g_renderer, dummysize, 1, 0, 1, 0);
	g_renderer->DrawSolidRect(WINDOW_WIDTH_HALF - dummysize / 2, WINDOW_HEIGHT_HALF - dummysize / 2, 0, dummysize, 1, 0.1, 0, 0);
	g_renderer->DrawSolidRect(WINDOW_WIDTH_HALF - dummysize / 2, -WINDOW_HEIGHT_HALF + dummysize / 2, 0, dummysize, 1, 1, 0, 0);
	g_renderer->DrawSolidRect(-WINDOW_WIDTH_HALF + dummysize / 2, WINDOW_HEIGHT_HALF - dummysize / 2, 0, dummysize, 1, 1, 0, 0);
	g_renderer->DrawSolidRect(-WINDOW_WIDTH_HALF + dummysize / 2, -WINDOW_HEIGHT_HALF + dummysize / 2, 0, dummysize, 1, 0.1, 0, 0);
}


void DrawSolidRectByMatrix(Vector3D<float> pos, Renderer* Renderer, int size, float r, float g, float b, float a)
{
	Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size, r, g, b, a);
}




