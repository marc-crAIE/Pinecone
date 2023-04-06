#pragma once

#include "Pinecone/Renderer/VertexArray.h"
#include "Pinecone/Renderer/Shader.h"
#include "Pinecone/Renderer/Camera.h"

namespace Pinecone
{
	class Renderer
	{
	public:
		/// <summary>
		/// Initialize the Renderer
		/// </summary>
		static void Init();
		/// <summary>
		/// Shutdown the Renderer
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Submit a vertex array of indices and vertex data along with the shader.
		/// Make sure that the BufferLayout inside the VertexBuffer matches the layout of the 
		/// variables inside the shader specified. 
		/// 
		/// Ensure that you have called BeginScene before submitting data to be drawn by the renderer
		/// as the renderer needs to get a view projection matrix from a camera to be used to render the scene.
		/// </summary>
		/// <param name="shader">The shader used to render the vertex array</param>
		/// <param name="vertexArray">The vertex array consisting of the index buffer and vertex data</param>
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		/// <summary>
		/// Begin rendering the scene. Required to be called as the renderer needs to get the view projection matrix
		/// from a camera to draw the scene using a shader.
		/// </summary>
		/// <param name="camera">The camera</param>
		static void BeginScene(Camera& camera);
		/// <summary>
		/// End drawing the scene
		/// </summary>
		static void EndScene();

		/// <summary>
		/// Resize the OpenGL viewport. Typically should be called when the window is resized
		/// </summary>
		/// <param name="width">The width of the viewport</param>
		/// <param name="height">The height of the viewport</param>
		static void OnWindowResize(int width, int height);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}