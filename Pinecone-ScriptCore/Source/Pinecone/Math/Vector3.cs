namespace Pinecone
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);
        public static Vector3 One => new Vector3(1.0f);
        public static Vector3 NegativeInfinity = new Vector3(float.NegativeInfinity);
        public static Vector3 PositiveInfinity = new Vector3(float.PositiveInfinity);

        public static Vector3 Up => new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 Down => new Vector3(0.0f, -1.0f, 0.0f);
        public static Vector3 Left => new Vector3(-1.0f, 0.0f, 0.0f);
        public static Vector3 Right => new Vector3(1.0f, 0.0f, 0.0f);
        public static Vector3 Forward => new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 Back => new Vector3(0.0f, 0.0f, -1.0f);

        #region Constructors

        public Vector3(float scalar = 0.0f)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(Vector2 xy, float z)
        {
            X = xy.X;
            Y = xy.Y;
            Z = z;
        }

        #endregion

        #region Properties

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        public float Magnitude => MathF.Sqrt(X * X + Y * Y + Z * Z);

        public Vector3 Normalized
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
                    case 2: return Z;
                }
                return 0;
            }
            set
            {
                switch (i)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                }
            }
        }

        #endregion

        #region Operators

        #region Unary Operators

        public static Vector3 operator +(Vector3 v)
        {
            return v;
        }

        public static Vector3 operator -(Vector3 v)
        {
            Vector3 result = new Vector3();
            result.X = -v.X;
            result.Y = -v.Y;
            result.Z = -v.Z;
            return result;
        }

        public static Vector3 operator ++(Vector3 v)
        {
            Vector3 result = new Vector3();
            result.X = v.X++;
            result.Y = v.Y++;
            result.Z = v.Z++;
            return v;
        }

        public static Vector3 operator --(Vector3 v)
        {
            Vector3 result = new Vector3();
            result.X = v.X--;
            result.Y = v.Y--;
            result.Z = v.Z--;
            return v;
        }

        #endregion

        #region Binary Operators

        public static Vector3 operator +(Vector3 v, float scalar) => new Vector3(v.X + scalar, v.Y + scalar, v.Z + scalar);
        public static Vector3 operator +(float scalar, Vector3 v) => v + scalar;
        public static Vector3 operator +(Vector3 v1, Vector2 v2) => new Vector3(v1.X + v2.X, v1.Y + v2.Y, v1.Z);
        public static Vector3 operator +(Vector3 v1, Vector3 v2) => new Vector3(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
        public static Vector3 operator +(Vector3 v1, Vector4 v2) => new Vector3(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);

        public static Vector3 operator -(Vector3 v, float scalar) => new Vector3(v.X - scalar, v.Y - scalar, v.Z - scalar);
        public static Vector3 operator -(float scalar, Vector3 v) => v - scalar;
        public static Vector3 operator -(Vector3 v1, Vector2 v2) => new Vector3(v1.X - v2.X, v1.Y - v2.Y, v1.Z);
        public static Vector3 operator -(Vector3 v1, Vector3 v2) => new Vector3(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
        public static Vector3 operator -(Vector3 v1, Vector4 v2) => new Vector3(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);

        public static Vector3 operator *(Vector3 v, float scalar) => new Vector3(v.X * scalar, v.Y * scalar, v.Z * scalar);
        public static Vector3 operator *(float scalar, Vector3 v) => v * scalar;
        public static Vector3 operator *(Vector3 v1, Vector2 v2) => new Vector3(v1.X * v2.X, v1.Y * v2.Y, v1.Z);
        public static Vector3 operator *(Vector3 v1, Vector3 v2) => new Vector3(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z);
        public static Vector3 operator *(Vector3 v1, Vector4 v2) => new Vector3(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z);

        public static Vector3 operator /(Vector3 v, float scalar) => new Vector3(v.X / scalar, v.Y / scalar, v.Z / scalar);
        public static Vector3 operator /(float scalar, Vector3 v) => v / scalar;
        public static Vector3 operator /(Vector3 v1, Vector2 v2) => new Vector3(v1.X / v2.X, v1.Y / v2.Y, v1.Z);
        public static Vector3 operator /(Vector3 v1, Vector3 v2) => new Vector3(v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z);
        public static Vector3 operator /(Vector3 v1, Vector4 v2) => new Vector3(v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z);

        #endregion

        #endregion

        #region Function Overloads

        public override string ToString() => $"( {X}, {Y}, {Z} )";

        #endregion
    }
}
