#pragma once

#include "Pinecone/Scene/GameObject.h"

namespace Pinecone
{
	class ScriptableGameObject
	{
	public:
		virtual ~ScriptableGameObject() {}

		template<typename T>
		T& GetComponent()
		{
			return m_GameObject.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestory() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		GameObject m_GameObject;
		friend class Scene;
	};
}