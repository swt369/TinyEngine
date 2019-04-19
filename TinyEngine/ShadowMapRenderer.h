#ifndef SHADOW_MAP_RENDERER_H
#define SHADOW_MAP_RENDERER_H

#include <glad/glad.h>

#include "Camera.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Shader.h"
#include "Texture.h"

class IShadowMapRenderer
{
public:
	const static int DEFAULT_SHADOW_MAP_WIDTH;
	const static int DEFAULT_SHADOW_MAP_HEIGHT;

	virtual void RenderShadowMap() = 0;
	virtual ITexture* GetShadowMap() = 0;
};

class DirectionalShadowMapRenderer : public IShadowMapRenderer
{
public:
	DirectionalShadowMapRenderer(DirectionalLight* light);

	void RenderShadowMap() override;
	ITexture* GetShadowMap() override;

	glm::mat4 GetProjectionMatrix();
private:
	const static float DEFAULT_ORTHO_LEFT;
	const static float DEFAULT_ORTHO_RIGHT;
	const static float DEFAULT_ORTHO_BOTTOM;
	const static float DEFAULT_ORTHO_UP;
	const static float DEFAULT_NEAR_PLANE;
	const static float DEFAULT_FAR_PLANE;
	const static float DEFAULT_DEPTH;

	DirectionalLight* light;

	Object* shadowMapCameraObj;
	Camera* shadowMapCamera;
	IFrameBuffer* shadowMapFrameBuffer;
	Shader* shadowMapShader;
};

class OmniDirectionalShadowMapRenderer : public IShadowMapRenderer
{
public:
	const static float DEFAULT_NEAR_PLANE;
	const static float DEFAULT_DEPTH;

	const static string VIEW_MATRICES_NAME;
	const static string LIGHT_POS_NAME;
	const static string FAR_PLANE_NAME;
	const static vector<glm::vec3> viewRotationVectors;
	const static vector<glm::vec3> ups;

	OmniDirectionalShadowMapRenderer(PointLight* light);

	void RenderShadowMap() override;
	ITexture* GetShadowMap() override;
private:
	PointLight* light;

	Object* shadowMapCameraObj;
	Camera* shadowMapCamera;
	IFrameBuffer* shadowMapFrameBuffer;
	Shader* shadowMapShader;
};
#endif // !SHADOW_MAP_RENDERER_H