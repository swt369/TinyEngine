#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <glm/glm.hpp>

#include "Cubemap.h"
#include "Shader.h"
#include "StringUtil.h"
#include "Texture.h"

using namespace std;

class Material
{
public:
	Shader* shader;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Material(Shader* shader = nullptr, glm::vec3 ambient = DEFAULT_AMBIENT, glm::vec3 diffuse = DEFAULT_DIFFUSE, glm::vec3 specular = DEFAULT_SPECULAR, float shininess = DEFAULT_SHININESS);

	void setShader(Shader* shader);
	void setInt(string key, int value);
	void setFloat(string key, float value);
	void setBool(string key, bool value);
	void setVec3(string key, glm::vec3 value);
	void setMat3(string key, glm::mat3 value);
	void setMat4(string key, glm::mat4 value);
	void setTexture(string key, Texture* value);
	void setDiffuseTexture(int index, Texture* value);
	void setSpecularTexture(int index, Texture* value);
	void setCubemap(string key, Cubemap* value);

	void Use();
private:
	static const string AMBIENT_NAME;
	static const string DIFFUSE_NAME;
	static const string SPECULAR_NAME;
	static const string SHININESS_NAME;

	static const string DIFFUSE_TEXTURE_NAME_FMT;
	static const string SPECULAR_TEXTURE_NAME_FMT;

	const static glm::vec3 DEFAULT_AMBIENT;
	const static glm::vec3 DEFAULT_DIFFUSE;
	const static glm::vec3 DEFAULT_SPECULAR;
	const static float DEFAULT_SHININESS;

	map<string, int> intMap;
	map<string, float> floatMap;
	map<string, bool> boolMap;
	map<string, glm::vec3> vec3Map;
	map<string, glm::mat3> mat3Map;
	map<string, glm::mat4> mat4Map;
	map<string, Texture*> textureMap;
	map<string, Cubemap*> cubemapMap;

	string getKey(string key);
};

#endif // !MATERIAL_H
