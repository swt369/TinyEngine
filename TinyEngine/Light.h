#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
using namespace std;

#include "Shader.h"
#include "StringUtil.h"

class Light
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light();

	virtual void use(Shader* shader, int id);
	virtual string getTypeName() = 0;
	virtual string getTypeCountName() = 0;
protected:
	string getKeyInArray(string key, int id);
private:
	static const string AMBIENT_NAME;
	static const string DIFFUSE_NAME;
	static const string SPECULAR_NAME;
};

class DirectionalLight : public Light 
{
public:
	glm::vec3 direction;

	DirectionalLight(glm::vec3 direction = DEFAULT_DIRECTION);

	virtual void use(Shader* shader, int id);
	virtual string getTypeName();
	virtual string getTypeCountName();
private:
	static const string DIRECTIONAL_LIGHT_NAME;
	static const string DIRECTIONAL_LIGHT_COUNT_NAME;

	static const string DIRECTION_NAME;

	static const glm::vec3 DEFAULT_DIRECTION;
};

class PointLight : public Light
{
public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3 position = DEFAULT_POSITION, float constant = DEFAULT_CONSTANT, float linear = DEFAULT_LINEAR, float quadratic = DEFAULT_QUADRATIC);

	virtual void use(Shader* shader, int id);
	virtual string getTypeName();
	virtual string getTypeCountName();
private:
	static const string POINT_LIGHT_NAME;
	static const string POINT_LIGHT_COUNT_NAME;

	static const string POSITION_NAME;
	static const string CONSTANT_NAME;
	static const string LINEAR_NAME;
	static const string QUADRATIC_NAME;
	
	static const glm::vec3 DEFAULT_POSITION;
	static const float DEFAULT_CONSTANT;
	static const float DEFAULT_LINEAR;
	static const float DEFAULT_QUADRATIC;
};

class SpotLight : public Light 
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutoffAngle;
	float outerCutoffAngle;

	SpotLight(glm::vec3 position = DEFAULT_POSITION, glm::vec3 direction = DEFAULT_DIREECTION,
		float innerCutoffAngle = DEFAULT_INNER_CUTOFF_ANGLE, float outerCutoffAngle = DEFAULT_OUTER_CUTOFF_ANGLE);

	virtual void use(Shader* shader, int id);
	virtual string getTypeName();
	virtual string getTypeCountName();
private:
	static const string SPOT_LIGHT_NAME;
	static const string SPOT_LIGHT_COUNT_NAME;

	static const string POSITION_NAME;
	static const string DIRECTION_NAME;
	static const string INNER_CUTOFF_NAME;
	static const string OUTER_CUTOFF_NAME;

	static const glm::vec3 DEFAULT_POSITION;
	static const glm::vec3 DEFAULT_DIREECTION;
	static const float DEFAULT_INNER_CUTOFF_ANGLE;
	static const float DEFAULT_OUTER_CUTOFF_ANGLE;
};

#endif // !LIGHT_H
