using System;

namespace Pinecone
{
    public class Texture2D : Texture
    {
        #region Constructors

        internal Texture2D(ulong handle)
        {
            Handle = handle;
        }

        public Texture2D(string path)
        {
            Handle = InternalCalls.Texture2D_New(path);
        }

        #endregion

        #region Properties

        public override uint ID 
        { 
            get => InternalCalls.Texture2D_GetRendererID(Handle);
        }

        public override uint Width 
        { 
            get => InternalCalls.Texture2D_GetWidth(Handle);
        }
        public override uint Height 
        { 
            get => InternalCalls.Texture2D_GetHeight(Handle);
        }

        #endregion
    }
}
