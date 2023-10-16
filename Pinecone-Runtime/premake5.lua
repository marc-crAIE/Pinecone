project "Pinecone-Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("../Pinecone-Editor/Resources/Runtime")
	objdir ("../Pinecone-Editor/Resources/Runtime/Intermediates")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Pinecone/vendor/spdlog/include",
		"%{wks.location}/Pinecone/src",
		"%{wks.location}/Pinecone/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Pinecone"
	}

    
    postbuildcommands {
        '{COPY} "../Pinecone-Editor/Resources/Scripts/Pinecone-ScriptCore.dll" "Resources/Scripts/"',
        '{COPY} "../Pinecone-Editor/Resources/Scripts/Pinecone-ScriptCore.pdb" "Resources/Scripts/"'
    }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
        defines "PC_DIST"
        kind "WindowedApp"
		runtime "Release"
		optimize "on"
