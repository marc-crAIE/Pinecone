#include "pcpch.h"
#include "Renderer2D.h"

#include "Pinecone/Renderer/VertexArray.h"
#include "Pinecone/Renderer/Shader.h"
#include "Pinecone/Renderer/RenderCommand.h"
#include "Pinecone/Renderer/UniformBuffer.h"
#include "Pinecone/Renderer/MSDFData.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pinecone
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 1.0f;

		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;
		Ref<Shader> TextShader;

		uint32_t TextIndexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		Ref<Texture2D> FontAtlasTexture;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// Quads
		s_Data.QuadVertexArray = VertexArray::Create();

		// Create our vertex buffer and set the layout to be the same as the Renderer2D_Quad shader
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" }
		});
		// Add the vertex buffer to the vertex array
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		// Create a new quad vertex buffer data array
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		// Setup the quad indices for all quads. 
		// Note: For every quad there are 4 vertices and 6 indices
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			// The indices point to a certain vertex. Two of the vertices are used twice 
			// to draw a quad so that we don't have to store 6 vertices for each quad with
			// two vertices. That is not necessary as two of the vertices share the exact 
			// same position
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			// Make sure to offset the number of vertices to point to. As here are 4 vertices 
			// for each quad, we increase the offset by 4
			offset += 4;
		}

		// Create the index buffer using the index data we just made and add it to the vertex array
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		// The basic vertex positions for a quad. This is modified by the transform matrix when the quad is drawn
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Text
		s_Data.TextVertexArray = VertexArray::Create();

		// Create our vertex buffer and set the layout to be the same as the Renderer2D_Text shader
		s_Data.TextVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex));
		s_Data.TextVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     }
			});
		// Add the vertex buffer to the vertex array
		s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
		// This also uses the same indices that quads do so also add that to the vertex array
		s_Data.TextVertexArray->SetIndexBuffer(quadIB);
		// Create a new text vertex buffer array
		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];
		
		// Create a basic white texture. This is used for drawing 2D quads with only a color and no texture
		s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
		// As the default texture specification size is 1x1. We only need to set 1 pixel as a white color
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create the shaders
		// Note: These shader files currently do need to exist in the client application
		s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
		s_Data.LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");
		s_Data.TextShader = Shader::Create("assets/shaders/Renderer2D_Text.glsl");

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Create a uniform buffer to pass our camera data into
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		// Delete our vertex buffer data arrays
		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.TextVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		// Set the camera buffer view projection from the camera
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection();
		// Then set the data in the uniform buffer, this will set the view projection in our shaders
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraBuffer));

		// Start the first batch
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		// Set the camera buffer view projection from the camera
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		// Then set the data in the uniform buffer, this will set the view projection in our shaders
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraBuffer));

		// Start the first batch
		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		// Similar to Renderer::EndScene.
		// End scene currently does nothing but this is for future proofing.
		// The use of it is recommended as it may cause problems if the 
		// engine is updated to give it use but the program does not currently 
		// use it. All it really does is just call flush but may do more in tne future
		// that flush should not do
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		// Initialize the 2D renderer data
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextIndexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		// Draw our quads
		if (s_Data.QuadIndexCount)
		{
			// Get the size of our vertex data we added and set that data in the vertex buffer
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			// Bind the quad shader and draw our vertex data
			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			// Every time we draw something with the render command, we should increase our total drawcalls
			s_Data.Stats.DrawCalls++;
		}

		// Draw lines
		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
		
		// Draw text
		if (s_Data.TextIndexCount)
		{
			// Get the size of our vertex data we added and set that data in the vertex buffer
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
			s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);

			// Bind the font atlas texture to texture slot 0
			s_Data.FontAtlasTexture->Bind(0);

			// Bind the text shader and draw our vertex data
			s_Data.TextShader->Bind();
			RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);
			// Every time we draw something with the render command, we should increase our total drawcalls
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		const size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		// If the number of indices has surpassed the max number of indices. Then we start the next batch
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		// Set the vertex data for every vertex in a quad
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		// Increment the index count by the number of indices needed for a quad
		s_Data.QuadIndexCount += 6;

		// Update the number of quads in the statistics
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, const glm::vec2& flipAxies)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		glm::vec2 textCoordFlip = { flipAxies.x ? -1.0f : 1.0f, flipAxies.y ? -1.0f : 1.0f };

		// If the number of indices has surpassed the max number of indices. Then we start the next batch
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		// Find what texture slot the texture we want to render is at
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		// Check if we could not find the texture
		// Note: Texture slot 0 is not used in this function as that is our white texture
		//		 for drawing plain colored quads
		if (textureIndex == 0.0f)
		{
			// Start a new batch if we exceed our max number of texture slots
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			// Set the texture in the texture slot at the current index
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			// Increase the current texture slot to be the next slot
			s_Data.TextureSlotIndex++;
		}

		// Set the vertex data for every vertex in a quad
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i] * textCoordFlip;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		// Increment the index count by the number of indices needed for a quad
		s_Data.QuadIndexCount += 6;

		// Update the number of quads in the statistics
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color)
	{
		DrawLine(glm::vec3(p0, 0.0f), glm::vec3(p1, 0.0f), color);
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteComponent& sprite)
	{
		// If the sprites texture is not null, draw a textured quad. Otherwise draw a colored quad
		if (sprite.Texture)
			DrawQuad(transform, sprite.Texture, sprite.TilingFactor, sprite.Color, sprite.FlipAxies);
		else
			DrawQuad(transform, sprite.Color);
	}

	void Renderer2D::DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform, const glm::vec4& color)
	{
		const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Ref<Texture2D> fontAtlas = font->GetAtlasTexture();

		// Set the font atlas texture to the texture created by the specified font
		s_Data.FontAtlasTexture = fontAtlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;
		float lineHeightOffset = 0.0f;

		// Loop through each character in the string
		for (size_t i = 0; i < string.size(); i++)
		{
			char character = string[i];
			// If it is a return character, skip
			if (character == '\r')
				continue;

			if (character == '\n')
			{
				// Newline characters will reset the x position to 0 and increase the y position
				// by the height of the text
				x = 0;
				y -= fsScale * metrics.lineHeight + lineHeightOffset;
				continue;
			}
			// Get the glyph data from our font
			auto glyph = fontGeometry.getGlyph(character);
			// If it is unknown, use a different char that should be known to indicate that the font
			// does not support that char
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			// If still unknown then exit the function and draw no text
			if (!glyph)
				return;

			// If the char is a tab then we will use ' ' as the glyph
			if (character == '\t')
				glyph = fontGeometry.getGlyph(' ');

			// Get the texture coordinates for the glyph in the font texture atlas
			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			// Get the size of the glyph
			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			// Set the vertex data for the single char to draw
			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Data.TextVertexBufferPtr++;

			// Increment the index count by the number of indices needed for a quad
			s_Data.TextIndexCount += 6;
			// Update the number of quads in the statistics
			s_Data.Stats.QuadCount++;

			// Advance the x position if there is more chars in the string to render
			if (i < string.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = string[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				float kerningOffset = 0.0f;
				x += fsScale * advance + kerningOffset;
			}
		}
	}

	void Renderer2D::NextBatch()
	{
		// Draw the current batch
		Flush();
		// Then stat the new batch
		StartBatch();
	}

	void Renderer2D::ResetStats()
	{
		// Reset the 2D renderer statistics by setting all of the data in it to 0
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
	
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}