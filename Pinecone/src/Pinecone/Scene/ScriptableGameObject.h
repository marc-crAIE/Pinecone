#pragma once

#include "Pinecone/Scene/GameObject.h"

namespace Pinecone
{
	class ScriptableGameObject
	{
	public:
		virtual ~ScriptableGameObject() { }

		//// GameObject functions

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_GameObject.AddComponent<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_GameObject.GetComponent<T>();
		}

		template<typename T>
		bool HasComponent()
		{
			return m_GameObject.HasComponent<T>();
		}

		template<typename T>
		void RemoveComponent()
		{
			m_GameObject.RemoveComponent<T>();
		}

		//// Scene interation functions

		Scene* GetScene() { return m_SceneContext; }

		GameObject GetGameObject() const { return m_GameObject; }
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		GameObject m_GameObject;
		Scene* m_SceneContext;
		friend class Scene;
	};
}