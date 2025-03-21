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
      defines { "WINDOWS"}
      defines { }

   filter "system:linux"
       toolset "clang"
       buildoptions { "-std=c++20" }
       defines { "LINUX"}
       files {
         "src/Plattform/linux/**.h",
         "src/Plattform/linux/**.cpp"
       }
       

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