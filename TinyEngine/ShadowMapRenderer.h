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
	virtual void RenderShadowMap() = 0;
	virtual Texture* GetShadowMap() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;
};

class DirectionalShadowMapRenderer : public IShadowMapRenderer
{
public:
	DirectionalShadowMapRenderer(DirectionalLight* light);

	void RenderShadowMap() override;
	Texture* GetShadowMap() override;
	glm::mat4 GetProjectionMatrix() override;
private:
	const static int DEFAULT_SHADOW_MAP_WIDTH;
	const static int DEFAULT_SHADOW_MAP_HEIGHT;

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
#endif // !SHADOW_MAP_RENDERER_H