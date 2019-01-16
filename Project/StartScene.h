#pragma once

#include <string>

#include "Scene.h"
#include "Shader.h"

class TextModel;

// Start scene that asks player to enter player's name
class StartScene : public Scene
{
public:
	~StartScene();
	// Compiles shaders and loads scene elements
	// Compiling shaders in constructor throws exception so scene must be initialized after its construction
	void initialize(Window* _window) override;
	// Renders the scene
	void render(float deltaTime) override;
	// Process keyboard or mouse key events. Returns true when change to the next scene is requested
	bool processKeyEvent(int key, int action) override;
	// Process mouse wheel event
	void processMouseScroll(double yoffset);
	// Process mouse movement event
	void processMouseMovement(double xpos, double ypos);
private:
	Shader shader;
	TextModel* textModel;
	std::string userInput;
};