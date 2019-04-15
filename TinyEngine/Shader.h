#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdarg.h>

class Shader 
{
public:	
	static const std::string LOCAL_TO_CLIP_MATRIX_NAME;
	static const std::string PROJECTION_MATRIX_NAME;
	static const std::string VIEW_MATRIX_NAME;
	static const std::string MODEL_MATRIX_NAME;
	static const std::string NORMAL_MATRIX_NAME;
	static const std::string VIEW_POS;

	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec(const std::string &name, int n, ...) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
	void setMat3(const std::string &name, const glm::mat3 &value) const;
	void setMat4(const std::string &name, const glm::mat4 &value) const;

private:
	void checkCompileResult(unsigned int shader, std::string type);
};

#endif // !SHADER_H