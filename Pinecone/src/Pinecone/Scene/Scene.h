#pragma once

#include "Pinecone/Core/UUID.h"
#include "Pinecone/Core/Timestep.h"
#include "Pinecone/Renderer/EditorCamera.h"

#include <entt.hpp>

namespace Pinecone
{
	class GameObject;

	class Scene
	{
	public:
		/// <summary>
		/// The Scene constructor
		/// </summary>
		Scene(const std::string& name = "Untitled");
		/// <summary>
		/// The Scene destructor
		/// </summary>
		~Scene() = default;

		void OnRuntimeStart();
		void OnRuntimeStop();

		/// <summary>
		/// Called when the scene runtime is to be updated.
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdateRuntime(Timestep ts);
		/// <summary>
		/// Called when the scene in editor mode is to be updated.
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		/// <summary>
		/// Create a new game object in the scene. Can also accept a name that will
		/// be used to set the value of the tag component for the game object
		/// </summary>
		/// <param name="name">The value of the tag component</param>
		/// <returns></returns>
		GameObject CreateGameObject(const std::string& name = std::string());
		GameObject CreateGameObjectWithUUID(UUID uuid, const std::string& name = std::string());
		/// <summary>
		/// Destroys a game object and removes it from the scene
		/// </summary>
		/// <param name="gameObject">The game object to be destroyed</param>
		void DestroyGameObject(GameObject gameObject);

		/// <summary>
		/// Get the first game object with the specified tag.
		/// The entity handle of the game object returned is null if no game objects
		/// where found with that tag.
		/// </summary>
		/// <param name="name">The name (tag) to search for</param>
		/// <returns>The found game object</returns>
		GameObject GetGameObjectByTag(std::string_view name);
		/// <summary>
		/// Get the first game object with the specified UUID.
		/// The entity handle of the game object returned is null if no game objects
		/// where found with that UUID.
		/// </summary>
		/// <param name="name">The UUID to search for</param>
		/// <returns>The found game object</returns>
		GameObject GetGameObjectByUUID(UUID uuid);

		/// <summary>
		/// Get all of the game objects with the specified tag.
		/// The vector will be empty if there are no game objects found with that tag
		/// </summary>
		/// <param name="name">The name (tag) to search for</param>
		/// <returns>The found game objects</returns>
		std::vector<GameObject> GetGameObjectsByTag(std::string_view name);

		/// <summary>
		/// Gets the game object that has a camera component that is flagged as the 
		/// primary camera. The entity handle of the game object returned is null if
		/// there is no camera game object that also is a primary camera
		/// </summary>
		/// <returns>The game object with the primary camera component</returns>
		GameObject GetPrimaryCameraGameObject();

		/// <summary>
		/// To be called when the viewport (window) is resized. This changes the viewport
		/// for all game objects with camera components
		/// </summary>
		void OnViewportResize(uint32_t width, uint32_t height);

		bool IsRunning() const { return m_Running; }

		static Ref<Scene> Copy(Ref<Scene> other);
	private:
		void RenderScene(EditorCamera& camera);
	private:
		UUID m_SceneID;
		std::string m_Name;

		entt::registry m_Registry;
		std::unordered_map<UUID, entt::entity> m_GameObjectMap;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_Running = false;

		entt::entity m_SceneEntity;

		friend class GameObject;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}