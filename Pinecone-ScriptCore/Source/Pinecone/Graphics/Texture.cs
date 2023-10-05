namespace Pinecone
{
    public abstract class Texture : Asset
    {
        public abstract uint ID { get; }

        public abstract uint Width { get; }
        public abstract uint Height { get; }

        public static Type GetStaticType() => Type.Texture2D;
        public override Type GetAssetType() => GetStaticType();
    }
}
