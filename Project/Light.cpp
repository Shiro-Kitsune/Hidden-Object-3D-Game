#include "Light.h"

void DirectionalLight::load(XMLElement * element)
{
	XMLElement* directionElement = element->FirstChildElement("Direction");
	if (directionElement != nullptr)
	{
		directionElement->FirstChildElement("X")->QueryFloatText(&direction.x);
		directionElement->FirstChildElement("Y")->QueryFloatText(&direction.y);
		directionElement->FirstChildElement("Z")->QueryFloatText(&direction.z);
	}

	XMLElement* ambientElement = element->FirstChildElement("Ambient");
	if (ambientElement != nullptr)
	{
		ambientElement->FirstChildElement("R")->QueryFloatText(&ambient.r);
		ambientElement->FirstChildElement("G")->QueryFloatText(&ambient.g);
		ambientElement->FirstChildElement("B")->QueryFloatText(&ambient.b);
	}

	XMLElement* diffuseElement = element->FirstChildElement("Diffuse");
	if (diffuseElement != nullptr)
	{
		diffuseElement->FirstChildElement("R")->QueryFloatText(&diffuse.r);
		diffuseElement->FirstChildElement("G")->QueryFloatText(&diffuse.g);
		diffuseElement->FirstChildElement("B")->QueryFloatText(&diffuse.b);
	}

	XMLElement* specularElement = element->FirstChildElement("Specular");
	if (specularElement != nullptr)
	{
		specularElement->FirstChildElement("R")->QueryFloatText(&specular.r);
		specularElement->FirstChildElement("G")->QueryFloatText(&specular.g);
		specularElement->FirstChildElement("B")->QueryFloatText(&specular.b);
	}
}

void SpotLight::load(XMLElement * element)
{
	XMLElement* directionElement = element->FirstChildElement("Direction");
	if (directionElement != nullptr)
	{
		directionElement->FirstChildElement("X")->QueryFloatText(&direction.x);
		directionElement->FirstChildElement("Y")->QueryFloatText(&direction.y);
		directionElement->FirstChildElement("Z")->QueryFloatText(&direction.z);
	}

	XMLElement* positionElement = element->FirstChildElement("Position");
	if (positionElement != nullptr)
	{
		positionElement->FirstChildElement("X")->QueryFloatText(&position.x);
		positionElement->FirstChildElement("Y")->QueryFloatText(&position.y);
		positionElement->FirstChildElement("Z")->QueryFloatText(&position.z);
	}

	XMLElement* ambientElement = element->FirstChildElement("Ambient");
	if (ambientElement != nullptr)
	{
		ambientElement->FirstChildElement("R")->QueryFloatText(&ambient.r);
		ambientElement->FirstChildElement("G")->QueryFloatText(&ambient.g);
		ambientElement->FirstChildElement("B")->QueryFloatText(&ambient.b);
	}

	XMLElement* diffuseElement = element->FirstChildElement("Diffuse");
	if (diffuseElement != nullptr)
	{
		diffuseElement->FirstChildElement("R")->QueryFloatText(&diffuse.r);
		diffuseElement->FirstChildElement("G")->QueryFloatText(&diffuse.g);
		diffuseElement->FirstChildElement("B")->QueryFloatText(&diffuse.b);
	}

	XMLElement* specularElement = element->FirstChildElement("Specular");
	if (specularElement != nullptr)
	{
		specularElement->FirstChildElement("R")->QueryFloatText(&specular.r);
		specularElement->FirstChildElement("G")->QueryFloatText(&specular.g);
		specularElement->FirstChildElement("B")->QueryFloatText(&specular.b);
	}

	XMLElement* constantElement = element->FirstChildElement("Constant");
	if (constantElement != nullptr)
	{
		constantElement->QueryFloatText(&constant);
	}

	XMLElement* linearElement = element->FirstChildElement("Linear");
	if (linearElement != nullptr)
	{
		linearElement->QueryFloatText(&linear);
	}

	XMLElement* quadraticElement = element->FirstChildElement("Quadratic");
	if (quadraticElement != nullptr)
	{
		quadraticElement->QueryFloatText(&quadratic);
	}

	XMLElement* cutOffElement = element->FirstChildElement("CutOff");
	if (cutOffElement != nullptr)
	{
		cutOffElement->QueryFloatText(&cutOff);
		cutOff = glm::radians(cutOff);
	}

	XMLElement* outerCutOffElement = element->FirstChildElement("OuterCutOff");
	if (outerCutOffElement != nullptr)
	{
		outerCutOffElement->QueryFloatText(&outerCutOff);
		outerCutOff = glm::radians(outerCutOff);
	}
}

void PointLight::load(XMLElement * element)
{
	XMLElement* positionElement = element->FirstChildElement("Position");
	if (positionElement != nullptr)
	{
		positionElement->FirstChildElement("X")->QueryFloatText(&position.x);
		positionElement->FirstChildElement("Y")->QueryFloatText(&position.y);
		positionElement->FirstChildElement("Z")->QueryFloatText(&position.z);
	}

	XMLElement* ambientElement = element->FirstChildElement("Ambient");
	if (ambientElement != nullptr)
	{
		ambientElement->FirstChildElement("R")->QueryFloatText(&ambient.r);
		ambientElement->FirstChildElement("G")->QueryFloatText(&ambient.g);
		ambientElement->FirstChildElement("B")->QueryFloatText(&ambient.b);
	}

	XMLElement* diffuseElement = element->FirstChildElement("Diffuse");
	if (diffuseElement != nullptr)
	{
		diffuseElement->FirstChildElement("R")->QueryFloatText(&diffuse.r);
		diffuseElement->FirstChildElement("G")->QueryFloatText(&diffuse.g);
		diffuseElement->FirstChildElement("B")->QueryFloatText(&diffuse.b);
	}

	XMLElement* specularElement = element->FirstChildElement("Specular");
	if (specularElement != nullptr)
	{
		specularElement->FirstChildElement("R")->QueryFloatText(&specular.r);
		specularElement->FirstChildElement("G")->QueryFloatText(&specular.g);
		specularElement->FirstChildElement("B")->QueryFloatText(&specular.b);
	}

	XMLElement* constantElement = element->FirstChildElement("Constant");
	if (constantElement != nullptr)
	{
		constantElement->QueryFloatText(&constant);
	}

	XMLElement* linearElement = element->FirstChildElement("Linear");
	if (linearElement != nullptr)
	{
		linearElement->QueryFloatText(&linear);
	}

	XMLElement* quadraticElement = element->FirstChildElement("Quadratic");
	if (quadraticElement != nullptr)
	{
		quadraticElement->QueryFloatText(&quadratic);
	}
}
