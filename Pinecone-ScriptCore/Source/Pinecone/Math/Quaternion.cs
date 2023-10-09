using System;

namespace Pinecone
{

    public struct Quaternion
    {
        public float X, Y, Z, W;

        public static Quaternion Identity => new Quaternion(1, 0, 0, 0);

        #region Constructors

        public Quaternion(float f) : this(f, f, f, f) { }

        public Quaternion(Vector3 eulerAngles)
        {
            this = Euler(eulerAngles);
        }

        public Quaternion(Vector4 v) : this(v.W, v.X, v.Y, v.Z) { }

        public Quaternion(float w, float x, float y, float z)
        {
            this.W = w;
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        #endregion

        #region General Functions

        public float Dot(Quaternion v)
        {
            return this.W * v.W + this.X * v.X + this.Y * v.Y + this.Z * v.Z;
        }

        public float Magnitude()
        {
            return (float)Math.Sqrt(W * W + X * X + Y * Y + Z * Z);
        }

        public void Normalize()
        {
            float mag = Magnitude();

            if (mag == 0.0f)
                return;

            this /= mag;
        }

        public Quaternion Conjugate()
        {
            return new Quaternion(W, -X, -Y, -Z);
        }

        public Quaternion Inverse()
        {
            return Conjugate() / Dot(this);
        }

        public static Quaternion Euler(Vector3 v)
        {
            Vector3 c = MathF.Cos(v / 2);
            Vector3 s = MathF.Sin(v / 2);

            Quaternion quat = new Quaternion();
            quat.W = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
            quat.X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
            quat.Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
            quat.Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
            return quat;
        }

        public static Quaternion FromAxisAngle(Vector3 axis, float angle)
        {
            axis.Normalize();

            float sin = (float)Math.Sin(angle / 2);
            float cos = (float)Math.Cos(angle / 2);
            return new Quaternion(cos, axis.X * sin, axis.Y * sin, axis.Z * sin);
        }

        #endregion

        #region Operators

        #region Binary Operators

        public static Quaternion operator +(Quaternion a, Quaternion b) => new Quaternion(a.W + b.W, a.X + b.X, a.Y + b.Y, a.Z + b.Z);

        public static Quaternion operator -(Quaternion a, Quaternion b) => new Quaternion(a.W - b.W, a.X - b.X, a.Y - b.Y, a.Z - b.Z);

        public static Quaternion operator *(Quaternion a, float scalar) => new Quaternion(a.W * scalar, a.X * scalar, a.Y * scalar, a.Z * scalar);
        public static Quaternion operator *(Quaternion a, Quaternion b)
        {
            return new Quaternion(
                a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z,
                a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y,
                a.W * b.Y - a.X * b.Z + a.Y * b.W + a.Z * b.X,
                a.W * b.Z + a.X * b.Y - a.Y * b.X + a.Z * b.W
            );
        }

        public static Quaternion operator /(Quaternion q, float scalar) => new Quaternion(q.W / scalar, q.X / scalar, q.Y / scalar, q.Z / scalar);

        #endregion

        #endregion

        #region Accessors

        public float this[int i]
        {
            get
            {
                switch (i)
                {
                    case 0: return W;
                    case 1: return X;
                    case 2: return Y;
                    case 3: return Z;
                }
                return 0;
            }
            set
            {
                switch (i)
                {
                    case 0: W = value; break;
                    case 1: X = value; break;
                    case 2: Y = value; break;
                    case 3: Z = value; break;
                }
            }
        }

        #endregion

        #region Type Conversions

        public static implicit operator Vector4(Quaternion q) => new Vector4(q.X, q.Y, q.Z, q.W);

        #endregion

        #region Function Overloads

        public override string ToString()
        {
            return $"( {W} {{ {X}, {Y}, {Z} }} )";
        }

        #endregion
    }
}
