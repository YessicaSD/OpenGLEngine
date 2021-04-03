workspace "Shadow"
    architecture "x64"
   configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

   outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
   startproject "Sandbox"

IncludeDir = {}
IncludeDir["GLFW"] = "Shadow/vendor/GLFW/include"
IncludeDir["Glad"] = "Shadow/vendor/Glad/include"
IncludeDir["imgui"] = "Shadow/vendor/imgui"
IncludeDir["glm"] = "Shadow/vendor/glm"
IncludeDir["assimp"] = "Shadow/vendor/assimp/include"

include "Shadow/vendor/GLFW"
include "Shadow/vendor/Glad"
include "Shadow/vendor/imgui"
include "Shadow/vendor/assimp"

project "Shadow"
    location "Shadow"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
   
   pchheader "swpch.h"
   pchsource "Shadow/src/swpch.cpp"

   files 
   { 
       "%{prj.name}/src/**.h",
       "%{prj.name}/src/**.cpp",
   }

   defines
   {
    "_CRT_SECURE_NO_WARNINGS"
   }
   includedirs
   {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.assimp}"
   }
   links
   {
       "GLFW",
       "Glad",
       "ImGui",
       "assimp",
       "opengl32.lib"
   }
    filter "system:windows"
        systemversion "latest"
        defines 
        { 
            "SW_PLATFORM_WINDOWS",
            "SW_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

   filter "configurations:Debug"
      defines "SW_DEBUG"
      symbols "on"

   filter "configurations:Release"
      defines "SW_RELEASE"
      optimize "on"

   filter "configurations:Dist"
      defines "SW_DIST"
      optimize "on"



      
 project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
   

     files 
   { 
       "%{prj.name}/src/**.h",
       "%{prj.name}/src/**.cpp",
   }

   includedirs 
   {
        "Shadow/vendor/spdlog/include",
        "Shadow/vendor/imgui",
        "Shadow/src",
        "%{IncludeDir.glm}"
   }

   links 
   {
       "Shadow"
   }

    filter "system:windows"
        systemversion "latest"
        defines 
        { 
            "SW_PLATFORM_WINDOWS"
        }
    
   filter "configurations:Debug"
      defines "SW_DEBUG"
      symbols "on"

   filter "configurations:Release"
      defines "SW_RELEASE"
      optimize "on"

   filter "configurations:Dist"
      defines "SW_DIST"
      optimize "on"

   