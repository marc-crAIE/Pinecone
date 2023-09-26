include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Pinecone"
	architecture "x86_64"
	startproject "Pinecone-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Pinecone/vendor/GLFW"
	include "Pinecone/vendor/Glad"
	include "Pinecone/vendor/imgui"
	include "Pinecone/vendor/msdf-atlas-gen"
group ""

group "Core"
	include "Pinecone"
group ""

group "Tools"
	include "Pinecone-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""
