#pragma once

#include "Pinecone/Core/UUID.h"
#include "Pinecone/Scene/SceneCamera.h"
#include "Pinecone/Renderer/Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Pinecone
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		operator UUID& () { return ID; }
		operator const UUID& () { return ID; }
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
		glm::vec2 FlipAxies = { 0.0f, 0.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color)
			: Color(color)
		{}

		void FlipX(bool flipped) { FlipAxies.x = flipped; }
		void FlipY(bool flipped) { FlipAxies.y = flipped; }
	};

	struct CameraComponent
	{
		Pinecone::SceneCamera Camera;
		bool Primary = true; // TODO: Probably should be managed directly by/moved to Scene

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// This a the component for native C++ scripting!
	class ScriptableGameObject;
	struct NativeScriptComponent
	{
		ScriptableGameObject* Instance = nullptr;

		void (*DestroyScript)(NativeScriptComponent*);

		bool Instantiated = false;

		template<typename T, typename... Args>
		void Bind(Args&&... args)
		{
			Instance = static_cast<ScriptableGameObject*>(new T(std::forward<Args>(args)...));
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}