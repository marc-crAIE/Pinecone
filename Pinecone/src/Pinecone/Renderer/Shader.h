#pragma once

#include <glm/glm.hpp>

typedef uint32_t GLenum;

namespace Pinecone
{
	class Shader
	{
	public:
		/// <summary>
		/// The Shader constructor to load a shader from a file.
		/// </summary>
		/// <param name="filepath">The filepath to the shader file</param>
		Shader(const std::string& filepath);
		/// <summary>
		/// The shader constructor to load a shader from given strings
		/// </summary>
		/// <param name="vertexSrc">The vertex shader code</param>
		/// <param name="fragmentSrc">The fragment shader code</param>
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		/// <summary>
		/// The Shader deconstructor
		/// </summary>
		~Shader();

		/// <summary>
		/// Bind the shader to be used
		/// </summary>
		void Bind() const;
		/// <summary>
		/// Unbind the shader
		/// </summary>
		void Unbind() const;

		/// <summary>
		/// Upload an int value to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="value">The int value to set the variable to</param>
		void UploadUniformInt(const std::string& name, int value);
		/// <summary>
		/// Upload an int array to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="value">The int array value to set the variable to</param>
		/// <param name="count">The size of the int array</param>
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		/// <summary>
		/// Upload a float value to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="value">The float value to set the variable to</param>
		void UploadUniformFloat(const std::string& name, float value);
		/// <summary>
		/// Upload a 2D vector value to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="value">The 2D vector value to set the variable to</param>
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		/// <summary>
		/// Upload a 3D vector value to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="value">The 3D vector value to set the variable to</param>
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		/// <summary>
		/// Upload a 4D vector value to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="value">The 4D vector value to set the variable to</param>
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		/// <summary>
		/// Upload a 3x3 matrix to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="matrix">The 3x3 matri value to set the variable to</param>
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		/// <summary>
		/// Upload a 4x4 matrix to a variable in the shader
		/// </summary>
		/// <param name="name">The name of the variable in the shader</param>
		/// <param name="matrix">The 4x4 matri value to set the variable to</param>
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		/// <summary>
		/// Create a smart shared pointer to a new Shader
		/// </summary>
		/// <param name="filepath">The filepath to the shader file</param>
		/// <returns>A shared pointer to a new Shader</returns>
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		/// <summary>
		/// Read a shader file and get its data
		/// </summary>
		/// <param name="filepath">The file to be read</param>
		/// <returns>The shader code inside the file</returns>
		std::string ReadFile(const std::string& filepath);
		/// <summary>
		/// Process the shader code. It splits the shader code into the shader types based on the #type values
		/// </summary>
		/// <param name="source">The shader source code</param>
		/// <returns>The Glad shader type and its respective code</returns>
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		/// <summary>
		/// Compile the shaders to be used with OpenGL
		/// </summary>
		/// <param name="shaderSources">The shader types and code</param>
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID;
	};
}