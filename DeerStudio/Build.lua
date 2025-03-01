project "DeerStudio"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"
   
   debugdir "../Roe"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
	  "../Deer/src",
	  "../Deer/vendor/spdlog/include",
	  "../Deer/vendor/imgui",
	  "../Deer/vendor/glm",
	  "../Deer/vendor/ImGuizmo",
	  "../Deer/vendor/entt/include"
   }

   links
   {
      "Deer",
	  "spdlog",
	  "ImGui"
   }
   defines { "DEER_RENDER" }

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

   filter "system:linux"
       toolset "clang"
       defines { "LINUX" }
       links { "GL", "glfw" }
       links { "glad" }
       links { "angelScript" }
       buildoptions { "-std=c++20" }

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"