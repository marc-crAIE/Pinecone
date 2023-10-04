#include "pcpch.h"
#include "ScriptGlue.h"

#include "Pinecone/Scripting/ScriptEngine.h"

#include "Pinecone/Core/UUID.h"
#include "Pinecone/Core/KeyCodes.h"
#include "Pinecone/Core/Input.h"

#include "Pinecone/Scene/Scene.h"
#include "Pinecone/Scene/GameObject.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace Pinecone
{
	namespace Utils 
	{
		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}
	}

	static std::unordered_map<MonoType*, std::function<bool(GameObject)>> s_GameObjectHasComponentFuncs;
#define PC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Pinecone.InternalCalls::" #Name, Name)


#pragma region GameObject

	static uint64_t GameObject_New(MonoString* name)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->CreateGameObject(Utils::MonoStringToString(name));
		PC_CORE_ASSERT(gameObject);

		return gameObject.GetUUID();
	}

	static bool GameObject_HasComponent(UUID gameObjectID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		PC_CORE_ASSERT(s_GameObjectHasComponentFuncs.find(managedType) != s_GameObjectHasComponentFuncs.end());
		return s_GameObjectHasComponentFuncs.at(managedType)(gameObject);
	}

	static uint64_t GameObject_FindGameObjectByName(MonoString* name)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByTag(Utils::MonoStringToString(name));

		if (!gameObject)
			return 0;

		return gameObject.GetUUID();
	}

	static uint64_t GameObject_GetGameObjectByUUID(UUID gameObjectID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);

		if (!gameObject)
			return 0;

		return gameObject.GetUUID();
	}

	static MonoObject* GameObject_GetScriptInstance(UUID gameObjectID)
	{
		return ScriptEngine::GetManagedInstance(gameObjectID);
	}

#pragma endregion

#pragma region TransformComponent

	static void TransformComponent_GetTranslation(UUID gameObjectID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		*outTranslation = gameObject.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID gameObjectID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		gameObject.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void TransformComponent_GetRotation(UUID gameObjectID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		*outRotation = gameObject.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID gameObjectID, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		gameObject.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID gameObjectID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		*outScale = gameObject.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID gameObjectID, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);

		gameObject.GetComponent<TransformComponent>().Scale = *scale;
	}

#pragma endregion

#pragma region TextComponent

	static MonoString* TextComponent_GetText(UUID gameObjectID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		return ScriptEngine::CreateString(tc.TextString.c_str());
	}

	static void TextComponent_SetText(UUID gameObjectID, MonoString* textString)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		tc.TextString = Utils::MonoStringToString(textString);
	}

	static void TextComponent_GetColor(UUID gameObjectID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		*color = tc.Color;
	}

	static void TextComponent_SetColor(UUID gameObjectID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		tc.Color = *color;
	}

	static float TextComponent_GetKerning(UUID gameObjectID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		return tc.Kerning;
	}

	static void TextComponent_SetKerning(UUID gameObjectID, float kerning)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		tc.Kerning = kerning;
	}

	static float TextComponent_GetLineSpacing(UUID gameObjectID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		return tc.LineSpacing;
	}

	static void TextComponent_SetLineSpacing(UUID gameObjectID, float lineSpacing)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);
		PC_CORE_ASSERT(gameObject);
		PC_CORE_ASSERT(gameObject.HasComponent<TextComponent>());

		auto& tc = gameObject.GetComponent<TextComponent>();
		tc.LineSpacing = lineSpacing;
	}

#pragma endregion

#pragma region Input

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	static bool Input_IsMouseButtonPressed(MouseCode button)
	{
		return Input::IsMouseButtonPressed(button);
	}

	static void Input_GetMousePosition(glm::vec2* position)
	{
		*position = Input::GetMousePosition();
	}

	static void Input_GetMouseX(float* xPosition)
	{
		*xPosition = Input::GetMouseX();
	}

	static void Input_GetMouseY(float* yPosition)
	{
		*yPosition = Input::GetMouseY();
	}

#pragma endregion

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Pinecone.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					PC_CORE_ERROR("Could not find C# component type {}", managedTypename);
					return;
				}
				s_GameObjectHasComponentFuncs[managedType] = [](GameObject gameObject) { return gameObject.HasComponent<Component>(); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		PC_CORE_INFO("Registering C# component classes");

		s_GameObjectHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		PC_CORE_INFO("Registering C# internal functions");

		PC_ADD_INTERNAL_CALL(GameObject_New);
		PC_ADD_INTERNAL_CALL(GameObject_HasComponent);
		PC_ADD_INTERNAL_CALL(GameObject_FindGameObjectByName);
		PC_ADD_INTERNAL_CALL(GameObject_GetGameObjectByUUID);
		PC_ADD_INTERNAL_CALL(GameObject_GetScriptInstance);

		PC_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		PC_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		PC_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		PC_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		PC_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		PC_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		PC_ADD_INTERNAL_CALL(TextComponent_GetText);
		PC_ADD_INTERNAL_CALL(TextComponent_SetText);
		PC_ADD_INTERNAL_CALL(TextComponent_GetColor);
		PC_ADD_INTERNAL_CALL(TextComponent_SetColor);
		PC_ADD_INTERNAL_CALL(TextComponent_GetKerning);
		PC_ADD_INTERNAL_CALL(TextComponent_SetKerning);
		PC_ADD_INTERNAL_CALL(TextComponent_GetLineSpacing);
		PC_ADD_INTERNAL_CALL(TextComponent_SetLineSpacing);

		PC_ADD_INTERNAL_CALL(Input_IsKeyDown);
		PC_ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
		PC_ADD_INTERNAL_CALL(Input_GetMousePosition);
		PC_ADD_INTERNAL_CALL(Input_GetMouseX);
		PC_ADD_INTERNAL_CALL(Input_GetMouseY);
	}
}