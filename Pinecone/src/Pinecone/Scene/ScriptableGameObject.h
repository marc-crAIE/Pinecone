#pragma once

#include "Pinecone/Scene/GameObject.h"

namespace Pinecone
{
	class ScriptableGameObject
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_GameObject.GetComponent<T>();
		}
	private:
		GameObject m_GameObject;
		friend class Scene;
	};
}