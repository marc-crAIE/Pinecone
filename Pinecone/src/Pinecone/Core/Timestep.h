#pragma once

namespace Pinecone
{
	class Timestep
	{
	public:
		/// <summary>
		/// The TimeStep conststructor that creates a new timestep with a 
		/// value of 0 (seconds) by default
		/// </summary>
		/// <param name="time">Time in seconds</param>
		Timestep(float time = 0.0f)
			: m_Time(time) {}

		/// <summary>
		/// Get the time (in seconds)
		/// </summary>
		operator float() const { return m_Time; }

		/// <summary>
		/// Get the time (in seconds)
		/// </summary>
		float GetSeconds() const { return m_Time; }
		/// <summary>
		/// Get the time (in milliseconds)
		/// </summary>
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}