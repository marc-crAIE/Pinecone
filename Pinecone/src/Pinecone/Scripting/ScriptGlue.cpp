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
	static std::unordered_map<MonoType*, std::function<bool(GameObject)>> s_GameObjectHasComponentFuncs;
#define PC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Pinecone.InternalCalls::" #Name, Name)


#pragma region GameObject

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
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		PC_CORE_ASSERT(scene);
		GameObject gameObject = scene->GetGameObjectByTag(nameCStr);
		mono_free(nameCStr);

		if (!gameObject)
			return 0;

		return gameObject.GetUUID();
	}

	static MonoObject* GameObject_GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
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

#pragma endregion

#pragma region Input

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
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

		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		PC_CORE_INFO("Registering C# internal functions");

		PC_ADD_INTERNAL_CALL(GameObject_HasComponent);
		PC_ADD_INTERNAL_CALL(GameObject_FindGameObjectByName);
		PC_ADD_INTERNAL_CALL(GameObject_GetScriptInstance);

		PC_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		PC_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		PC_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}