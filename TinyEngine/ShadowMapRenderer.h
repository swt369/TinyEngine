#ifndef SHADOW_MAP_RENDERER_H
#define SHADOW_MAP_RENDERER_H

#include <glad/glad.h>

#include "FrameBuffer.h"
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

class ShadowMapRenderer
{
public:
	static ShadowMapRenderer& getInstance();

	void RenderShadowMap();
	Texture* GetShadowMap();
private:
	ShadowMapRenderer();

	IFrameBuffer* shadowMapFrameBuffer;
	Shader* shadowMapShader;
};
#endif // !SHADOW_MAP_RENDERER_H