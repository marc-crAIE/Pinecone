#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace Sandbox
{
	class CameraController : public ScriptableGameObject
	{
	public:
		void OnCreate()
		{
		}

		void OnDestroy()
		{
		}

		void OnUpdate(Timestep ts)
		{
			auto& transform = GetComponent<TransformComponent>();
			float speed = 5.0f;

			if (Input::IsKeyPressed(Key::W))
			{
				transform.Translation += glm::vec3{ 0.0f, speed, 0.0f } * (float)ts;
			}
			if (Input::IsKeyPressed(Key::S))
			{
				transform.Translation += glm::vec3{ 0.0f, -speed, 0.0f } * (float)ts;
			}
			if (Input::IsKeyPressed(Key::A))
			{
				transform.Translation += glm::vec3{ -speed, 0.0f, 0.0f } * (float)ts;
			}
			if (Input::IsKeyPressed(Key::D))
			{
				transform.Translation += glm::vec3{ speed, 0.0f, 0.0f } * (float)ts;
			}
		}
	};
}