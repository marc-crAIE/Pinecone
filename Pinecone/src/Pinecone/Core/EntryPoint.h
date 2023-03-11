#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Application.h"

extern Pinecone::Application* Pinecone::CreateApplication();

int main(int argc, char** argv)
{
	Pinecone::Log::Init();
	PC_CORE_INFO("Initialized Log!");

	auto app = Pinecone::CreateApplication();
	app->Run();
	delete app;
}