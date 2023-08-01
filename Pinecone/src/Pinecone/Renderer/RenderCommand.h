#pragma once

#include "Pinecone/Renderer/VertexArray.h"

namespace Pinecone
{
	class RenderCommand
	{
	public:
		/// <summary>
		/// Initialize OpenGL renderer settings
		/// </summary>
		static void Init();

		/// <summary>
		/// Set the OpenGL viewport. Recommended values for the x and y are 0.
		/// All values are in pixels.
		/// </summary>
		/// <param name="x">The lower left corner x coordinate</param>
		/// <param name="y">The lower left corner y coordinate</param>
		/// <param name="width">The width of the viewport</param>
		/// <param name="height">The height of the viewport</param>
		static void SetViewport(int x, int y, int width, int height);
		/// <summary>
		/// Set the color which we should use when the buffer is cleared
		/// </summary>
		/// <param name="color">The color to use when clearing the buffers</param>
		static void SetClearColor(const glm::vec4& color);
		/// <summary>
		/// Clear the color buffers. Use SetClearColor to specify what color the buffers should 
		/// be cleared with.
		/// </summary>
		static void Clear();

		/// <summary>
		/// Draw our vertex array data.
		/// </summary>
		/// <param name="vertexArray">The vertex data</param>
		/// <param name="indexCount">The number of indices (set to 0 or leave as default to use 
		/// the number in the vertex array)</param>
		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);

		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);

		static void SetLineWidth(float width);
	};
}