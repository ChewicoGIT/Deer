project "Deer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files {
   "src/**.h",
   "src/**.cpp",
   "vendor/ImGuizmo/**.cpp",
   "vendor/stb/stb_image.cpp"
   }

   links { "spdlog", "GLFW", "glad", "ImGui", "angelScript" } 

   includedirs
   {
      "src",
	  "vendor/spdlog/include",
	  "vendor/GLFW/include",
	  "vendor/glad/include",
	  "vendor/imgui",
	  "vendor/glm",
	  "vendor/stb",
	  "vendor/ImGuizmo",
	  "vendor/entt/include",
	  "vendor/cereal/include",
	  "vendor/objload/include/objload",
	  "vendor/angelScript/include"
   }

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

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