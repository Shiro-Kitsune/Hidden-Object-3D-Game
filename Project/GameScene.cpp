#include "GameScene.h"

#include <algorithm>
#include <time.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "glm/gtx/string_cast.hpp"

#include "tinyxml2.h"
#include "Window.h"
#include "SkyBoxModel.h"
#include "TextModel.h"
#include "Camera.h"
#include "HiddenObject.h"
#include "Model3D.h"
#include "Model2D.h"

using namespace tinyxml2;

GameScene::GameScene() : camera(vec3(1.f, 1.f, 3.f)), firstMouse(true)
{
}

GameScene::~GameScene()
{
	delete skybox;
	delete textModel;

	for (auto object : models)
	{
		delete object;
	}

	for (auto object : discardModels)
	{
		delete object;
	}

	for (auto object : blendModels)
	{
		delete object;
	}

	for (auto object : hiddenObjects)
	{
		delete object;
	}

	for (auto object : hiddenObjectIcons)
	{
		delete object;
	}
}

void GameScene::initialize(Window* _window)
{
	Scene::initialize(_window);

	loadShaders();
	loadScene();
	loadPlayerData();

	aspectRatio = window->getScreenWidth() / window->getScreenHeight();
	firstMouse = true;
	initialized = true;
}

void GameScene::loadShaders()
{
	shader.compile("VertexShader.vs", "BlendFragmentShader.fs");
	discardShader.compile("VertexShader.vs", "DiscardFragmentShader.fs");
	skyboxShader.compile("SkyboxVertexShader.vs", "SkyboxFragmentShader.fs");
	textShader.compile("TextVertexShader.vs", "TextFragmentShader.fs");
}

void GameScene::loadScene()
{
	std::string gameFile = "../Assets/GameData.xml";
	XMLDocument document;
	document.LoadFile(gameFile.c_str());
	if (document.Error())
	{
		std::string msg = "Unable to load " + gameFile;
		throw std::exception(msg.c_str());
	}

	XMLElement* gameElement = document.FirstChildElement("Game");
	if (gameElement == nullptr) throw std::exception("Game element is missing");

	// Load static models of a scene (decorations)
	XMLElement* staticModelsElement = gameElement->FirstChildElement("StaticModels");
	if (staticModelsElement != nullptr)
	{
		loadModels(staticModelsElement);
	}

	// Load hidden game objects
	XMLElement* hiddenObjectsElement = gameElement->FirstChildElement("HiddenObjects");
	if (hiddenObjectsElement != nullptr)
	{
		loadGameObjects(hiddenObjectsElement);
	}

	// Load text rendering model
	XMLElement* fontElement = gameElement->FirstChildElement("Font");
	if (fontElement != nullptr)
	{
		textModel = new TextModel(fontElement->GetText());
	}

	// Load spawn points for game objects
	XMLElement* spawnPointsElement = gameElement->FirstChildElement("SpawnPoints");
	if (spawnPointsElement != nullptr)
	{
		loadSpawnPoints(spawnPointsElement);
	}
	
	// Load light sources
	XMLElement* lightElement = gameElement->FirstChildElement("Light");
	if (lightElement != nullptr)
	{
		loadLightSources(lightElement);
	}

	// Load Player Stats File name
	XMLElement* playerStatsFileElement = gameElement->FirstChildElement("PlayerStatsFile");
	if (playerStatsFileElement != nullptr)
	{
		recordFileName = playerStatsFileElement->GetText();
	}
}

void GameScene::loadModels(XMLElement* element)
{
	// Load city (opaque objects only)
	XMLElement* opaqueModelsElement = element->FirstChildElement("OpaqueModels");
	if (opaqueModelsElement != nullptr)
	{
		XMLElement* modelElement = opaqueModelsElement->FirstChildElement("Model");
		while (modelElement != nullptr)
		{
			auto filePath = modelElement->GetText();
			if (filePath != nullptr)
			{
				models.push_back(new Model3D(filePath));
			}
			modelElement = modelElement->NextSiblingElement("Model");
		}	
	}

	// Load city trees
	XMLElement* transparentModelsElement = element->FirstChildElement("TransparentModels");
	if (transparentModelsElement != nullptr)
	{
		XMLElement* modelElement = transparentModelsElement->FirstChildElement("Model");
		while (modelElement != nullptr)
		{
			auto filePath = modelElement->GetText();
			if (filePath != nullptr)
			{
				blendModels.push_back(new Model3D(filePath));
			}
			modelElement = modelElement->NextSiblingElement("Model");
		}
	}

	// Load skybox
	XMLElement* skyboxElement = element->FirstChildElement("Skybox");
	if (skyboxElement != nullptr)
	{
		std::vector<std::string> faces;
		XMLElement* faceElement = skyboxElement->FirstChildElement("Face");
		while (faceElement != nullptr)
		{
			faces.push_back(faceElement->GetText());
			faceElement = faceElement->NextSiblingElement("Face");
		}
		skybox = new SkyBoxModel(faces);
	}
}

void GameScene::loadGameObjects(XMLElement * element)
{
	XMLElement* hiddenObjectElement = element->FirstChildElement("HiddenObject");
	while (hiddenObjectElement != nullptr)
	{
		std::string modelName;
		XMLElement* modelElement = hiddenObjectElement->FirstChildElement("Model");
		if (modelElement != nullptr)
		{
			modelName = modelElement->GetText();
		}

		std::string iconFileName;
		XMLElement* iconElement = hiddenObjectElement->FirstChildElement("Icon");
		if (iconElement != nullptr)
		{
			iconFileName = iconElement->GetText();
		}

		hiddenObjects.push_back(new HiddenObject(modelName, iconFileName));
		hiddenObjectElement = hiddenObjectElement->NextSiblingElement("HiddenObject");
	}
}

void GameScene::loadSpawnPoints(XMLElement* element)
{
	// Load spawn points from XML
	XMLElement* spawnPointElement = element->FirstChildElement("Point");
	while (spawnPointElement != nullptr)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		spawnPointElement->FirstChildElement("X")->QueryFloatText(&x);
		spawnPointElement->FirstChildElement("Y")->QueryFloatText(&y);
		spawnPointElement->FirstChildElement("Z")->QueryFloatText(&z);
		spawnPoints.push_back(vec3(x, y, z));
		spawnPointElement = spawnPointElement->NextSiblingElement("Point");
	}
	// Shuffle spawn points
	srand(time(0));
	std::random_shuffle(spawnPoints.begin(), spawnPoints.end());
}

void GameScene::loadLightSources(XMLElement * element)
{
	// Load directional light
	XMLElement* dirLightElement = element->FirstChildElement("DirectionalLight");
	if (dirLightElement != nullptr)
	{
		directionalLight.load(dirLightElement);
	}

	// Load spot light
	XMLElement* spotLightElement = element->FirstChildElement("SpotLight");
	if (spotLightElement != nullptr)
	{
		spotLight.load(spotLightElement);
	}

	// Load point light
	XMLElement* pointLightElement = element->FirstChildElement("PointLight");
	if (pointLightElement != nullptr)
	{
		pointLight.load(pointLightElement);
	}
}

void GameScene::updateCamera(float deltaTime)
{
	// Calculate camera velocity
	float velocity = camera.MovementSpeed * deltaTime;

	switch (cameraState)
	{
	case CameraMovementState::FORWARD:
		camera.Position += camera.Front * velocity;
		break;
	case CameraMovementState::BACKWARD:
		camera.Position -= camera.Front * velocity;
		break;
	case CameraMovementState::LEFT:
		camera.Position -= camera.Right * velocity;
		break;
	case CameraMovementState::RIGHT:
		camera.Position += camera.Right * velocity;
		break;
	case CameraMovementState::UP:
		camera.Position += camera.Up * velocity;
		break;
	case CameraMovementState::DOWN:
		camera.Position -= camera.Up * velocity;
		break;
	default:
		break;
	}
}

void GameScene::printElapsedTime()
{
	// Create string of the form hh:mm:ss from the elapsed time
	std::string timerText = PlayerData::formatTime(totalTimeElapsed);
	// Set text to render to TextModel object
	float letterSize = 20.0f;
	textModel->setTextToRender(timerText, 10, window->getScreenHeight() - letterSize, letterSize);
}

void GameScene::loadPlayerData()
{
	XMLDocument document;
	document.LoadFile(recordFileName.c_str());
	if (document.Error())
	{
		std::cout << "Unable to load " << recordFileName << std::endl;
		return;
	}

	XMLElement* gameElement = document.FirstChildElement("PlayerList");
	if (gameElement == nullptr) throw std::exception("PlayerList element is missing");

	XMLElement* playerElement = gameElement->FirstChildElement("PlayerData");
	while (playerElement != nullptr)
	{
		PlayerData player;
		player.load(playerElement);
		addPlayerData(player);
		playerElement = playerElement->NextSiblingElement("PlayerData");
	}
}

void GameScene::addPlayerData(const PlayerData & player)
{
	// Check if player with this name is already in map
	auto found = players.find(player.getPlayerName());
	if (found != players.end())
	{
		// If player already exists check if new time is the best time and replace record
		if (player.getGameTime() < found->second.getGameTime())
		{
			players.erase(player.getPlayerName());
		}
	}
	players.emplace(player.getPlayerName(), player);
}

void GameScene::savePlayerData()
{
	XMLDocument document;
	XMLElement* gameElement = document.NewElement("PlayerList");
	document.InsertEndChild(gameElement);

	for (auto& iter : players)
	{
		XMLElement* playerElement = gameElement->GetDocument()->NewElement("PlayerData");
		gameElement->InsertEndChild(playerElement);
		iter.second.save(playerElement);
	}

	document.SaveFile(recordFileName.c_str());
}

void GameScene::renderPlayerList()
{
	float letterSize = 20.0f;
	float x = window->getScreenWidth() / 2 - 10 * letterSize;
	int index = 3;
	for (auto& player : players)
	{
		std::string playerStr = player.second.getPlayerName() + "  " + player.second.getFormattedGameTime();
		textModel->setTextToRender(playerStr, x, window->getScreenHeight() - letterSize * index, letterSize);
		textModel->render(textShader);
		++index;
	}
}

void GameScene::render(float deltaTime)
{
	if (!initialized)
	{
		std::cout << "GameScene is not initialized!" << std::endl;
		return;
	}

	totalTimeElapsed += deltaTime;
	printElapsedTime();
	updateCamera(deltaTime);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// Calculate MVP matrices
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model;
	glm::mat4 inverseModel = glm::transpose(glm::inverse(model));
	// Use shader program
	shader.bind();
	// Set MVP matrices in shader
	shader.bindUniform("projection", projection);
	shader.bindUniform("view", view);
	shader.bindUniform("model", model);
	shader.bindUniform("inverseModel", inverseModel);
	shader.bindUniform("viewPos", camera.Position);
	// Set directional light properties in shader
	shader.bindUniform("dirLight.direction", directionalLight.direction);
	shader.bindUniform("dirLight.ambient", directionalLight.ambient);
	shader.bindUniform("dirLight.diffuse", directionalLight.diffuse);
	shader.bindUniform("dirLight.specular", directionalLight.specular);
	// Set spot light properties in shader
	shader.bindUniform("spotLight.position", camera.Position);
	shader.bindUniform("spotLight.direction", camera.Front);
	shader.bindUniform("spotLight.cutOff", spotLight.cutOff);
	shader.bindUniform("spotLight.outerCutOff", spotLight.outerCutOff);
	shader.bindUniform("spotLight.ambient", spotLight.ambient);
	shader.bindUniform("spotLight.diffuse", spotLight.diffuse);
	shader.bindUniform("spotLight.specular", spotLight.specular);
	shader.bindUniform("spotLight.constant", spotLight.constant);
	shader.bindUniform("spotLight.linear", spotLight.linear);
	shader.bindUniform("spotLight.quadratic", spotLight.quadratic);
	// Set point light properties in shader
	shader.bindUniform("pointLight.position", pointLight.position);
	shader.bindUniform("pointLight.ambient", pointLight.ambient);
	shader.bindUniform("pointLight.diffuse", pointLight.diffuse);
	shader.bindUniform("pointLight.specular", pointLight.specular);
	shader.bindUniform("pointLight.constant", pointLight.constant);
	shader.bindUniform("pointLight.linear", pointLight.linear);
	shader.bindUniform("pointLight.quadratic", pointLight.quadratic);

	// Draw opaque models
	for (const auto model : models)
	{
		model->render(shader);
	}

	// Draw hidden objects
	for (int i = 0; i < hiddenObjects.size(); ++i)
	{
		glm::mat4 model2;
		model2 = glm::translate(model2, spawnPoints[i]);
		shader.bindUniform("model", model2);
		hiddenObjects[i]->render(shader);

		// If camera is close enough to hidden object, it is considered found
		if (!hiddenObjects[i]->isFound() && glm::length(camera.Position - spawnPoints[i]) < 1.5f)
		{
			hiddenObjects[i]->setFound(true);
			// Find next x coordinate to place new icon on screen
			int x = 40 * hiddenObjectIcons.size();
			// Create icon model
			hiddenObjectIcons.push_back(new Model2D(hiddenObjects[i]->getIconFileName(), x, 10, 40));
			++objectsFound;
			// Game is completed when all objects are found, save new player time
			if (objectsFound == 5)
			{
				PlayerData player(window->getPlayerName(), totalTimeElapsed);
				addPlayerData(player);
				savePlayerData();
			}
		}
	}

	// Draw skybox
	skyboxShader.bind();
	skyboxShader.bindUniform("projection", projection);
	glm::mat4 skyboxView = glm::mat4(glm::mat3(view));  // Remove translation from skybox
	skyboxShader.bindUniform("view", skyboxView);
	skybox->render(skyboxShader);

	// Enable blending and set blending function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Transparent models must be rendered last (after skybox as well) for blending to work properly
	shader.bind();
	// Set MVP matrices in shader
	shader.bindUniform("projection", projection);
	shader.bindUniform("view", view);
	shader.bindUniform("model", model);
	shader.bindUniform("inverseModel", inverseModel);
	shader.bindUniform("viewPos", camera.Position);
	// Set directional light properties in shader
	shader.bindUniform("dirLight.direction", directionalLight.direction);
	shader.bindUniform("dirLight.ambient", directionalLight.ambient);
	shader.bindUniform("dirLight.diffuse", directionalLight.diffuse);
	shader.bindUniform("dirLight.specular", directionalLight.specular);
	// Set spot light properties in shader
	shader.bindUniform("spotLight.position", camera.Position);
	shader.bindUniform("spotLight.direction", camera.Front);
	shader.bindUniform("spotLight.cutOff", spotLight.cutOff);
	shader.bindUniform("spotLight.outerCutOff", spotLight.outerCutOff);
	shader.bindUniform("spotLight.ambient", spotLight.ambient);
	shader.bindUniform("spotLight.diffuse", spotLight.diffuse);
	shader.bindUniform("spotLight.specular", spotLight.specular);
	shader.bindUniform("spotLight.constant", spotLight.constant);
	shader.bindUniform("spotLight.linear", spotLight.linear);
	shader.bindUniform("spotLight.quadratic", spotLight.quadratic);
	// Set point light properties in shader
	shader.bindUniform("pointLight.position", pointLight.position);
	shader.bindUniform("pointLight.ambient", pointLight.ambient);
	shader.bindUniform("pointLight.diffuse", pointLight.diffuse);
	shader.bindUniform("pointLight.specular", pointLight.specular);
	shader.bindUniform("pointLight.constant", pointLight.constant);
	shader.bindUniform("pointLight.linear", pointLight.linear);
	shader.bindUniform("pointLight.quadratic", pointLight.quadratic);

	// Draw transparent models (must be last in order to blend with skybox properly)
	for (const auto model : blendModels)
	{
		model->render(shader);
	}

	// Disable depth test here so text and icons are always rendered on top of everything
	glDisable(GL_DEPTH_TEST);

	// Render Text
	textShader.bind();
	textShader.bindUniform("halfScreenSize", vec2(window->getScreenWidth() / 2, window->getScreenHeight() / 2));
	textModel->render(textShader);

	// Render found objects' icons
	for (const auto& icon : hiddenObjectIcons)
	{
		icon->render(textShader);
	}

	if (printPlayers)
	{
		renderPlayerList();
	}
}

bool GameScene::processKeyEvent(int key, int action)
{
	if (action == GLFW_RELEASE)
	{
		// Stop camera movement
		cameraState = CameraMovementState::NONE;
	}
	else if (action == GLFW_PRESS)
	{
		// Move camera left
		if (key == GLFW_KEY_A)
		{
			cameraState = CameraMovementState::LEFT;
		}
		// Move camera right
		else if (key == GLFW_KEY_D)
		{
			cameraState = CameraMovementState::RIGHT;
		}
		// Move camera up
		else if (key == GLFW_KEY_W)
		{
			cameraState = CameraMovementState::UP;
		}
		// Move camera down
		else if (key == GLFW_KEY_S)
		{
			cameraState = CameraMovementState::DOWN;
		}
		// Move camera backward
		else if (key == GLFW_KEY_Q)
		{
			cameraState = CameraMovementState::BACKWARD;
		}
		// Move camera forward
		else if (key == GLFW_KEY_E)
		{
			cameraState = CameraMovementState::FORWARD;
		}
		else if (key == GLFW_KEY_P)
		{
			printPlayers = !printPlayers;
		}
	}

	return false;
}

void GameScene::processMouseScroll(double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void GameScene::processMouseMovement(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
