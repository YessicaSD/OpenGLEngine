project "ImGuiNodeEditor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"crude_json.cpp",
		"crude_json.h",
		"imgui_bezier_math.h",
		"imgui_bezier_math.inl",
		"imgui_canvas.cpp",
		"imgui_canvas.h",
		"imgui_extra_math.h",
		"imgui_extra_math.inl",
		"imgui_node_editor.cpp",
		"imgui_node_editor.h",
		"imgui_node_editor_api.cpp",
		"imgui_node_editor_internal.h",
		"imgui_node_editor_internal.inl"
	}
	includedirs
	{
		"../imgui/"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
