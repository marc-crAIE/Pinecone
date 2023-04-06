#pragma once

#include <glm/glm.hpp>

namespace Pinecone
{
	/// <summary>
	/// This is a very basic Camera class. It is really only intended to be used with
	/// creating other camera subclasses (such as the SceneCamera class). However it is
	/// still ok to be used for general use with camera objects that do not need any extra 
	/// functionality
	/// </summary>
	class Camera
	{
	public:
		/// <summary>
		/// The Camera constructor
		/// </summary>
		Camera() = default;
		/// <summary>
		/// The Camera constructor that takes in a view projection matrix
		/// </summary>
		/// <param name="projection">The view projection matrix</param>
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		/// <summary>
		/// The Camera deconstructor
		/// </summary>
		virtual ~Camera() = default;

		/// <summary>
		/// Get the view projection matrix of the camera
		/// </summary>
		/// <returns></returns>
		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}