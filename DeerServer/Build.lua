project "DeerServer"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files {
    "src/DeerServer/**.h",
	"src/DeerServer/**.cpp"
	}
   debugdir "../Roe"

   includedirs
   {
      "src",
	  "../Deer/src",
	  "../Deer/vendor/spdlog/include",
	  "../Deer/vendor/glm",
	  "../Deer/vendor/entt/include"
   }

   links
   {
      "DeerService",
	  "spdlog"
   }

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")
   defines { "DEER_SERVICE" }

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