namespace Pinecone
{
    public class Vector4
    {
        public float X, Y, Z, W;

        public static Vector4 Zero => new Vector4(0.0f);
        public static Vector4 One => new Vector4(1.0f);
        public static Vector4 NegativeInfinity = new Vector4(float.NegativeInfinity);
        public static Vector4 PositiveInfinity = new Vector4(float.PositiveInfinity);

        #region Constructors

        public Vector4(float scalar = 0.0f)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(Vector3 xyz, float w)
        {
            X = xyz.X;
            Y = xyz.Y;
            Z = xyz.Z;
            W = w;
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

        public Vector3 XYZ
        {
            get => new Vector3(X, Y, Z);
            set
            {
                X = value.X;
                Y = value.Y;
                Z = value.Z;
            }
        }

        public float Magnitude => MathF.Sqrt(X * X + Y * Y + Z * Z + W * W);

        public Vector4 Normalized
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
                    case 3: return W;
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
                    case 3: W = value; break;
                }
            }
        }

        #endregion

        #region Operators

        #region Unary Operators

        public static Vector4 operator +(Vector4 v)
        {
            return v;
        }

        public static Vector4 operator -(Vector4 v)
        {
            Vector4 result = new Vector4();
            result.X = -v.X;
            result.Y = -v.Y;
            result.Z = -v.Z;
            result.W = -v.W;
            return result;
        }

        public static Vector4 operator ++(Vector4 v)
        {
            Vector4 result = new Vector4();
            result.X = v.X++;
            result.Y = v.Y++;
            result.Z = v.Z++;
            result.W = v.W++;
            return v;
        }

        public static Vector4 operator --(Vector4 v)
        {
            Vector4 result = new Vector4();
            result.X = v.X--;
            result.Y = v.Y--;
            result.Z = v.Z--;
            result.W = v.W--;
            return v;
        }

        #endregion

        #region Binary Operators

        public static Vector4 operator +(Vector4 v, float scalar) => new Vector4(v.X + scalar, v.Y + scalar, v.Z + scalar, v.W + scalar);
        public static Vector4 operator +(float scalar, Vector4 v) => v + scalar;
        public static Vector4 operator +(Vector4 v1, Vector2 v2) => new Vector4(v1.X + v2.X, v1.Y + v2.Y, v1.Z, v1.W);
        public static Vector4 operator +(Vector4 v1, Vector3 v2) => new Vector4(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z, v1.W);
        public static Vector4 operator +(Vector4 v1, Vector4 v2) => new Vector4(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z, v1.W + v2.W);

        public static Vector4 operator -(Vector4 v, float scalar) => new Vector4(v.X - scalar, v.Y - scalar, v.Z - scalar, v.W - scalar);
        public static Vector4 operator -(float scalar, Vector4 v) => v - scalar;
        public static Vector4 operator -(Vector4 v1, Vector2 v2) => new Vector4(v1.X - v2.X, v1.Y - v2.Y, v1.Z, v1.W);
        public static Vector4 operator -(Vector4 v1, Vector3 v2) => new Vector4(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z, v1.W);
        public static Vector4 operator -(Vector4 v1, Vector4 v2) => new Vector4(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z, v1.W - v2.W);

        public static Vector4 operator *(Vector4 v, float scalar) => new Vector4(v.X * scalar, v.Y * scalar, v.Z * scalar, v.W * scalar);
        public static Vector4 operator *(float scalar, Vector4 v) => v * scalar;
        public static Vector4 operator *(Vector4 v1, Vector2 v2) => new Vector4(v1.X * v2.X, v1.Y * v2.Y, v1.Z, v1.W);
        public static Vector4 operator *(Vector4 v1, Vector3 v2) => new Vector4(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z, v1.W);
        public static Vector4 operator *(Vector4 v1, Vector4 v2) => new Vector4(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z, v1.W * v2.W);

        public static Vector4 operator /(Vector4 v, float scalar) => new Vector4(v.X / scalar, v.Y / scalar, v.Z / scalar, v.W / scalar);
        public static Vector4 operator /(float scalar, Vector4 v) => v / scalar;
        public static Vector4 operator /(Vector4 v1, Vector2 v2) => new Vector4(v1.X / v2.X, v1.Y / v2.Y, v1.Z, v1.W);
        public static Vector4 operator /(Vector4 v1, Vector3 v2) => new Vector4(v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z, v1.W);
        public static Vector4 operator /(Vector4 v1, Vector4 v2) => new Vector4(v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z, v1.W / v2.W);

        #endregion

        #endregion

        #region Function Overloads

        public override string ToString() => $"( {X}, {Y}, {Z}, {W} )";

        #endregion
    }
}
