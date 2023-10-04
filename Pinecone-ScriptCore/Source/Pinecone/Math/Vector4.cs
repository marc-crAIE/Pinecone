using System;
using System.IO;

namespace Pinecone
{
    public struct Vector4
    {
        public const float kEpsilon = 0.00001F;
        public const float kEpsilonNormalSqrt = 1e-15f;


        public float X, Y, Z, W;

        public static Vector4 Zero => new Vector4(0.0f);
        public static Vector4 One => new Vector4(1.0f);
        public static Vector4 NegativeInfinity => new Vector4(float.NegativeInfinity);
        public static Vector4 PositiveInfinity => new Vector4(float.PositiveInfinity);

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
        public float SqrMagnitude => X * X + Y * Y + Z * Z + W * W;

        public Vector4 Normalized
        {
            get
            {
                float mag = Magnitude;
                if (mag > kEpsilon)
                    return this / mag;
                else
                    return Zero;
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
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector4 index!");
                }
            }
            set
            {
                switch (i)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    case 3: W = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector4 index!");
                }
            }
        }

        #endregion

        #region Methods

        public void Normalize()
        {
            this = Normalized;
        }

        public void Scale(Vector4 scale)
        {
            X *= scale.X; 
            Y *= scale.Y;
            Z *= scale.Z;
            W *= scale.W;
        }

        #endregion

        #region Static Methods

        public static Vector4 Normalize(Vector4 value)
        {
            float mag = value.Magnitude;
            if (mag > kEpsilon)
                return value / mag;
            else
                return Zero;
        }

        public static Vector4 Scale(Vector4 v1, Vector4 v2) => new Vector4(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z, v1.W * v2.W);

        public static float Dot(Vector4 a, Vector4 b) => a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;

        public static float Distance(Vector4 a, Vector4 b) => (a - b).Magnitude;

        public static Vector4 Project(Vector4 a, Vector4 b) { return b * (Dot(a, b) / Dot(b, b)); }

        public static Vector4 Lerp(Vector4 a, Vector4 b, float t)
        {
            t = MathF.Clamp01(t);
            return new Vector4(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t,
                a.W + (b.W - a.W) * t
            );
        }

        public static Vector4 LerpUnclamped(Vector4 a, Vector4 b, float t)
        {
            return new Vector4(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t,
                a.W + (b.W - a.W) * t
            );
        }

        public static Vector4 MoveTowards(Vector4 current, Vector4 target, float maxDistanceDelta)
        {
            float toVectorX = target.X - current.X;
            float toVectorY = target.Y - current.Y;
            float toVectorZ = target.Z - current.Z;
            float toVectorW = target.W - current.W;

            float sqdist = (toVectorX * toVectorX +
                toVectorY * toVectorY +
                toVectorZ * toVectorZ +
                toVectorW * toVectorW);

            if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
                return target;

            var dist = MathF.Sqrt(sqdist);

            return new Vector4(
                current.X + toVectorX / dist * maxDistanceDelta,
                current.Y + toVectorY / dist * maxDistanceDelta,
                current.Z + toVectorZ / dist * maxDistanceDelta,
                current.W + toVectorW / dist * maxDistanceDelta
            );
        }

        public static Vector4 Min(Vector4 lhs, Vector4 rhs) =>
            new Vector4(MathF.Min(lhs.X, rhs.X), MathF.Min(lhs.Y, rhs.Y), MathF.Min(lhs.Z, rhs.Z), MathF.Min(lhs.W, rhs.W));
        public static Vector4 Max(Vector4 lhs, Vector4 rhs) =>
            new Vector4(MathF.Max(lhs.X, rhs.X), MathF.Max(lhs.Y, rhs.Y), MathF.Max(lhs.Z, rhs.Z), MathF.Max(lhs.W, rhs.W));

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

        #region Comparison Operators

        public static bool operator ==(Vector4 v1, Vector4 v2) =>
            MathF.Approximately(v1.X, v2.X) &&
            MathF.Approximately(v1.Y, v2.Y) &&
            MathF.Approximately(v1.Z, v2.Z) &&
            MathF.Approximately(v1.W, v2.W);
        public static bool operator !=(Vector4 v1, Vector4 v2) => !(v1 == v2);

        #endregion

        #endregion

        #region Type Conversion

        public static implicit operator Vector4(Vector2 v) => new Vector4(v, 0.0f);
        public static implicit operator Vector4(Vector3 v) => new Vector4(v, 0.0f);

        #endregion

        #region Function Overloads

        public override string ToString() => $"( {X}, {Y}, {Z}, {W} )";

        public override int GetHashCode() => X.GetHashCode() ^ (Y.GetHashCode() << 2) ^ (Z.GetHashCode() >> 2) ^ (W.GetHashCode() >> 1);
        public override bool Equals(object other)
        {
            if (!(other is Vector4)) return false;
            return Equals((Vector4)other);
        }

        public bool Equals(Vector4 other) => this == other;

        #endregion
    }
}
