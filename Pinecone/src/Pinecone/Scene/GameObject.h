#pragma once

#include "Pinecone/Scene/Scene.h"

#include <entt.hpp>

namespace Pinecone
{
	class GameObject
	{
	public:
		GameObject() = default;
		GameObject(entt::entity handle, Scene* scene);
		GameObject(const GameObject& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			PC_CORE_ASSERT(!HasComponent<T>(), "GameObject already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			PC_CORE_ASSERT(HasComponent<T>(), "GameObject does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			PC_CORE_ASSERT(HasComponent<T>(), "GameObject does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}