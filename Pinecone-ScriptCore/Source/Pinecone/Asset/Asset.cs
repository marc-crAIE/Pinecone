namespace Pinecone
{
    public abstract class Asset
    {
        public ulong Handle;

        public enum Type : ushort
        {
            None = 0,
            Scene,
            Texture2D
        }

        public abstract Type GetAssetType();
    }
}
