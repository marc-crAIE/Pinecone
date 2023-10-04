using System;
using System.Runtime.CompilerServices;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Pinecone
{
    public static class InternalCalls
    {
        #region GameObject

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GameObject_New(string name);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GameObject_HasComponent(ulong gameObjectID, Type componentType);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong GameObject_FindGameObjectByName(string name);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong GameObject_GetGameObjectByUUID(ulong gameObjectID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GameObject_GetScriptInstance(ulong gameObjectID);

        #endregion

        #region TransformComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong gameObjectID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong gameObjectID, ref Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(ulong gameObjectID, out Vector3 rotation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(ulong gameObjectID, ref Vector3 rotation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(ulong gameObjectID, out Vector3 scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(ulong gameObjectID, ref Vector3 scale);

        #endregion

        #region TextComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TextComponent_GetText(ulong gameObjectID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetText(ulong gameObjectID, string text);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_GetColor(ulong gameObjectID, out Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetColor(ulong gameObjectID, ref Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextComponent_GetKerning(ulong gameObjectID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetKerning(ulong gameObjectID, float kerning);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextComponent_GetLineSpacing(ulong gameObjectID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetLineSpacing(ulong gameObjectID, float lineSpacing);

        #endregion

        #region Input

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseButtonPressed(MouseCode button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMousePosition(out Vector2 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMouseX(out float xPosition);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMouseY(out float yPosition);

        #endregion
    }
}
