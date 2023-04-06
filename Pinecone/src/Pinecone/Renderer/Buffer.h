#pragma once

namespace Pinecone
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	/// <summary>
	/// Get the size of the data type in bytes
	/// </summary>
	/// <param name="type">The shader data type</param>
	/// <returns>The size of the shader data type in bytes</returns>
	static int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		PC_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		/// <summary>
		/// The BufferElement constructor
		/// </summary>
		BufferElement() = default;

		/// <summary>
		/// The BufferElement constructor that takes in a data type, name, and optionally if it is normalized
		/// </summary>
		/// <param name="type">The shader data type</param>
		/// <param name="name">The name of the buffer element</param>
		/// <param name="normalized">If it is normalized</param>
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}

		/// <summary>
		/// Get the number of "components" that make up the shader data type.
		/// For example a ShaderDataType::Float3 would return 3 as it is made
		/// up from 3 floats.
		/// </summary>
		/// <returns>The amount of components in the data type</returns>
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3 * float3
			case ShaderDataType::Mat4:    return 4; // 4 * float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			PC_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		/// <summary>
		/// The BufferLayout constructor
		/// </summary>
		BufferLayout() {}

		/// <summary>
		/// The BufferLayout constructor being defined from a list.
		/// Allows for this method of defining
		/// Example:
		/// 
		/// BufferLayout layout = {
		///		{ ShaderDataType::Float3, "position" },
		///		{ ShaderDataType::Float4, "color" }
		///	}
		/// </summary>
		/// <param name="elements"></param>
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			// Calculate the element offsets and the stride of the layout
			CalculateOffsetsAndStride();
		}

		/// <summary>
		/// Get the stride of the layout
		/// </summary>
		/// <returns>The layout stride</returns>
		uint32_t GetStride() const { return m_Stride; }
		/// <summary>
		/// Get the buffer elements that make up the layout
		/// </summary>
		/// <returns>The buffer elements</returns>
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		// Iterator functions for use with range-based for loops

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		/// <summary>
		/// Calculate the offsets for each element and the stride of the layout.
		/// The offset for each element is where the data for that buffer element starts
		/// in the vertex array. The layout stride is the total size of the buffer layout
		/// in bytes for each vertex (not all vertices combined).
		/// </summary>
		void CalculateOffsetsAndStride()
		{
			// Keep track of the element offset and reset the stride
			size_t offset = 0;
			m_Stride = 0;
			// Loop through each element
			for (auto& element : m_Elements)
			{
				// Set the element offset and increase the offset by the size of the element
				element.Offset = offset;
				// The next element will start at this new offset
				offset += element.Size;
				// The stride is also increased by the element size
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};
}