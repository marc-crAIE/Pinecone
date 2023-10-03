using System;
using System.IO;

namespace Pinecone
{
    public struct Vector3
    {
        public const float kEpsilon = 0.00001F;
        public const float kEpsilonNormalSqrt = 1e-15f;


        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);
        public static Vector3 One => new Vector3(1.0f);
        public static Vector3 NegativeInfinity => new Vector3(float.NegativeInfinity);
        public static Vector3 PositiveInfinity => new Vector3(float.PositiveInfinity);

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
        public float SqrMagnitude => X * X + Y * Y + Z * Z;

        public Vector3 Normalized
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
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector3 index!");
                }
            }
            set
            {
                switch (i)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector3 index!");
                }
            }
        }

        #endregion

        #region Methods

        public void Normalize()
        {
            this = Normalized;
        }

        public void Scale(Vector3 scale)
        {
            X *= scale.X;
            Y *= scale.Y;
            Z *= scale.Z;
        }

        #endregion

        #region Static Methods

        public static Vector3 Normalize(Vector3 value)
        {
            float mag = value.Magnitude;
            if (mag > kEpsilon)
                return value / mag;
            else 
                return Zero;
        }

        public static Vector3 Scale(Vector3 v1, Vector3 v2) => new Vector3(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z);

        public static Vector3 Cross(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(
                lhs.Y * rhs.Z - lhs.Z * rhs.Y,
                lhs.Z * rhs.X - lhs.X * rhs.Z,
                lhs.X * rhs.Y - lhs.Y * rhs.X
            );
        }

        public static float Dot(Vector3 lhs, Vector3 rhs) => lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;

        public static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal)
        {
            float factor = -2F * Dot(inNormal, inDirection);
            return new Vector3(factor * inNormal.X + inDirection.X,
                factor * inNormal.Y + inDirection.Y,
                factor * inNormal.Z + inDirection.Z);
        }

        public static Vector3 Project(Vector3 vector, Vector3 onNormal)
        {
            float sqrMag = Dot(onNormal, onNormal);
            if (sqrMag < MathF.Epsilon)
                return Zero;
            else
            {
                var dot = Dot(vector, onNormal);
                return new Vector3(
                    onNormal.X * dot / sqrMag,
                    onNormal.Y * dot / sqrMag,
                    onNormal.Z * dot / sqrMag
                );
            }
        }
        public static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
        {
            float sqrMag = Dot(planeNormal, planeNormal);
            if (sqrMag < MathF.Epsilon)
                return vector;
            else
            {
                var dot = Dot(vector, planeNormal);
                return new Vector3(
                    vector.X - planeNormal.X * dot / sqrMag,
                    vector.Y - planeNormal.Y * dot / sqrMag,
                    vector.Z - planeNormal.Z * dot / sqrMag
                );
            }
        }

        public static float Angle(Vector3 from, Vector3 to)
        {
            // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
            float denominator = MathF.Sqrt(from.SqrMagnitude * to.SqrMagnitude);
            if (denominator < kEpsilonNormalSqrt)
                return 0F;

            float dot = MathF.Clamp(Dot(from, to) / denominator, -1F, 1F);
            return (MathF.Acos(dot)) * MathF.Rad2Deg;
        }

        public static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis)
        {
            float unsignedAngle = Angle(from, to);

            float crossX = from.Y * to.Z - from.Z * to.Y;
            float crossY = from.Z * to.X - from.X * to.Z;
            float crossZ = from.X * to.Y - from.Y * to.X;
            float sign = MathF.Sign(axis.X * crossX + axis.Y * crossY + axis.Z * crossZ);
            return unsignedAngle * sign;
        }
        public static float Distance(Vector3 a, Vector3 b)
        {
            float diffX = a.X - b.X;
            float diffY = a.Y - b.Y;
            float diffZ = a.Z - b.Z;
            return (float)Math.Sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
        }
        public static Vector3 ClampMagnitude(Vector3 vector, float maxLength)
        {
            float sqrmag = vector.SqrMagnitude;
            if (sqrmag > maxLength * maxLength)
            {
                float mag = (float)Math.Sqrt(sqrmag);

                float normalizedX = vector.X / mag;
                float normalizedY = vector.Y / mag;
                float normalizedZ = vector.Z / mag;
                return new Vector3(
                    normalizedX * maxLength,
                    normalizedY * maxLength,
                    normalizedZ * maxLength
                );
            }
            return vector;
        }

        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            t = MathF.Clamp01(t);
            return new Vector3(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t
            );
        }

        public static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t)
        {
            return new Vector3(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t
            );
        }

        public static Vector3 Min(Vector3 lhs, Vector3 rhs) => new Vector3(MathF.Min(lhs.X, rhs.X), MathF.Min(lhs.Y, rhs.Y), MathF.Min(lhs.Z, rhs.Z));
        public static Vector3 Max(Vector3 lhs, Vector3 rhs) => new Vector3(MathF.Max(lhs.X, rhs.X), MathF.Max(lhs.Y, rhs.Y), MathF.Max(lhs.Z, rhs.Z));

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

        #region Comparison Operators

        public static bool operator ==(Vector3 v1, Vector3 v2) => 
            MathF.Approximately(v1.X, v2.X) && 
            MathF.Approximately(v1.Y, v2.Y) && 
            MathF.Approximately(v1.Z, v2.Z);
        public static bool operator !=(Vector3 v1, Vector3 v2) => !(v1 == v2);

        #endregion

        #endregion

        #region Function Overloads

        public override string ToString() => $"( {X}, {Y}, {Z} )";

        public override int GetHashCode() => X.GetHashCode() ^ (Y.GetHashCode() << 2) ^ (Z.GetHashCode() >> 2);

        public override bool Equals(object other)
        {
            if (!(other is Vector3)) return false;
            return Equals((Vector3)other);
        }

        public bool Equals(Vector3 other) => this == other;

        #endregion
    }
}
