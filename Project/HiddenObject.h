#pragma once

#include "Model3D.h"
#include "tinyxml2.h"

using namespace tinyxml2;

// A class that represents object hidden and a game word that player must search for; it renders itself and holds file path to its icon
class HiddenObject
{
public:
	HiddenObject(const std::string& modelFileName, const std::string& iconFileName);
	void render(const Shader& shader) const;
	const std::string& getIconFileName() const { return iconFileName; }
	bool isFound() const { return found; }
	void setFound(bool isFound) { found = isFound; }
private:
	Model* objectModel;
	std::string iconFileName;
	bool found;
};