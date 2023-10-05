namespace Pinecone
{
    public class SpriteComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                InternalCalls.SpriteComponent_GetColor(GameObject.ID, out Vector4 color);
                return color;
            }

            set => InternalCalls.SpriteComponent_SetColor(GameObject.ID, ref value);
        }

        public Texture2D Texture
        {
            get
            {
                ulong handle = InternalCalls.SpriteComponent_GetTexture(GameObject.ID);
                if (handle == 0)
                    return null;
                Texture2D texture = new Texture2D(handle);
                return texture;
            }
            set => InternalCalls.SpriteComponent_SetTexture(GameObject.ID, value.Handle);
        }

        public float TilingFactor
        {
            get => InternalCalls.SpriteComponent_GetTilingFactor(GameObject.ID);
            set => InternalCalls.SpriteComponent_SetTilingFactor(GameObject.ID, value);
        }
    }
}
