project "glad"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.c" }

   includedirs
   {
      "include"
   }
   
   targetdir ("../../../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../../../bin/int/" .. OutputDir .. "/%{prj.name}")

   filter "system:linux"
       toolset "gcc"
       defines { "LINUX" }
       links { "GL" }

   filter "system:windows"
       systemversion "latest"
       defines { }
