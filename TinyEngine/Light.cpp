#include "Light.h"

const string Light::AMBIENT_NAME = "ambient";
const string Light::DIFFUSE_NAME = "diffuse";
const string Light::SPECULAR_NAME = "specular";

const glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.1f, 0.1f, 0.1f);
const glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec3 DEFAULT_SPECULAR = glm::vec3(1.0f, 1.0f, 1.0f);

const string DirectionalLight::DIRECTIONAL_LIGHT_NAME = "directionalLights";
const string DirectionalLight::DIRECTIONAL_LIGHT_COUNT_NAME = "directionalLightCount";
const string DirectionalLight::DIRECTION_NAME = "direction";
const glm::vec3 DirectionalLight::DEFAULT_DIRECTION = glm::vec3(1.0f, cos(glm::radians(135.0f)), 1.0f);

const string PointLight::POINT_LIGHT_NAME = "pointLights";
const string PointLight::POINT_LIGHT_COUNT_NAME = "pointLightCount";
const string PointLight::POSITION_NAME = "position";
const string PointLight::CONSTANT_NAME = "constant";
const string PointLight::LINEAR_NAME = "linear";
const string PointLight::QUADRATIC_NAME = "quadratic";
const glm::vec3 PointLight::DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const float PointLight::DEFAULT_CONSTANT = 1.0f;
const float PointLight::DEFAULT_LINEAR = 0.09f;
const float PointLight::DEFAULT_QUADRATIC = 0.032f;

const string SpotLight::SPOT_LIGHT_NAME = "spotLights";
const string SpotLight::SPOT_LIGHT_COUNT_NAME = "spotLightCount";
const string SpotLight::POSITION_NAME = "position";
const string SpotLight::DIRECTION_NAME = "direction";
const string SpotLight::INNER_CUTOFF_NAME = "innerCutoff";
const string SpotLight::OUTER_CUTOFF_NAME = "outerCutoff";
const glm::vec3 SpotLight::DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 SpotLight::DEFAULT_DIREECTION = glm::vec3(-0.2f, -1.0f, -0.3f);
const float SpotLight::DEFAULT_INNER_CUTOFF_ANGLE = 12.5f;
const float SpotLight::DEFAULT_OUTER_CUTOFF_ANGLE = 17.5f;

Light::Light()
{
	this->ambient = DEFAULT_AMBIENT;
	this->diffuse = DEFAULT_DIFFUSE;
	this->specular = DEFAULT_SPECULAR;
}

void Light::use(Shader * shader, int id)
{
	shader->setVec3(getKeyInArray(AMBIENT_NAME, id), ambient);
	shader->setVec3(getKeyInArray(DIFFUSE_NAME, id), diffuse);
	shader->setVec3(getKeyInArray(SPECULAR_NAME, id), specular);
}

string Light::getKeyInArray(string key, int id)
{
	return StringUtil::getKeyInArray(getTypeName(), key, id);
}

DirectionalLight::DirectionalLight(glm::vec3 direction)
{
	this->direction = direction;
}

void DirectionalLight::use(Shader* shader, int id)
{
	Light::use(shader, id);
	shader->setVec3(getKeyInArray(DIRECTION_NAME, id), direction);
}

string DirectionalLight::getTypeName()
{
	return DIRECTIONAL_LIGHT_NAME;
}

string DirectionalLight::getTypeCountName()
{
	return DIRECTIONAL_LIGHT_COUNT_NAME;
}

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic)
{
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void PointLight::use(Shader* shader, int id)
{
	Light::use(shader, id);
	shader->setVec3(getKeyInArray(POSITION_NAME, id), position);
	shader->setFloat(getKeyInArray(CONSTANT_NAME, id), constant);
	shader->setFloat(getKeyInArray(LINEAR_NAME, id), linear);
	shader->setFloat(getKeyInArray(QUADRATIC_NAME, id), quadratic);
}

string PointLight::getTypeName()
{
	return POINT_LIGHT_NAME;
}

string PointLight::getTypeCountName()
{
	return POINT_LIGHT_COUNT_NAME;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float innerCutoffAngle, float outerCutoffAngle)
{
	this->position = position;
	this->direction = direction;
	this->innerCutoffAngle = innerCutoffAngle;
	this->outerCutoffAngle = outerCutoffAngle;
}

void SpotLight::use(Shader* shader, int id)
{
	Light::use(shader, id);
	shader->setVec3(getKeyInArray(POSITION_NAME, id), position);
	shader->setVec3(getKeyInArray(DIRECTION_NAME, id), direction);
	shader->setFloat(getKeyInArray(INNER_CUTOFF_NAME, id), cos(glm::radians(innerCutoffAngle)));
	shader->setFloat(getKeyInArray(OUTER_CUTOFF_NAME, id), cos(glm::radians(outerCutoffAngle)));
}

string SpotLight::getTypeName()
{
	return SPOT_LIGHT_NAME;
}

string SpotLight::getTypeCountName()
{
	return SPOT_LIGHT_COUNT_NAME;
}
