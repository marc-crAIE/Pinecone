#include "pcpch.h"
#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pinecone
{
	/// <summary>
	/// Convert the shader type string read from the value of #type in the shader file to a
	/// Glad shader type value
	/// </summary>
	/// <param name="type">The shader type string</param>
	/// <returns>The Glad shader type</returns>
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		// Convert the shader string type to a Glad enum value
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		// Could not figure out what shader type was specified or it is currently not supported
		PC_CORE_ASSERT(false, "Unknown or unsupported shader type!");
		return 0;
	}

	Shader::Shader(const std::string& filepath)
	{
		// Read the shader code form the file
		std::string source = ReadFile(filepath);
		// Then process the code into the shader types and their code
		auto shaderSources = PreProcess(source);
		// Compile the processed shaders
		Compile(shaderSources);
	}

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create a map of our shader types and their code
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		// Compile the shaders
		Compile(sources);
	}

	Shader::~Shader()
	{
		// Delete the shader
		glDeleteProgram(m_RendererID);
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;
		// Open the file
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			// Go to the end of the file to get its total size
			in.seekg(0, std::ios::end);
			// Resize the code result to the size of the file data
			result.resize(in.tellg());
			// Go back to the beginning of the file and read all of the data into our result
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			// Close the file
			in.close();
		}
		else
		{
			// Failed to open the file as it does not seem to exist at the filepath
			PC_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		// Return our code result
		return result;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		// The token to look for that contains the shader type written after it in the shader code
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		// Find the file position of the first type
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			// Check to make sure that after our token, it is not the end of the line
			PC_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			// Get the shader type text
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			// Make sure it is a supported/known shader type
			PC_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			// Find the next token
			pos = source.find(typeToken, nextLinePos);
			// Add the shader type and its code
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		// Return all of the shaders we retrieved from the file
		return shaderSources;
	}

	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Create a new program to attach our shaders to
		GLuint program = glCreateProgram();
		// Used to keep track of the created shader ID's
		std::vector<GLenum> glShaderIDs;
		for (auto& kv : shaderSources)
		{
			// Get the shader type and the source code
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create a new shader based on the type
			GLuint shader = glCreateShader(type);

			// Set the source code of the shader we created
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the shader code
			glCompileShader(shader);

			// Check to make sure the shader has been successfully compiled
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				// If it failed to compile, get the reason from the log message
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// Delete the shader
				glDeleteShader(shader);

				// Print the error
				PC_CORE_ERROR("{0}", infoLog.data());
				PC_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			// Attach the shader to our program if it compiled successfully
			glAttachShader(program, shader);
			// Keep track of the shader ID we created
			glShaderIDs.push_back(shader);
		}

		// Store the ID of the program to reference it later in our code
		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Check to make sure our program was successfully linked
		GLint isLinked = 0;
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			// If it failed to link, get the reason from the log message
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Delete the shaders as we also don't need them anymore
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			// Print the error
			PC_CORE_ERROR("{0}", infoLog.data());
			PC_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Detach the shaders
		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}

	void Shader::Bind() const
	{
		// Use our shader program
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		// No longer use this shader program
		glUseProgram(0);
	}

	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		// Get the location of the variable and set its value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		return CreateRef<Shader>(filepath);
	}

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return CreateRef<Shader>(vertexSrc, fragmentSrc);
	}
}