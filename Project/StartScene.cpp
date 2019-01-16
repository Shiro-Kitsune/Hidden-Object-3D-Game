#include "StartScene.h"

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include "Window.h"
#include "TextModel.h"

StartScene::~StartScene()
{
	delete textModel;
}

void StartScene::initialize(Window* _window)
{
	Scene::initialize(_window);

	userInput = "_";
	shader.compile("TextVertexShader.vs", "TextFragmentShader.fs");
	// Create text
	textModel = new TextModel("../Assets/fonts/Holstein.DDS");
}

void StartScene::render(float deltaTime)
{
	shader.bind();
	shader.bindUniform("halfScreenSize", glm::vec2(window->getScreenWidth() / 2, window->getScreenHeight() / 2));
	
	// Render instruction text
	float fontSize = 26.0f;
	float x = window->getScreenWidth() / 2 - fontSize * 22;
	float y = window->getScreenHeight() - fontSize * 7;
	textModel->setTextToRender("Enter your name and press Enter to start game", x, y, fontSize);
	textModel->render(shader);
	// Render user input
	x = window->getScreenWidth() / 2 - fontSize * userInput.length() / 2;
	y = window->getScreenHeight() - fontSize * 10;
	textModel->setTextToRender(userInput, x, y, fontSize);
	textModel->render(shader);

	shader.unbind();
}

bool StartScene::processKeyEvent(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		// Change scene to the next scene
		if (key == GLFW_KEY_ENTER)
		{
			userInput.pop_back(); // Remove slash from the user name
			window->setPlayerName(userInput);
			return true;
		}
		// Add character to user input
		else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
		{
			userInput.pop_back();      // Remove slash from user name
			userInput.push_back(key);  // Append entered character
			userInput.push_back('_');  // Append slash again
		}
		// Remove last character from user input
		else if (key == GLFW_KEY_BACKSPACE)
		{
			if (userInput.length() > 1)
			{
				userInput.pop_back();      // Remove slash from user name
				userInput.pop_back();      // Remove last character
				userInput.push_back('_');  // Append slash again
			}
		}
	}
	return false;
}

void StartScene::processMouseScroll(double yoffset)
{
}

void StartScene::processMouseMovement(double xpos, double ypos)
{
}
