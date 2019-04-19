#include "Material.h"

const string Material::AMBIENT_NAME = "ambient";
const string Material::DIFFUSE_NAME = "diffuse";
const string Material::SPECULAR_NAME = "specular";
const string Material::SHININESS_NAME = "shininess";

const string Material::DIFFUSE_TEXTURE_NAME_FMT = "texture_diffuse_%d";
const string Material::SPECULAR_TEXTURE_NAME_FMT = "texture_specular_%d";

const glm::vec3 Material::DEFAULT_AMBIENT = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 Material::DEFAULT_DIFFUSE = glm::vec3(1.0f, 1.0f, 1.0f);
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

void Material::setInt(string key, int value, bool isPermanent)
{
	if (isPermanent)
	{
		intMap[key] = value;
	}
	else
	{
		disposableIntMap[key] = value;
	}
}

void Material::setFloat(string key, float value, bool isPermanent)
{
	if (isPermanent)
	{
		floatMap[key] = value;
	}
	else
	{
		disposableFloatMap[key] = value;
	}
}

void Material::setBool(string key, bool value, bool isPermanent)
{
	if (isPermanent)
	{
		boolMap[key] = value;
	}
	else
	{
		disposableBoolMap[key] = value;
	}
}

void Material::setVec3(string key, glm::vec3 value, bool isPermanent)
{
	if (isPermanent)
	{
		vec3Map[key] = value;
	}
	else
	{
		disposableVec3Map[key] = value;
	}
}

void Material::setMat3(string key, glm::mat3 value, bool isPermanent)
{
	if (isPermanent)
	{
		mat3Map[key] = value;
	}
	else
	{
		disposableMat3Map[key] = value;
	}
}

void Material::setMat4(string key, glm::mat4 value, bool isPermanent)
{
	if (isPermanent)
	{
		mat4Map[key] = value;
	}
	else
	{
		disposableMat4Map[key] = value;
	}
}

void Material::setTexture(string key, Texture* value, bool isPermanent)
{
	if (isPermanent)
	{
		textureMap[key] = value;
	}
	else
	{
		disposableTextureMap[key] = value;
	}
}

void Material::setDiffuseTexture(int index, Texture * value, bool isPermanent)
{
	setTexture(StringUtil::format(DIFFUSE_TEXTURE_NAME_FMT, index), value, isPermanent);
}

void Material::setSpecularTexture(int index, Texture * value, bool isPermanent)
{
	setTexture(StringUtil::format(SPECULAR_TEXTURE_NAME_FMT, index), value, isPermanent);
}

void Material::setCubemap(string key, Cubemap * value, bool isPermanent)
{
	if (isPermanent)
	{
		cubemapMap[key] = value;
	}
	else
	{
		disposableCubemapMap[key] = value;
	}
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
	for (map<string, int>::const_iterator it = disposableIntMap.begin(); it != disposableIntMap.end(); it++)
	{
		shaderToBeUsed->setInt(it->first, it->second);
	}

	for (map<string, float>::const_iterator it = floatMap.begin(); it != floatMap.end(); it++)
	{
		shaderToBeUsed->setFloat(it->first, it->second);
	}
	for (map<string, float>::const_iterator it = disposableFloatMap.begin(); it != disposableFloatMap.end(); it++)
	{
		shaderToBeUsed->setFloat(it->first, it->second);
	}

	for (map<string, bool>::const_iterator it = boolMap.begin(); it != boolMap.end(); it++)
	{
		shaderToBeUsed->setBool(it->first, it->second);
	}
	for (map<string, bool>::const_iterator it = disposableBoolMap.begin(); it != disposableBoolMap.end(); it++)
	{
		shaderToBeUsed->setBool(it->first, it->second);
	}

	for (map<string, glm::vec3>::const_iterator it = vec3Map.begin(); it != vec3Map.end(); it++)
	{
		shaderToBeUsed->setVec3(it->first, it->second);
	}
	for (map<string, glm::vec3>::const_iterator it = disposableVec3Map.begin(); it != disposableVec3Map.end(); it++)
	{
		shaderToBeUsed->setVec3(it->first, it->second);
	}

	for (map<string, glm::mat3>::const_iterator it = mat3Map.begin(); it != mat3Map.end(); it++)
	{
		shaderToBeUsed->setMat3(it->first, it->second);
	}
	for (map<string, glm::mat3>::const_iterator it = disposableMat3Map.begin(); it != disposableMat3Map.end(); it++)
	{
		shaderToBeUsed->setMat3(it->first, it->second);
	}

	for (map<string, glm::mat4>::const_iterator it = mat4Map.begin(); it != mat4Map.end(); it++)
	{
		shaderToBeUsed->setMat4(it->first, it->second);
	}
	for (map<string, glm::mat4>::const_iterator it = disposableMat4Map.begin(); it != disposableMat4Map.end(); it++)
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
	for (map<string, Texture*>::const_iterator it = disposableTextureMap.begin(); it != disposableTextureMap.end(); it++)
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
	for (map<string, Cubemap*>::const_iterator it = disposableCubemapMap.begin(); it != disposableCubemapMap.end(); it++)
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

	disposableIntMap.clear();
	disposableFloatMap.clear();
	disposableBoolMap.clear();
	disposableVec3Map.clear();
	disposableMat3Map.clear();
	disposableMat4Map.clear();
	disposableTextureMap.clear();
	disposableCubemapMap.clear();
}

string Material::getKey(string key)
{
	char c[64];
	sprintf_s(c, "material.%s", key.c_str());
	return string(c);
}
