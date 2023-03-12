#pragma once

#include "Pinecone/Core/Timestep.h"

#include <entt.hpp>

namespace Pinecone
{
	class GameObject;

	class Scene
	{
	public:
		Scene();
		~Scene();

		GameObject CreateGameObject(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class GameObject;
	};
}