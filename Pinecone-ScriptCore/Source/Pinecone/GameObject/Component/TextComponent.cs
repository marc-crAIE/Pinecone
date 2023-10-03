namespace Pinecone
{
    public class TextComponent : Component
    {

        public string Text
        {
            get => InternalCalls.TextComponent_GetText(GameObject.ID);
            set => InternalCalls.TextComponent_SetText(GameObject.ID, value);
        }

        public Vector4 Color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(GameObject.ID, out Vector4 color);
                return color;
            }

            set
            {
                InternalCalls.TextComponent_SetColor(GameObject.ID, ref value);
            }
        }

        public float Kerning
        {
            get => InternalCalls.TextComponent_GetKerning(GameObject.ID);
            set => InternalCalls.TextComponent_SetKerning(GameObject.ID, value);
        }

        public float LineSpacing
        {
            get => InternalCalls.TextComponent_GetLineSpacing(GameObject.ID);
            set => InternalCalls.TextComponent_SetLineSpacing(GameObject.ID, value);
        }

    }
}
