namespace Pinecone
{
    public sealed class Graphics2D
    {
        public static void DrawQuad(Vector3 position, Vector2 size)
        {
            DrawQuad(position, size, Vector4.One);
        }

        public static void DrawQuad(Vector3 position, Vector2 size, Vector4 color)
        {
            InternalCalls.Graphics2D_DrawQuad(ref position, ref size, ref color);
        }

        public static void DrawRotatedQuad(Vector3 position, Vector2 size, float rotation)
        {
            DrawRotatedQuad(position, size, rotation, Vector4.One);
        }

        public static void DrawRotatedQuad(Vector3 position, Vector2 size, float rotation, Vector4 color)
        {
            InternalCalls.Graphics2D_DrawRotatedQuad(ref position, ref size, rotation, ref color);
        }

        public static void DrawTexture(Vector3 position, Vector2 size, Texture2D texture)
        {
            DrawTexture(position, size, texture, 1.0f, Vector2.Zero, Vector4.One);
        }

        public static void DrawTexture(Vector3 position, Vector2 size, Texture2D texture, Vector4 tintColor)
        {
            DrawTexture(position, size, texture, 1.0f, Vector2.Zero, tintColor);
        }

        public static void DrawTexture(Vector3 position, Vector2 size, Texture2D texture, float tilingFactor, Vector4 tintColor)
        {
            DrawTexture(position, size, texture, tilingFactor, Vector2.Zero, tintColor);
        }

        public static void DrawTexture(Vector3 position, Vector2 size, Texture2D texture, float tilingFactor, Vector2 flipAxies, Vector4 tintColor)
        {
            InternalCalls.Graphics2D_DrawTexture(ref  position, ref size, texture.Handle, tilingFactor, ref flipAxies, ref tintColor);
        }

        public static void DrawRotatedTexture(Vector3 position, Vector2 size, float rotation, Texture2D texture)
        {
            DrawRotatedTexture(position, size, rotation, texture, 1.0f, Vector2.Zero, Vector4.One);
        }

        public static void DrawRotatedTexture(Vector3 position, Vector2 size, float rotation, Texture2D texture, Vector4 tintColor)
        {
            DrawRotatedTexture(position, size, rotation, texture, 1.0f, Vector2.Zero, tintColor);
        }

        public static void DrawRotatedTexture(Vector3 position, Vector2 size, float rotation, Texture2D texture, float tilingFactor, Vector4 tintColor)
        {
            DrawRotatedTexture(position, size, rotation, texture, tilingFactor, Vector2.Zero, tintColor);
        }

        public static void DrawRotatedTexture(Vector3 position, Vector2 size, float rotation, Texture2D texture, float tilingFactor, Vector2 flipAxies, Vector4 tintColor)
        {
            InternalCalls.Graphics2D_DrawRotatedTexture(ref position, ref size, rotation, texture.Handle, tilingFactor, ref flipAxies, ref tintColor);
        }
    }
}
