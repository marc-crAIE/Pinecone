#include "pcpch.h"
#include "GameObject.h"

namespace Pinecone
{
	GameObject::GameObject(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}