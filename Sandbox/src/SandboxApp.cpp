#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

using namespace Pinecone;

class SandboxApp : public Application
{

};

Application* Pinecone::CreateApplication()
{
	return new SandboxApp();
}
