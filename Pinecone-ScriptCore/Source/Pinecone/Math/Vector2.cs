namespace Pinecone
{
    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero => new Vector2(0.0f);
        public static Vector2 One = new Vector2(1.0f);
        public static Vector2 NegativeInfinity = new Vector2(float.NegativeInfinity);
        public static Vector2 PositiveInfinity = new Vector2(float.PositiveInfinity);

        public static Vector2 Up = new Vector2(0.0f, 1.0f);
        public static Vector2 Down = new Vector2(0.0f, -1.0f);
        public static Vector2 Left = new Vector2(-1.0f, 0.0f);
        public static Vector2 Right = new Vector2(1.0f, 0.0f);

        #region Constructors

        public Vector2(float scalar = 0.0f)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        #endregion

        #region Properties

        public float Magnitude => MathF.Sqrt(X * X + Y * Y);

        public Vector2 Normalized
        {
            get
            {
                float mag = Magnitude;

                if (mag == 0.0f)
                    return Zero;

                return this / mag;
            }
        }

        public float this[int i]
        {
            get
            {
                switch (i)
                {
                    case 0: return X;
                    case 1: return Y;
                }
                return 0;
            }
            set
            {
                switch (i)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                }
            }
        }

        #endregion

        #region Operators

        #region Unary Operators

        public static Vector2 operator +(Vector2 v)
        {
            return v;
        }

        public static Vector2 operator -(Vector2 v)
        {
            Vector2 result = new Vector2();
            result.X = -v.X;
            result.Y = -v.Y;
            return result;
        }

        public static Vector2 operator ++(Vector2 v)
        {
            Vector2 result = new Vector2();
            result.X = v.X++;
            result.Y = v.Y++;
            return v;
        }

        public static Vector2 operator --(Vector2 v)
        {
            Vector2 result = new Vector2();
            result.X = v.X--;
            result.Y = v.Y--;
            return v;
        }

        #endregion

        #region Binary Operators

        public static Vector2 operator +(Vector2 v, float scalar) => new Vector2(v.X + scalar, v.Y + scalar);
        public static Vector2 operator +(float scalar, Vector2 v) => v + scalar;
        public static Vector2 operator +(Vector2 v1, Vector2 v2) => new Vector2(v1.X + v2.X, v1.Y + v2.Y);
        public static Vector2 operator +(Vector2 v1, Vector3 v2) => new Vector2(v1.X + v2.X, v1.Y + v2.Y);
        public static Vector2 operator +(Vector2 v1, Vector4 v2) => new Vector2(v1.X + v2.X, v1.Y + v2.Y);

        public static Vector2 operator -(Vector2 v, float scalar) => new Vector2(v.X - scalar, v.Y - scalar);
        public static Vector2 operator -(float scalar, Vector2 v) => v - scalar;
        public static Vector2 operator -(Vector2 v1, Vector2 v2) => new Vector2(v1.X - v2.X, v1.Y - v2.Y);
        public static Vector2 operator -(Vector2 v1, Vector3 v2) => new Vector2(v1.X - v2.X, v1.Y - v2.Y);
        public static Vector2 operator -(Vector2 v1, Vector4 v2) => new Vector2(v1.X - v2.X, v1.Y - v2.Y);

        public static Vector2 operator *(Vector2 v, float scalar) => new Vector2(v.X * scalar, v.Y * scalar);
        public static Vector2 operator *(float scalar, Vector2 v) => v * scalar;
        public static Vector2 operator *(Vector2 v1, Vector2 v2) => new Vector2(v1.X * v2.X, v1.Y * v2.Y);
        public static Vector2 operator *(Vector2 v1, Vector3 v2) => new Vector2(v1.X * v2.X, v1.Y * v2.Y);
        public static Vector2 operator *(Vector2 v1, Vector4 v2) => new Vector2(v1.X * v2.X, v1.Y * v2.Y);

        public static Vector2 operator /(Vector2 v, float scalar) => new Vector2(v.X / scalar, v.Y / scalar);
        public static Vector2 operator /(float scalar, Vector2 v) => v / scalar;
        public static Vector2 operator /(Vector2 v1, Vector2 v2) => new Vector2(v1.X / v2.X, v1.Y / v2.Y);
        public static Vector2 operator /(Vector2 v1, Vector3 v2) => new Vector2(v1.X / v2.X, v1.Y / v2.Y);
        public static Vector2 operator /(Vector2 v1, Vector4 v2) => new Vector2(v1.X / v2.X, v1.Y / v2.Y);

        #endregion

        #endregion

        #region Function Overloads

        public override string ToString() => $"( {X}, {Y} )";

        #endregion
    }
}
