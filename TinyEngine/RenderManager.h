#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <map>
#include <vector>
using namespace std;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "FrameBuffer.h"
#include "MeshRenderer.h"
#include "RenderPipeline.h"
#include "Shader.h"
#include "SystemSettings.h"

class RenderManager
{
	friend class RenderObjectStage;
	friend class ShadowMapRenderer;
public:
	static const int DEFAULT_RENDER_QUEUE = 1000;
	static const int OPAQUE_RENDER_QUEUE_MAXIMUM = 1999;
	static const int TRANSPARENT_RENDER_QUEUE_MAXIMUM = 2999;
	static const int GLOBAL_RENDER_QUEUE_MAXIMUM = 3999;

	static RenderManager& getInstance();

	void Prepare();
	void RenderWorld(Camera* camera);
	bool AddRenderer(MeshRenderer* renderer);
	bool RemoveRenderer(MeshRenderer* renderer);
	void SetMSAA(bool enabled);

	Camera* renderingCamera;
	map<int, map<int, MeshRenderer*>*> renderQueue;
	bool isMSAAEnabled = true;
private:
	RenderPipeline* renderPipeline;

	vector<MeshRenderer*> opaqueQueue;
	vector<MeshRenderer*> transparentQueue;
	vector<MeshRenderer*> globalQueue;

	RenderManager();

	void SortRenderQueue();
	void RenderObjects(Shader* globalShader = nullptr);
};

#endif // !RENDER_MANAGER_H