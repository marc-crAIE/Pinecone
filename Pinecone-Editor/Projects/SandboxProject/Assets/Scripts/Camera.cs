using System;
using Pinecone;

namespace Sandbox
{
    public class Camera : GameObject
    {
        public float Speed = 3.0f;

        void OnCreate()
        {

        }

        void OnUpdate(float ts)
        {
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.Up) || Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.Down) || Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.Left) || Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.Right) || Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            Vector3 translation = Translation;
            translation += velocity * Speed * ts;
            Translation = translation;
        }
    }
}
