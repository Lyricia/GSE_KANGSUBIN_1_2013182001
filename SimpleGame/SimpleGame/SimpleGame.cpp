/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"
#include "Timer.h"

Renderer	*g_Renderer = NULL;
Scene*		CurrentScene;
Timer*		g_Timer;

void RenderScene(void)
{
	CurrentScene->update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	CurrentScene->render();

	glutSwapBuffers();
}

void Initialize()
{
	srand((unsigned)time(NULL));

	g_Timer = new Timer();
	g_Timer->Init();

	CurrentScene = new Scene();
	CurrentScene->setRenderer(g_Renderer);
	CurrentScene->setTimer(g_Timer);
	CurrentScene->buildScene();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	x = x - WINDOW_WIDTH_HALF;
	y = -(y - WINDOW_HEIGHT_HALF);

	CurrentScene->mouseinput(button, state, x, y);
	RenderScene();
}

void MouseMove(int x, int y)
{
	x = x - WINDOW_WIDTH_HALF;
	y = -(y - WINDOW_HEIGHT_HALF);
	CurrentScene->mouseinput(0, 0, x, y);
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	CurrentScene->keyinput(key);
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	CurrentScene->keyspcialinput(key);
	RenderScene();
}

void SceneChanger(Scene* scene) {
	CurrentScene = scene;
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");



	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(500, 500);
	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	
	
	
	Initialize();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);
	glutPassiveMotionFunc(MouseMove);
	glutMainLoop();

	CurrentScene->releaseScene();
	delete g_Renderer;

    return 0;
}

