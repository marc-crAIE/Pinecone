#pragma once

#include "Pinecone/Scene/GameObject.h"

namespace Pinecone
{
	class ScriptableGameObject
	{
	public:
		virtual ~ScriptableGameObject() { }

		//// GameObject functions

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
			return m_GameObject.AddComponent<T>(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Get a component by type
		/// </summary>
		/// <typeparam name="T">The conponent type</typeparam>
		/// <returns>A reference to the component</returns>
		template<typename T>
		T& GetComponent()
		{
			return m_GameObject.GetComponent<T>();
		}

		/// <summary>
		/// Check if the game object has a component
		/// </summary>
		/// <typeparam name="T">The component type</typeparam>
		/// <returns>True if the component exists</returns>
		template<typename T>
		bool HasComponent()
		{
			return m_GameObject.HasComponent<T>();
		}

		/// <summary>
		/// Remove a component by type
		/// </summary>
		/// <typeparam name="T">The component type</typeparam>
		template<typename T>
		void RemoveComponent()
		{
			m_GameObject.RemoveComponent<T>();
		}

		//// Scene interation functions

		/// <summary>
		/// Get the scene that the game object was created in
		/// </summary>
		/// <returns>The scene the game object was created in</returns>
		Scene* GetScene() { return m_SceneContext; }

		/// <summary>
		/// Returns the game object the script is attached to
		/// </summary>
		/// <returns>The game object the script is attached to</returns>
		GameObject GetGameObject() const { return m_GameObject; }
	protected:
		/// <summary>
		/// Called when the script is instantiated
		/// </summary>
		virtual void OnCreate() {}
		/// <summary>
		/// Called when the script is destroyed
		/// </summary>
		virtual void OnDestroy() {}
		/// <summary>
		/// Called when the script is updated
		/// </summary>
		/// <param name="ts"></param>
		virtual void OnUpdate(Timestep ts) {}
	private:
		GameObject m_GameObject;
		Scene* m_SceneContext;
		friend class Scene;
	};
}