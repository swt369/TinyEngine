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

class ShadowMapRenderer
{
public:
	static ShadowMapRenderer& getInstance();

	void RenderShadowMap();
	Texture* GetShadowMap();

	Object* shadowMapCameraObj;
	Camera* shadowMapCamera;
private:
	const static float DEFAULT_ORTHO_LEFT;
	const static float DEFAULT_ORTHO_RIGHT;
	const static float DEFAULT_ORTHO_BOTTOM;
	const static float DEFAULT_ORTHO_UP;
	const static float DEFAULT_NEAR_PLANE;
	const static float DEFAULT_FAR_PLANE;

	ShadowMapRenderer();

	IFrameBuffer* shadowMapFrameBuffer;
	Shader* shadowMapShader;
};
#endif // !SHADOW_MAP_RENDERER_H