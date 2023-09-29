local PineconeRootDir = '../../..'
include (PineconeRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Sandbox"
	architecture "x86_64"
	startproject "Assembly-CSharp"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Assembly-CSharp"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Bin")
	objdir ("Bin-Int")

	files 
	{
		"Assets/**.cs",
	}

	includedirs
	{
		"Assets/**"
	}

	links
	{
		"Pinecone-ScriptCore"
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

group "Pinecone"
	include (PineconeRootDir .. "/Pinecone-ScriptCore")
group ""