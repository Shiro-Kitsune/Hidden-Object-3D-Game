#pragma once

#include <vector>
#include <map>
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "PlayerData.h"

class Model;
class SkyBoxModel;
class TextModel;
class HiddenObject;
class Model2D;

// A scene class that loads, stores and renders all game models and light sources and contains game logic
class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
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
	enum class CameraMovementState {
		NONE,
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	Shader shader, discardShader, skyboxShader, textShader;
	Camera camera;
	CameraMovementState cameraState;
	DirectionalLight directionalLight;
	SpotLight spotLight;
	PointLight pointLight;
	std::vector<Model*> models;
	std::vector<Model*> discardModels;
	std::vector<Model*> blendModels;
	SkyBoxModel* skybox = nullptr;
	TextModel* textModel = nullptr;
	std::vector<HiddenObject*> hiddenObjects;
	int objectsFound;
	std::vector<glm::vec3> spawnPoints;
	std::vector<Model2D*> hiddenObjectIcons;
	std::map<std::string, PlayerData> players;
	std::string recordFileName;
	bool printPlayers;

	bool initialized;
	float totalTimeElapsed;

	// Window properties
	float aspectRatio;

	// Mouse movement data
	bool firstMouse;
	float lastX;
	float lastY;

	// Loads and compiles shaders
	void loadShaders();
	// Load scene file
	void loadScene();
	// Loads all models
	void loadModels(XMLElement* element);
	// Load hidden game objects
	void loadGameObjects(XMLElement* element);
	// Load spawn points for hidden objects
	void loadSpawnPoints(XMLElement* element);
	// Loads light sources
	void loadLightSources(XMLElement* element);
	// Update camera based on elapsed time from last frame and current state
	void updateCamera(float deltaTime);
	// Renders text with the elapsed time
	void printElapsedTime();
	// Load players and their best time from file
	void loadPlayerData();
	void addPlayerData(const PlayerData& player);
	// Save players and their best time from file
	void savePlayerData();
	// Internal render functions
	void renderPlayerList();
};