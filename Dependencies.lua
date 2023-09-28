
-- Pinecone Dependencies

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Pinecone/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Pinecone/vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/Pinecone/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Pinecone/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Pinecone/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/Pinecone/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Pinecone/vendor/ImGuizmo"
IncludeDir["entt"] = "%{wks.location}/Pinecone/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/Pinecone/vendor/mono/include"
IncludeDir["msdfgen"] = "%{wks.location}/Pinecone/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Pinecone/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Pinecone/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"