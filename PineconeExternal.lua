IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Pinecone/Pinecone/vendor/stb_image"
IncludeDir["GLFW"] = "%{wks.location}/Pinecone/Pinecone/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Pinecone/Pinecone/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Pinecone/Pinecone/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Pinecone/Pinecone/vendor/entt/include"
IncludeDir["msdfgen"] = "%{wks.location}/Pinecone/Pinecone/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Pinecone/Pinecone/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}

Library = {}

group "Dependencies"
	include "Pinecone/vendor/GLFW"
	include "Pinecone/vendor/Glad"
	include "Pinecone/vendor/msdf-atlas-gen"
group ""

group "Core"
	include "Pinecone"
group ""