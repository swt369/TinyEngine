#ifndef LIGHT_H
#define LIGHT_H

#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include "Shader.h"

class Light : public Component
{
public:
	glm::vec3 ambient = DEFAULT_AMBIENT;
	glm::vec3 diffuse = DEFAULT_DIFFUSE;
	glm::vec3 specular = DEFAULT_SPECULAR;

	Light(Object* object);

	virtual void use(Shader* shader, int id);
	virtual string getTypeName() = 0;
	virtual string getTypeCountName() = 0;
protected:
	string getKeyInArray(string key, int id);
private:
	static const string AMBIENT_NAME;
	static const string DIFFUSE_NAME;
	static const string SPECULAR_NAME;
	static const string LIGHT_SPACE_MATRIX_NAME;

	static const glm::vec3 DEFAULT_AMBIENT;
	static const glm::vec3 DEFAULT_DIFFUSE;
	static const glm::vec3 DEFAULT_SPECULAR;
};

class DirectionalLight : public Light 
{
public:
	static const string DIRECTIONAL_LIGHT_NAME;

	DirectionalLight(Object* object);

	string GetComponentName() override;
	void use(Shader* shader, int id) override;
	string getTypeName() override;
	string getTypeCountName() override;
private:
	static const string DIRECTIONAL_LIGHT_ARRAY_NAME;
	static const string DIRECTIONAL_LIGHT_COUNT_NAME;
	static const string DIRECTION_NAME;
};

class PointLight : public Light
{
public:
	static const string POINT_LIGHT_NAME;

	float constant = DEFAULT_CONSTANT;
	float linear = DEFAULT_LINEAR;
	float quadratic = DEFAULT_QUADRATIC;

	PointLight(Object* object);

	string GetComponentName() override;
	void use(Shader* shader, int id) override;
	string getTypeName() override;
	string getTypeCountName() override;
private:
	static const string POINT_LIGHT_ARRAY_NAME;
	static const string POINT_LIGHT_COUNT_NAME;

	static const string POSITION_NAME;
	static const string CONSTANT_NAME;
	static const string LINEAR_NAME;
	static const string QUADRATIC_NAME;
	
	static const float DEFAULT_CONSTANT;
	static const float DEFAULT_LINEAR;
	static const float DEFAULT_QUADRATIC;
};

class SpotLight : public Light 
{
public:
	static const string SPOT_LIGHT_NAME;

	float innerCutoffAngle = DEFAULT_INNER_CUTOFF_ANGLE;
	float outerCutoffAngle = DEFAULT_OUTER_CUTOFF_ANGLE;

	SpotLight(Object* object);

	string GetComponentName() override;
	void use(Shader* shader, int id) override;
	string getTypeName() override;
	string getTypeCountName() override;
private:
	static const string SPOT_LIGHT_ARRAY_NAME;
	static const string SPOT_LIGHT_COUNT_NAME;

	static const string POSITION_NAME;
	static const string DIRECTION_NAME;
	static const string INNER_CUTOFF_NAME;
	static const string OUTER_CUTOFF_NAME;

	static const float DEFAULT_INNER_CUTOFF_ANGLE;
	static const float DEFAULT_OUTER_CUTOFF_ANGLE;
};

#endif // !LIGHT_H
