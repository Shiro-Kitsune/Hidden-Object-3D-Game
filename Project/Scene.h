#pragma once

class Window;

// An abstract scene class that is base for all scenes that load and render models, light sources, etc.
class Scene
{
public:
	virtual ~Scene() = default;
	// Loads all models and does other required initialization tasks
	virtual void initialize(Window* _window);
	// Renders the scene using aspect ratio passed as arguments
	virtual void render(float deltaTime) = 0;
	// Process keyboard or mouse key events. Returns true when change to the next scene is requested
	virtual bool processKeyEvent(int key, int action) = 0;
	// Process mouse wheel event
	virtual void processMouseScroll(double yoffset) = 0;
	// Process mouse movement event
	virtual void processMouseMovement(double xpos, double ypos) = 0;
protected:
	Window* window;
};