project "Pinecone-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8.1"

	targetdir ("../Pinecone-Editor/Resources/Scripts")
	objdir ("../Pinecone-Editor/Resources/Scripts/Intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"