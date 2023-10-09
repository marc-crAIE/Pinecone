using System;

namespace Pinecone
{
    public struct Matrix3
    {
        private float[,] matrix;

        #region Properties

        // First row
        public Vector3 m0 { get => this[0]; set => this[0] = value; }
        public float m00 { get => matrix[0, 0]; set => matrix[0, 0] = value; }
        public float m01 { get => matrix[0, 1]; set => matrix[0, 1] = value; }
        public float m02 { get => matrix[0, 2]; set => matrix[0, 2] = value; }

        // Second row
        public Vector3 m1 { get => this[1]; set => this[1] = value; }
        public float m10 { get => matrix[1, 0]; set => matrix[1, 0] = value; }
        public float m11 { get => matrix[1, 1]; set => matrix[1, 1] = value; }
        public float m12 { get => matrix[1, 2]; set => matrix[1, 2] = value; }

        // Third row
        public Vector3 m2 { get => this[2]; set => this[2] = value; }
        public float m20 { get => matrix[2, 0]; set => matrix[2, 0] = value; }
        public float m21 { get => matrix[2, 1]; set => matrix[2, 1] = value; }
        public float m22 { get => matrix[2, 2]; set => matrix[2, 2] = value; }

        #endregion

        #region Constructors

        public Matrix3(float f)
        {
            this.matrix = new float[3, 3];
            this[0, 0] = f;
            this[1, 1] = f;
            this[2, 2] = f;
        }

        public Matrix3(float[,] m)
        {
            if (m.GetLength(0) != 3 || m.GetLength(1) != 3)
                throw new ArgumentException("Input data must be a 3x3 matrix!");

            this.matrix = m;
        }

        public Matrix3(Vector3 m0, Vector3 m1, Vector3 m2)
        {
            this.matrix = new float[3, 3];
            this[0] = m0;
            this[1] = m1;
            this[2] = m2;
        }

        public Matrix3(float m00, float m01, float m02,
                       float m10, float m11, float m12,
                       float m20, float m21, float m22)
        {
            this.matrix = new float[3, 3];
            this[0, 0] = m00; this[0, 1] = m01; this[0, 2] = m02;
            this[1, 0] = m10; this[1, 1] = m11; this[1, 2] = m12;
            this[2, 0] = m20; this[2, 1] = m21; this[2, 2] = m22;
        }

        #endregion

        #region General Functions

        public static Matrix3 Identity() => new Matrix3(1.0f);

        public float Determinant()
        {
            return m00 * m11 * m22 + m10 * m21 * m02 + m20 * m01 * m12
                - m02 * m11 * m20 - m12 * m21 * m00 - m22 * m01 * m10;
        }

        public Matrix3 Transpose()
        {
            Matrix3 result = new Matrix3(0.0f);
            result[0, 0] = this[0, 0];
            result[0, 1] = this[1, 0];
            result[0, 2] = this[2, 0];

            result[1, 0] = this[0, 1];
            result[1, 1] = this[1, 1];
            result[1, 2] = this[2, 1];

            result[2, 0] = this[0, 2];
            result[2, 1] = this[1, 2];
            result[2, 2] = this[2, 2];
            return result;
        }

        public Matrix3 Inverse()
        {
            float det = Determinant();
            if (det == 0)
                throw new InvalidOperationException("Matrix is not invertible.");

            Matrix3 result = new Matrix3(0.0f);

            result[0, 0] = (m11 * m22 - m12 * m21);
            result[0, 1] = (m02 * m21 - m01 * m22);
            result[0, 2] = (m01 * m12 - m02 * m11);

            result[1, 0] = (m12 * m20 - m10 * m22);
            result[1, 1] = (m00 * m22 - m02 * m20);
            result[1, 2] = (m02 * m10 - m00 * m12);

            result[2, 0] = (m10 * m21 - m11 * m20);
            result[2, 1] = (m01 * m20 - m00 * m21);
            result[2, 2] = (m00 * m11 - m01 * m10);

            return result / det;
        }

        #endregion

        #region Transformation Functions

        public void SetTranslation(Vector2 v) => this[2] = new Vector3(v, this[2].Z);
        public void SetTranslation(float x, float y) => SetTranslation(new Vector2(x, y));

        public void SetRotation(float angle, Vector3 axis) => this = Rotation(this, angle, axis);
        public void SetRotation(float angle, float x, float y, float z) => SetRotation(angle, new Vector3(x, y, z));
        public void SetRotationX(float angle) => SetRotation(angle, new Vector3(1.0f, 0.0f, 0.0f));
        public void SetRotationY(float angle) => SetRotation(angle, new Vector3(0.0f, 1.0f, 0.0f));
        public void SetRotationZ(float angle) => SetRotation(angle, new Vector3(0.0f, 0.0f, 1.0f));

        public void SetScale(Vector2 v) => this *= Scale(v);
        public void SetScale(float x, float y) => SetScale(new Vector2(x, y));

        #region Create Transform Matrices

        public static Matrix3 Translation(Vector2 v)
        {
            Matrix3 translation = new Matrix3();
            translation[2, 0] = v.X;
            translation[2, 1] = v.Y;
            return translation;
        }

        public static Matrix3 Rotation(Matrix3 m, float angle, Vector3 axis)
        {
            Matrix3 rotation = m;

            axis.Normalize();

            float sin = MathF.Sin(angle);
            float cos = MathF.Cos(angle);

            Vector3 omc = (1.0f - cos) * axis;

            float x = axis.X;
            float y = axis.Y;
            float z = axis.Z;

            rotation[0, 0] = cos + omc.X * x;
            rotation[1, 0] = omc.Y * x - sin * z;
            rotation[2, 0] = omc.Z * x + sin * y;

            rotation[0, 1] = omc.X * y + sin * z;
            rotation[1, 1] = cos + omc.Y * y;
            rotation[2, 1] = omc.Z * y - sin * x;

            rotation[0, 2] = omc.X * z - sin * y;
            rotation[1, 2] = omc.Y * z + sin * x;
            rotation[2, 2] = cos + omc.Z * z;

            return rotation;
        }
        public static Matrix3 Rotation(float angle, Vector3 axis) => Rotation(new Matrix3(1.0f), angle, axis);
        public static Matrix3 Rotation(float angle, float x, float y, float z) => Rotation(angle, new Vector3(x, y, z));

        public static Matrix3 RotationX(float angle) => Rotation(angle, new Vector3(1.0f, 0.0f, 0.0f));
        public static Matrix3 RotationY(float angle) => Rotation(angle, new Vector3(0.0f, 1.0f, 0.0f));
        public static Matrix3 RotationZ(float angle) => Rotation(angle, new Vector3(0.0f, 0.0f, 1.0f));

        public static Matrix3 Scale(Vector2 v)
        {
            Matrix3 scale = new Matrix3();
            scale[0, 0] = v.X;
            scale[1, 1] = v.Y;
            return scale;
        }
        public static Matrix3 Scale(float x, float y) => Scale(new Vector2(x, y));

        #endregion

        #endregion

        #region Operators

        #region Binary Operators

        public static Matrix3 operator +(Matrix3 m, float scalar)
        {
            Matrix3 result = new Matrix3();
            result[0] = m[0] + scalar;
            result[1] = m[1] + scalar;
            result[2] = m[2] + scalar;
            return result;
        }
        public static Matrix3 operator +(float scalar, Matrix3 m) => m + scalar;

        public static Matrix3 operator +(Matrix3 a, Matrix3 b)
        {
            Matrix3 result = new Matrix3(0.0f);
            result[0] = a[0] + b[0];
            result[1] = a[1] + b[1];
            result[2] = a[2] + b[2];
            return result;
        }

        public static Matrix3 operator -(Matrix3 m, float scalar)
        {
            Matrix3 result = new Matrix3();
            result[0] = m[0] - scalar;
            result[1] = m[1] - scalar;
            result[2] = m[2] - scalar;
            return result;
        }
        public static Matrix3 operator -(float scalar, Matrix3 m) => m - scalar;

        public static Matrix3 operator -(Matrix3 a, Matrix3 b)
        {
            Matrix3 result = new Matrix3(0.0f);
            result[0] = a[0] - b[0];
            result[1] = a[1] - b[1];
            result[2] = a[2] - b[2];
            result[3] = a[3] - b[3];
            return result;
        }

        public static Matrix3 operator *(Matrix3 m, float scalar)
        {
            Matrix3 result = new Matrix3();
            result[0] = m[0] * scalar;
            result[1] = m[1] * scalar;
            result[2] = m[2] * scalar;
            return result;
        }
        public static Matrix3 operator *(float scalar, Matrix3 m) => m * scalar;
        public static Matrix3 operator *(Matrix3 a, Matrix3 b)
        {
            Matrix3 result = new Matrix3(0.0f);
            result[0] = a[0] * b[0, 0] + a[1] * b[0, 1] + a[2] * b[0, 2];
            result[1] = a[0] * b[1, 0] + a[1] * b[1, 1] + a[2] * b[1, 2];
            result[2] = a[0] * b[2, 0] + a[1] * b[2, 1] + a[2] * b[2, 2];
            return result;
        }

        public static Matrix3 operator /(Matrix3 m, float scalar)
        {
            Matrix3 result = new Matrix3();
            result[0] = m[0] / scalar;
            result[1] = m[1] / scalar;
            result[2] = m[2] / scalar;
            return result;
        }
        public static Matrix3 operator /(float scalar, Matrix3 m) => scalar * m.Inverse();
        public static Matrix3 operator /(Matrix3 a, Matrix3 b) => a * b.Inverse();

        #endregion

        #region Comparison Operators

        public static bool operator ==(Matrix3 a, Matrix3 b) => (a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]);
        public static bool operator !=(Matrix3 a, Matrix3 b) => !(a == b);

        #endregion

        #region Unary Operators

        public static Matrix3 operator +(Matrix3 m)
        {
            return m;
        }

        public static Matrix3 operator -(Matrix3 m)
        {
            Matrix3 result = new Matrix3();
            result[0] = -m[0];
            result[1] = -m[1];
            result[2] = -m[2];
            return result;
        }

        public static Matrix3 operator ++(Matrix3 m)
        {
            Matrix3 result = new Matrix3();
            result[0] = m[0]++;
            result[1] = m[1]++;
            result[2] = m[2]++;
            return result;
        }

        public static Matrix3 operator --(Matrix3 m)
        {
            Matrix3 result = new Matrix3();
            result[0] = m[0]--;
            result[1] = m[1]--;
            result[2] = m[2]--;
            return result;
        }

        #endregion

        #endregion

        #region Accessors

        public Vector3 this[int row]
        {
            get
            {
                return new Vector3(matrix[row, 0], matrix[row, 1], matrix[row, 2]);
            }
            set
            {
                matrix[row, 0] = value.X;
                matrix[row, 1] = value.Y;
                matrix[row, 2] = value.Z;
            }
        }

        public float this[int row, int col]
        {
            get { return matrix[row, col]; }
            set { matrix[row, col] = value; }
        }

        #endregion

        #region Type Conversion

        public static implicit operator Matrix3(Matrix4 m)
        {
            return new Matrix3(m[0], m[1], m[2]);
        }

        public static implicit operator Matrix3(Quaternion q)
        {
            Matrix3 result = new Matrix3();

            result[0, 0] = 1 - 2 * ((q.Y * q.Y) + (q.Z * q.Z));
            result[0, 1] = 2 * ((q.X * q.Y) + (q.W * q.Z));
            result[0, 2] = 2 * ((q.X * q.Z - (q.W * q.Y)));

            result[1, 0] = 2 * ((q.X * q.Y) - (q.W * q.Z));
            result[1, 1] = 1 - 2 * ((q.X * q.X) + (q.Z * q.Z));
            result[1, 2] = 2 * ((q.Y * q.Z) + (q.W * q.X));

            result[2, 0] = 2 * ((q.X * q.Z) + (q.W * q.Y));
            result[2, 1] = 2 * ((q.Y * q.Z) - (q.W * q.X));
            result[2, 2] = 1 - 2 * ((q.X * q.X) + (q.Y * q.Y));

            return result;
        }

        #endregion

        #region Function Overloads

        public override string ToString()
        {
            string result = "";
            for (int row = 0; row < 3; row++)
            {
                result += "[ ";
                for (int col = 0; col < 3; col++)
                {
                    result += this[row, col].ToString("F2");
                    if (col < 2)
                    {
                        result += ", ";
                    }
                }
                result += " ]";
                if (row < 2)
                {
                    result += '\n';
                }
            }
            return result;
        }

        #endregion
    }
}
