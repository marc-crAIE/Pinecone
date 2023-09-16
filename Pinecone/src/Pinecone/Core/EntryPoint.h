#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Application.h"

// This function MUST be defined in the client program
extern Pinecone::Application* Pinecone::CreateApplication();

int main(int argc, char** argv)
{
	// Initialize the logger
	Pinecone::Log::Init();
	PC_CORE_INFO("Initialized Log!");

	PC_PROFILE_BEGIN_SESSION("Startup", "PineconeProfile-Startup.json");
	// Call the create application function
	auto app = Pinecone::CreateApplication();
	PC_PROFILE_END_SESSION();

	PC_PROFILE_BEGIN_SESSION("Runtime", "PineconeProfile-Runtime.json");
	// Run the application
	app->Run();
	PC_PROFILE_END_SESSION();

	PC_PROFILE_BEGIN_SESSION("Shutdown", "PineconeProfile-Shutdown.json");
	// Delete the application after it has exited
	delete app;
	PC_PROFILE_END_SESSION();
}