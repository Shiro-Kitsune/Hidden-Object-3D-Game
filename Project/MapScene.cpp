#include "MapScene.h"

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include "Window.h"
#include "Model2D.h"
#include "TextModel.h"

MapScene::~MapScene()
{
	delete textModel;
	delete mapModel;
}

void MapScene::initialize(Window* _window)
{
	Scene::initialize(_window);

	shader.compile("TextVertexShader.vs", "TextFragmentShader.fs");
	// Load map texture and create model to render it
	float mapSize = 500.0f;
	float x = window->getScreenWidth() / 2 - mapSize / 2;
	float y = window->getScreenHeight() / 2 - mapSize / 2;
	mapModel = new Model2D("../Assets/city_map/map.png", x, y, mapSize);
	// Create text
	textModel = new TextModel("../Assets/fonts/Holstein.DDS");
	textModel->setTextToRender("Press Enter to start game", x - 150, y - 40, 30);
}

void MapScene::render(float deltaTime)
{
	shader.bind();
	shader.bindUniform("halfScreenSize", glm::vec2(window->getScreenWidth() / 2, window->getScreenHeight() / 2));
	textModel->render(shader);
	mapModel->render(shader);
	shader.unbind();
}

bool MapScene::processKeyEvent(int key, int action)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

void MapScene::processMouseScroll(double yoffset)
{
}

void MapScene::processMouseMovement(double xpos, double ypos)
{
}
