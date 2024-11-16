project "DeerService"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   
   files {
    "src/Deer/**.h",
	"src/Deer/**.cpp"
	}

   links { "spdlog", "angelScript" } 

   includedirs
   {
      "src",
	  "vendor/spdlog/include",
	  "vendor/glm",
	  "vendor/entt/include",
	  "vendor/cereal/include",
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