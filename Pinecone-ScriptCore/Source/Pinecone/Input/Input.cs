namespace Pinecone
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static bool IsMouseButtonPressed(MouseCode button)
        {
            return InternalCalls.Input_IsMouseButtonPressed(button);
        }

        public static Vector2 GetMousePosition()
        {
            InternalCalls.Input_GetMousePosition(out Vector2 position);
            return position;
        }

        public static float GetMouseX()
        {
            InternalCalls.Input_GetMouseX(out float x);
            return x;
        }

        public static float GetMouseY()
        {
            InternalCalls.Input_GetMouseY(out float y);
            return y;
        }
    }
}
