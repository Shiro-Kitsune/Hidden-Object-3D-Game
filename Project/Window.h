#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW\glfw3.h>

struct GLFWDeleter {
	void operator()(GLFWwindow* ptr) {
		glfwDestroyWindow(ptr);
	}
};

typedef std::unique_ptr<GLFWwindow, GLFWDeleter> SmartGLFWwindow;

class Scene;

class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window();
	void run();
	float getScreenWidth() const { return width; }
	float getScreenHeight() const { return height; }
	const std::string getPlayerName() const { return playerName; }
	void setPlayerName(const std::string name) { playerName = name; }
protected:
	static void error_cb(int error, const char* description);
	static void keyboard_cb(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void mouse_button_cb(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void keyboardPress(int button, int action);
	void mouseClick(int button, int action, int mods);
	void mouseScroll(double yoffset);
	void mouseMovement(double xpos, double ypos);

	void render(float deltaTime) const;

	//Window specific data
	float width;
	float height;
	std::string title;
	SmartGLFWwindow window;

	std::vector<Scene*> scenes;
	std::string playerName;
};
