#pragma once

#include "Pinecone/Renderer/Camera.h"
#include "Pinecone/Renderer/Texture2D.h"
#include "Pinecone/Renderer/Font.h"

#include "Pinecone/Scene/Components.h"

namespace Pinecone
{
	class Renderer2D
	{
	public:
		/// <summary>
		/// Initialize the 2D Renderer
		/// </summary>
		static void Init();
		/// <summary>
		/// Shutdown the 2D Renderer
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Begin rendering a 2D scene
		/// </summary>
		/// <param name="camera">The camera</param>
		static void BeginScene(const Camera& camera);
		/// <summary>
		/// Begin rendering a 2D scene with a camera transform modifier
		/// </summary>
		/// <param name="camera">The camera</param>
		/// <param name="transform">A transform to modify the camera view projection with</param>
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		/// <summary>
		/// End the scene
		/// </summary>
		static void EndScene();
		/// <summary>
		/// Draw the scene
		/// </summary>
		static void Flush();

		/// <summary>
		/// Draw a 2D quad with a given position, size, and color
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="color">The color of the quad</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		/// <summary>
		/// Draw a 2D quad with a given position, size, and color
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="color">The color of the quad</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		/// <summary>
		/// Draw a 2D quad with a given position, size, and texture
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="texture">The texture to render the quad with</param>
		/// <param name="tilingFactor">How should the texture be tiled (1 is to show the full texture without tiling)</param>
		/// <param name="tintColor">The tint color (leave as white to render the texture in full colors)</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		/// <summary>
		/// Draw a 2D quad with a given position, size, and texture
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="texture">The texture to render the quad with</param>
		/// <param name="tilingFactor">How should the texture be tiled (1 is to show the full texture without tiling)</param>
		/// <param name="tintColor">The tint color (leave as white to render the texture in full colors)</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a 2D quad with a given position, size, rotation, and color.
		/// Do note that rotation is in radians
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="rotation">How the quad should be rotated</param>
		/// <param name="color">The color of the quad</param>
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		/// <summary>
		/// Draw a 2D quad with a given position, size, rotation, and color.
		/// Do note that rotation is in radians
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="rotation">How the quad should be rotated</param>
		/// <param name="color">The color of the quad</param>
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		/// <summary>
		/// Draw a 2D quad with a given position, size, rotation, and texture
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="rotation">How the quad should be rotated</param>
		/// <param name="texture">The texture to render the quad with</param>
		/// <param name="tilingFactor">How should the texture be tiled (1 is to show the full texture without tiling)</param>
		/// <param name="tintColor">The tint color (leave as white to render the texture in full colors)</param>
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		/// <summary>
		/// Draw a 2D quad with a given position, size, rotation, and texture
		/// </summary>
		/// <param name="position">The position to draw the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="rotation">How the quad should be rotated</param>
		/// <param name="texture">The texture to render the quad with</param>
		/// <param name="tilingFactor">How should the texture be tiled (1 is to show the full texture without tiling)</param>
		/// <param name="tintColor">The tint color (leave as white to render the texture in full colors)</param>
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a 2D quad with a given transform matrix and a color
		/// </summary>
		/// <param name="transform">The quads transform</param>
		/// <param name="color">The color of the quad</param>
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		/// <summary>
		/// Draw a 2D quad with a given transform matrix and a texture
		/// </summary>
		/// <param name="transform">The quads transform</param>
		/// <param name="texture">The texture to render the quad with</param>
		/// <param name="tilingFactor">How should the texture be tiled (1 is to show the full texture without tiling)</param>
		/// <param name="tintColor">The tint color (leave as white to render the texture in full colors)</param>
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), const glm::vec2& flipAxies = { 0.0f, 0.0f });

		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color);
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

		/// <summary>
		/// Draw a 2D quad with a given transform and a sprite component to get the color and texture information from
		/// </summary>
		/// <param name="transform">The sprites transform</param>
		/// <param name="sprite">The sprite component</param>
		static void DrawSprite(const glm::mat4& transform, SpriteComponent& sprite);

		/// <summary>
		/// Draw a string with a given font, transform, and color
		/// </summary>
		/// <param name="string">The string to render to the screen</param>
		/// <param name="font">The specified font (use Font::GetDefault for the default font)</param>
		/// <param name="transform">The transform of the string</param>
		/// <param name="color">The color to render the string with</param>
		static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform, const glm::vec4& color);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		/// <summary>
		/// Reset the 2D renderer statistics
		/// </summary>
		static void ResetStats();
		/// <summary>
		/// Get the 2D renderer statistics
		/// </summary>
		/// <returns>2D renderer statistics</returns>
		static Statistics GetStats();
	private:
		/// <summary>
		/// Initialize the batch. Reset vertex buffer information and start a new draw
		/// </summary>
		static void StartBatch();
		/// <summary>
		/// Draw the current batch to the screen and start a new batch
		/// </summary>
		static void NextBatch();
	};
}