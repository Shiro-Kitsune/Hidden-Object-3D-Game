#include "Window.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h> 

#include "Scene.h"
#include "GameScene.h"
#include "MapScene.h"
#include "StartScene.h"

Window::Window(int _width, int _height, const std::string & _title)
{
	// initialize glfw
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwSetErrorCallback(Window::error_cb);

	width = static_cast<float>(_width);
	height = static_cast<float>(_height);
	title = _title;
	window = std::unique_ptr<GLFWwindow, GLFWDeleter>(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));


	// Have the glfwwindow know about custom window class
	glfwSetWindowUserPointer(window.get(), this);
	glfwSetMouseButtonCallback(window.get(), mouse_button_cb);
	glfwSetScrollCallback(window.get(), scroll_callback);
	glfwSetCursorPosCallback(window.get(), mouse_callback);
	glfwSetKeyCallback(window.get(), keyboard_cb);
	glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwMakeContextCurrent(window.get());

	// glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
	// Enable back face culling
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

	// Create all scenes in reverse order
	scenes.push_back(new GameScene());
	scenes.push_back(new MapScene());
	scenes.push_back(new StartScene());
}

Window::~Window()
{
	for (const auto& scene : scenes)
	{
		delete scene;
	}
}

void Window::run()
{
	// Initialize scene before rendering it
	scenes.back()->initialize(this);	

	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window.get()))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		render(deltaTime);
	}
}

void Window::error_cb(int error, const char * description)
{
	std::cout << "Error (" << error << "): " << description << std::endl;
}

void Window::keyboard_cb(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	win->keyboardPress(key, action);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	win->mouseScroll(yoffset);
}

void Window::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	win->mouseMovement(xpos, ypos);
}

void Window::keyboardPress(int button, int action)
{
	bool goToNextScene = scenes.back()->processKeyEvent(button, action);
	if (goToNextScene)
	{
		delete scenes.back();
		scenes.pop_back();
		scenes.back()->initialize(this);
	}
	// Close application
	if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window.get(), GL_TRUE);
	}
}

void Window::mouse_button_cb(GLFWwindow * window, int button, int action, int mods)
{
	auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	win->mouseClick(button, action, mods);
}

void Window::mouseClick(int button, int action, int mods)
{
	scenes.back()->processKeyEvent(button, action);
}

void Window::mouseScroll(double yoffset)
{
	scenes.back()->processMouseScroll(yoffset);
}

void Window::mouseMovement(double xpos, double ypos)
{
	scenes.back()->processMouseMovement(xpos, ypos);
}

void Window::render(float deltaTime) const
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	scenes.back()->render(deltaTime);

	glfwSwapBuffers(window.get());
}