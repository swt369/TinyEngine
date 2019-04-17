#include "Light.h"
#include "StringUtil.h"
#include "Transform.h"

const string Light::AMBIENT_NAME = "ambient";
const string Light::DIFFUSE_NAME = "diffuse";
const string Light::SPECULAR_NAME = "specular";

const glm::vec3 Light::DEFAULT_AMBIENT = glm::vec3(0.1f, 0.1f, 0.1f);
const glm::vec3 Light::DEFAULT_DIFFUSE = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec3 Light::DEFAULT_SPECULAR = glm::vec3(1.0f, 1.0f, 1.0f);

const string DirectionalLight::DIRECTIONAL_LIGHT_NAME = "DirectionalLight";
const string DirectionalLight::DIRECTIONAL_LIGHT_ARRAY_NAME = "directionalLights";
const string DirectionalLight::DIRECTIONAL_LIGHT_COUNT_NAME = "directionalLightCount";
const string DirectionalLight::DIRECTION_NAME = "direction";

const string PointLight::POINT_LIGHT_NAME = "PointLight";
const string PointLight::POINT_LIGHT_ARRAY_NAME = "pointLights";
const string PointLight::POINT_LIGHT_COUNT_NAME = "pointLightCount";
const string PointLight::POSITION_NAME = "position";
const string PointLight::CONSTANT_NAME = "constant";
const string PointLight::LINEAR_NAME = "linear";
const string PointLight::QUADRATIC_NAME = "quadratic";
const float PointLight::DEFAULT_CONSTANT = 1.0f;
const float PointLight::DEFAULT_LINEAR = 0.09f;
const float PointLight::DEFAULT_QUADRATIC = 0.032f;

const string SpotLight::SPOT_LIGHT_NAME = "SpotLight";
const string SpotLight::SPOT_LIGHT_ARRAY_NAME = "spotLights";
const string SpotLight::SPOT_LIGHT_COUNT_NAME = "spotLightCount";
const string SpotLight::POSITION_NAME = "position";
const string SpotLight::DIRECTION_NAME = "direction";
const string SpotLight::INNER_CUTOFF_NAME = "innerCutoff";
const string SpotLight::OUTER_CUTOFF_NAME = "outerCutoff";
const float SpotLight::DEFAULT_INNER_CUTOFF_ANGLE = 12.5f;
const float SpotLight::DEFAULT_OUTER_CUTOFF_ANGLE = 17.5f;

Light::Light(Object* object) : Component(object)
{

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

DirectionalLight::DirectionalLight(Object* object) : Light(object)
{

}

string DirectionalLight::GetComponentName()
{
	return DIRECTIONAL_LIGHT_NAME;
}

void DirectionalLight::use(Shader* shader, int id)
{
	Light::use(shader, id);
	shader->setVec3(getKeyInArray(DIRECTION_NAME, id), GetTransform()->getForward());
}

string DirectionalLight::getTypeName()
{
	return DIRECTIONAL_LIGHT_ARRAY_NAME;
}

string DirectionalLight::getTypeCountName()
{
	return DIRECTIONAL_LIGHT_COUNT_NAME;
}

PointLight::PointLight(Object* object) : Light(object)
{

}

string PointLight::GetComponentName()
{
	return POINT_LIGHT_NAME;
}

void PointLight::use(Shader* shader, int id)
{
	Light::use(shader, id);
	shader->setVec3(getKeyInArray(POSITION_NAME, id), GetTransform()->position);
	shader->setFloat(getKeyInArray(CONSTANT_NAME, id), constant);
	shader->setFloat(getKeyInArray(LINEAR_NAME, id), linear);
	shader->setFloat(getKeyInArray(QUADRATIC_NAME, id), quadratic);
}

string PointLight::getTypeName()
{
	return POINT_LIGHT_ARRAY_NAME;
}

string PointLight::getTypeCountName()
{
	return POINT_LIGHT_COUNT_NAME;
}

SpotLight::SpotLight(Object* object) : Light(object)
{

}

string SpotLight::GetComponentName()
{
	return SPOT_LIGHT_NAME;
}

void SpotLight::use(Shader* shader, int id)
{
	Light::use(shader, id);
	shader->setVec3(getKeyInArray(POSITION_NAME, id), GetTransform()->position);
	shader->setVec3(getKeyInArray(DIRECTION_NAME, id), GetTransform()->getForward());
	shader->setFloat(getKeyInArray(INNER_CUTOFF_NAME, id), cos(glm::radians(innerCutoffAngle)));
	shader->setFloat(getKeyInArray(OUTER_CUTOFF_NAME, id), cos(glm::radians(outerCutoffAngle)));
}

string SpotLight::getTypeName()
{
	return SPOT_LIGHT_ARRAY_NAME;
}

string SpotLight::getTypeCountName()
{
	return SPOT_LIGHT_COUNT_NAME;
}
