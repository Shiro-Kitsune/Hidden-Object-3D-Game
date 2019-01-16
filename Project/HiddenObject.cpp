#include "HiddenObject.h"

HiddenObject::HiddenObject(const std::string & modelName, const std::string& iconName)
{
	objectModel = new Model3D(modelName);
	iconFileName = iconName;
}

void HiddenObject::render(const Shader & shader) const
{
	objectModel->render(shader);
}
