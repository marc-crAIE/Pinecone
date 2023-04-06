#pragma once

#include "Pinecone/Scene/Scene.h"
#include "Pinecone/Scene/Components.h"
#include "Pinecone/Core/UUID.h"

#include <entt.hpp>

namespace Pinecone
{
	class GameObject
	{
	public:
		/// <summary>
		/// The GameObject constructor
		/// </summary>
		GameObject() = default;
		/// <summary>
		/// The GameObject constructor that takes in an entity handle and a scene
		/// </summary>
		/// <param name="handle">The entity handle</param>
		/// <param name="scene">The scene the entity handle was created</param>
		GameObject(entt::entity handle, Scene* scene);
		/// <summary>
		/// The GameObject copy constructor
		/// </summary>
		GameObject(const GameObject& other) = default;

		/// <summary>
		/// Add a component
		/// </summary>
		/// <typeparam name="T">The component type</typeparam>
		/// <typeparam name="...Args">The arguments for the component</typeparam>
		/// <param name="...args">The arguments for the component</param>
		/// <returns>A reference to the component</returns>
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			PC_CORE_ASSERT(!HasComponent<T>(), "GameObject already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Get a component by type
		/// </summary>
		/// <typeparam name="T">The conponent type</typeparam>
		/// <returns>A reference to the component</returns>
		template<typename T>
		T& GetComponent()
		{
			PC_CORE_ASSERT(HasComponent<T>(), "GameObject does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		/// <summary>
		/// Check if the game object has a component
		/// </summary>
		/// <typeparam name="T">The component type</typeparam>
		/// <returns>True if the component exists</returns>
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		/// <summary>
		/// Remove a component by type
		/// </summary>
		/// <typeparam name="T">The component type</typeparam>
		template<typename T>
		void RemoveComponent()
		{
			PC_CORE_ASSERT(HasComponent<T>(), "GameObject does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		/// <summary>
		/// Returns true if the entity handle is created
		/// </summary>
		operator bool() const { return m_EntityHandle != entt::null; }
		/// <summary>
		/// Returns the entity handle for the game object
		/// </summary>
		operator entt::entity() const { return m_EntityHandle; }
		/// <summary>
		/// Returns the entity handle for the game object as an unsigned int
		/// </summary>
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		/// <summary>
		/// Gets the UUID of the game object
		/// </summary>
		/// <returns>The game objects UUID</returns>
		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		/// <summary>
		/// Gets the name (tag) of the game object
		/// </summary>
		/// <returns>Returns the game objects name (tag)</returns>
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		/// <summary>
		/// Checks if the GameObject is the same as another. Does this by checking if the entity handle and the
		/// scene it was created in of both objects are the same.
		/// </summary>
		/// <param name="other">The other game object</param>
		/// <returns>True if the game objects are the same</returns>
		bool operator==(const GameObject& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		/// <summary>
		/// Checks if the GameObject are not the same as another
		/// </summary>
		/// <param name="other">The other game object</param>
		/// <returns>True if the game objects are not the same</returns>
		bool operator!=(const GameObject& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}