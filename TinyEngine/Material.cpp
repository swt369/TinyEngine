#include "Material.h"

const string Material::AMBIENT_NAME = "ambient";
const string Material::DIFFUSE_NAME = "diffuse";
const string Material::SPECULAR_NAME = "specular";
const string Material::SHININESS_NAME = "shininess";

const string Material::DIFFUSE_TEXTURE_NAME_FMT = "texture_diffuse_%d";
const string Material::SPECULAR_TEXTURE_NAME_FMT = "texture_specular_%d";

const glm::vec3 Material::DEFAULT_AMBIENT = glm::vec3(1.0f, 0.5f, 0.31f);
const glm::vec3 Material::DEFAULT_DIFFUSE = glm::vec3(1.0f, 0.5f, 0.31f);
const glm::vec3 Material::DEFAULT_SPECULAR = glm::vec3(0.5f, 0.5f, 0.5f);
const float Material::DEFAULT_SHININESS = 32.0f;

Material::Material(Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	this->shader = shader;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

void Material::setShader(Shader * shader)
{
	this->shader = shader;
}

void Material::setInt(string key, int value)
{
	intMap[key] = value;
}

void Material::setFloat(string key, float value)
{
	floatMap[key] = value;
}

void Material::setBool(string key, bool value)
{
	boolMap[key] = value;
}

void Material::setVec3(string key, glm::vec3 value)
{
	vec3Map[key] = value;
}

void Material::setMat3(string key, glm::mat3 value)
{
	mat3Map[key] = value;
}

void Material::setMat4(string key, glm::mat4 value)
{
	mat4Map[key] = value;
}

void Material::setTexture(string key, Texture* value)
{
	textureMap[key] = value;
}

void Material::setDiffuseTexture(int index, Texture * value)
{
	setTexture(StringUtil::format(DIFFUSE_TEXTURE_NAME_FMT, index), value);
}

void Material::setSpecularTexture(int index, Texture * value)
{
	setTexture(StringUtil::format(SPECULAR_TEXTURE_NAME_FMT, index), value);
}

void Material::setCubemap(string key, Cubemap * value)
{
	cubemapMap[key] = value;
}

void Material::Use(Shader* globalShader)
{
	Shader* shaderToBeUsed = globalShader != nullptr ? globalShader : shader;
	if (shaderToBeUsed == nullptr)
	{
		return;
	}

	shaderToBeUsed->use();
	for (map<string, int>::const_iterator it = intMap.begin(); it != intMap.end(); it++)
	{
		shaderToBeUsed->setInt(it->first, it->second);
	}

	for (map<string, float>::const_iterator it = floatMap.begin(); it != floatMap.end(); it++)
	{
		shaderToBeUsed->setFloat(it->first, it->second);
	}

	for (map<string, bool>::const_iterator it = boolMap.begin(); it != boolMap.end(); it++)
	{
		shaderToBeUsed->setBool(it->first, it->second);
	}

	for (map<string, glm::vec3>::const_iterator it = vec3Map.begin(); it != vec3Map.end(); it++)
	{
		shaderToBeUsed->setVec3(it->first, it->second);
	}

	for (map<string, glm::mat3>::const_iterator it = mat3Map.begin(); it != mat3Map.end(); it++)
	{
		shaderToBeUsed->setMat3(it->first, it->second);
	}

	for (map<string, glm::mat4>::const_iterator it = mat4Map.begin(); it != mat4Map.end(); it++)
	{
		shaderToBeUsed->setMat4(it->first, it->second);
	}

	int curTextureIndex = 0;
	for (map<string, Texture*>::const_iterator it = textureMap.begin(); it != textureMap.end(); it++)
	{
		string name = it->first;
		Texture* texture = it->second;
		texture->Bind(curTextureIndex);
		shaderToBeUsed->setInt(name, curTextureIndex++);
	}
	for (map<string, Cubemap*>::const_iterator it = cubemapMap.begin(); it != cubemapMap.end(); it++)
	{
		string name = it->first;
		Cubemap* cubemap = it->second;
		cubemap->Bind(curTextureIndex);
		shaderToBeUsed->setInt(name, curTextureIndex++);
	}

	shaderToBeUsed->setVec3(getKey(AMBIENT_NAME), ambient);
	shaderToBeUsed->setVec3(getKey(DIFFUSE_NAME), diffuse);
	shaderToBeUsed->setVec3(getKey(SPECULAR_NAME), specular);
	shaderToBeUsed->setFloat(getKey(SHININESS_NAME), shininess);
}

string Material::getKey(string key)
{
	char c[64];
	sprintf_s(c, "material.%s", key.c_str());
	return string(c);
}
