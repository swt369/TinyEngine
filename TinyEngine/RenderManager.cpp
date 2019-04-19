#include "MapUtil.h"
#include "RenderManager.h"
#include "ShadowMapRenderer.h"

RenderManager & RenderManager::getInstance()
{
	static RenderManager instance;
	return instance;
}

void RenderManager::RenderWorld()
{
	renderPipeline->Render();
}

bool RenderManager::AddRenderer(MeshRenderer * renderer)
{
	int seq = renderer->GetRenderQueue();

	map<int, MeshRenderer*>* mp;
	if (!MapUtil::Contains(&renderQueue, seq))
	{
		mp = new map<int, MeshRenderer*>();
		renderQueue.insert(make_pair(seq, mp));
	}
	else
	{
		mp = renderQueue[seq];
	}

	int id = renderer->GetOwner()->id;
	if (MapUtil::Contains(mp, id))
	{
		return false;
	}

	mp->insert(make_pair(id, renderer));
	return true;
}

bool RenderManager::RemoveRenderer(MeshRenderer * renderer)
{
	int seq = renderer->GetRenderQueue();

	map<int, MeshRenderer*>* mp;
	if (!MapUtil::Contains(&renderQueue, seq))
	{
		return false;
	}

	mp = renderQueue[seq];
	int id = renderer->GetOwner()->id;
	if (!MapUtil::Contains(mp, id))
	{
		return false;
	}

	mp->erase(id);
	return true;
}

void RenderManager::SetMSAA(bool enabled)
{
	if (enabled == isMSAAEnabled)
	{
		return;
	}

	isMSAAEnabled = enabled;
	renderPipeline->SetMSAA(isMSAAEnabled);
}

void RenderManager::Prepare()
{
	renderPipeline = new RenderPipeline();
}

RenderManager::RenderManager()
{

}

void RenderManager::SortRenderQueue()
{
	opaqueQueue.clear();
	transparentQueue.clear();
	globalQueue.clear();

	auto i = renderQueue.begin();
	while (i != renderQueue.end())
	{
		int seq = i->first;
		if (seq > OPAQUE_RENDER_QUEUE_MAXIMUM)
		{
			break;
		}

		auto curQueue = i->second;
		for (auto j = curQueue->begin(); j != curQueue->end(); j++)
		{
			opaqueQueue.push_back(j->second);
		}

		i++;
	}

	if (i != renderQueue.end())
	{
		map<float, MeshRenderer*> distanceMap;
		while (i != renderQueue.end())
		{
			int seq = i->first;
			if (seq > TRANSPARENT_RENDER_QUEUE_MAXIMUM)
			{
				break;
			}

			auto curQueue = i->second;
			for (auto j = curQueue->begin(); j != curQueue->end(); j++)
			{
				float distance = glm::length(j->second->GetTransform()->position - renderingCamera->GetTransform()->position);
				distanceMap[distance] = j->second;
			}
			i++;
		}

		for (auto j = distanceMap.rbegin(); j != distanceMap.rend(); j++)
		{
			transparentQueue.push_back(j->second);
		}
	}

	while (i != renderQueue.end())
	{
		int seq = i->first;
		if (seq > GLOBAL_RENDER_QUEUE_MAXIMUM)
		{
			break;
		}

		auto curQueue = i->second;
		for (auto j = curQueue->begin(); j != curQueue->end(); j++)
		{
			globalQueue.push_back(j->second);
		}

		i++;
	}
}

void RenderManager::RenderObjects(Camera* camera, Shader* globalShader)
{
	renderingCamera = camera;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	for (MeshRenderer* renderer : opaqueQueue)
	{
		renderer->Draw(renderingCamera, globalShader);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (MeshRenderer* renderer : transparentQueue)
	{
		renderer->Draw(renderingCamera, globalShader);
	}

	for (MeshRenderer* renderer : globalQueue)
	{
		renderer->Draw(renderingCamera, globalShader);
	}

	renderingCamera = nullptr;
}