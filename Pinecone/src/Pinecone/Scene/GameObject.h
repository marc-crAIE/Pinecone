#pragma once

#include "Pinecone/Scene/Scene.h"
#include "Pinecone/Scene/Components.h"

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
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const GameObject& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const GameObject& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}