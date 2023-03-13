#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace Sandbox
{
	class CameraController : public ScriptableGameObject
	{
	public:
		CameraController(std::string& test)
		{
			PC_INFO("CameraComponent: {0}", test);
		}

		void OnCreate() override
		{
			auto objects = GetScene()->GetGameObjectsByTag("TestObject");
			PC_INFO("TestObject Count: {0}", objects.size());
		}

		void OnDestroy() override
		{
		}

		void OnUpdate(Timestep ts) override
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