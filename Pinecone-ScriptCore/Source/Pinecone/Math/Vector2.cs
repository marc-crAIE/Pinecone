using System;
using System.IO;

namespace Pinecone
{
    public struct Vector2
    {
        public const float kEpsilon = 0.00001F;
        public const float kEpsilonNormalSqrt = 1e-15f;


        public float X, Y;

        public static Vector2 Zero => new Vector2(0.0f);
        public static Vector2 One => new Vector2(1.0f);
        public static Vector2 NegativeInfinity => new Vector2(float.NegativeInfinity);
        public static Vector2 PositiveInfinity => new Vector2(float.PositiveInfinity);

        public static Vector2 Up => new Vector2(0.0f, 1.0f);
        public static Vector2 Down => new Vector2(0.0f, -1.0f);
        public static Vector2 Left => new Vector2(-1.0f, 0.0f);
        public static Vector2 Right => new Vector2(1.0f, 0.0f);

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
        public float SqrMagnitude => X * X + Y * Y;

        public Vector2 Normalized
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
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index!");
                }
            }
            set
            {
                switch (i)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index!");
                }
            }
        }

        #endregion

        #region Methods

        public void Normalize()
        {
            this = Normalized;
        }

        public void Scale(Vector2 scale)
        {
            X *= scale.X;
            Y *= scale.Y;
        }

        #endregion

        #region Static Methods

        public static Vector2 Scale(Vector2 v1, Vector2 v2) => new Vector2(v1.X * v2.X, v1.Y * v2.Y);

        public static float Dot(Vector2 v1, Vector2 v2) => v1.X * v2.X + v1.Y * v2.Y;

        public static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal)
        {
            float factor = -2F * Dot(inNormal, inDirection);
            return new Vector2(factor * inNormal.X + inDirection.X, factor * inNormal.Y + inDirection.Y);
        }

        public static Vector2 Perpendicular(Vector2 inDirection) => new Vector2(-inDirection.Y, inDirection.X);

        public static float Angle(Vector2 from, Vector2 to)
        {
            float denominator = (float)Math.Sqrt(from.SqrMagnitude * to.SqrMagnitude);
            if (denominator < kEpsilonNormalSqrt)
                return 0F;

            float dot = MathF.Clamp(Dot(from, to) / denominator, -1F, 1F);
            return MathF.Acos(dot) * MathF.Rad2Deg;
        }

        public static float SignedAngle(Vector2 from, Vector2 to)
        {
            float unsignedAngle = Angle(from, to);
            float sign = MathF.Sign(from.X * to.Y - from.Y * to.X);
            return unsignedAngle * sign;
        }

        public static float Distance(Vector2 a, Vector2 b)
        {
            float diffX = a.X - b.X;
            float diffY = a.Y - b.Y;
            return MathF.Sqrt(diffX * diffX + diffY * diffY);
        }

        public static Vector2 ClampMagnitude(Vector2 vector, float maxLength)
        {
            float sqrMagnitude = vector.SqrMagnitude;
            if (sqrMagnitude > maxLength * maxLength)
            {
                float mag = MathF.Sqrt(sqrMagnitude);

                float normalizedX = vector.X / mag;
                float normalizedY = vector.Y / mag;
                return new Vector2(
                    normalizedX * maxLength,
                    normalizedY * maxLength
                );
            }
            return vector;
        }

        public static Vector2 Lerp(Vector2 v1, Vector2 v2, float t)
        {
            t = MathF.Clamp01(t);
            return new Vector2(
                v1.X + (v2.X - v1.X) * t,
                v1.Y + (v2.Y - v1.Y) * t
            );
        }

        public static Vector2 LerpUnclamped(Vector2 v1, Vector2 v2, float t)
        {
            return new Vector2(
                v1.X + (v2.X - v1.X) * t,
                v1.Y + (v2.Y - v1.Y) * t
            );
        }

        public static Vector2 Min(Vector2 lhs, Vector2 rhs) { return new Vector2(MathF.Min(lhs.X, rhs.X), MathF.Min(lhs.Y, rhs.Y)); }
        public static Vector2 Max(Vector2 lhs, Vector2 rhs) { return new Vector2(MathF.Max(lhs.X, rhs.X), MathF.Max(lhs.Y, rhs.Y)); }

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

        #region Comparison Operators

        public static bool operator ==(Vector2 v1, Vector2 v2) => MathF.Approximately(v1.X, v2.X) && MathF.Approximately(v1.Y, v2.Y);
        public static bool operator !=(Vector2 v1, Vector2 v2) => !(v1 == v2);

        #endregion

        #endregion

        #region Function Overloads

        public override string ToString() => $"( {X}, {Y} )";

        public override int GetHashCode() => X.GetHashCode() ^ (Y.GetHashCode() << 2);

        public override bool Equals(object other)
        {
            if (!(other is Vector2)) return false;

            return Equals((Vector2)other);
        }

        public bool Equals(Vector2 other) => this == other;

        #endregion
    }
}
