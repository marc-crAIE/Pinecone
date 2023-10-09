using System;
using System.Collections.Generic;

namespace Pinecone
{
    public class GameObject
    {
        private Dictionary<Type, Component> m_ComponentCache = new Dictionary<Type, Component>();

        protected GameObject() { ID = 0; }

        internal GameObject(ulong id)
        {
            ID = id;
        }

        public GameObject(string name = "GameObject")
        {
            ID = InternalCalls.GameObject_New(name);
        }

        public readonly ulong ID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        }

        public T AddComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
                return GetComponent<T>();

            Type componentType = typeof(T);
            InternalCalls.GameObject_AddComponent(ID, componentType);
            T component = new T { GameObject = this };
            m_ComponentCache.Add(componentType, component);
            return component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.GameObject_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { GameObject = this };
            return component;
        }

        public static GameObject FindGameObjectByName(string name)
        {
            ulong gameObjectID = InternalCalls.GameObject_FindGameObjectByName(name);
            if (gameObjectID == 0)
                return null;

            return new GameObject(gameObjectID);
        }

        public static GameObject GetGameObjectByUUID(ulong uuid)
        {
            ulong gameObjectID = InternalCalls.GameObject_GetGameObjectByUUID(uuid);
            if (gameObjectID == 0)
                return null;

            return new GameObject(gameObjectID);
        }

        public T As<T>() where T : GameObject, new()
        {
            object instance = InternalCalls.GameObject_GetScriptInstance(ID);
            return instance as T;
        }
    }
}
