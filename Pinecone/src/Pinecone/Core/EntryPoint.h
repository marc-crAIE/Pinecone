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

	// Call the create application function
	auto app = Pinecone::CreateApplication();
	// Run the application
	app->Run();
	// Delete the application after it has exited
	delete app;
}