project "spdlog"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "include"
   }
   
   defines { "SPDLOG_COMPILED_LIB" }

   targetdir ("../../../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../../../bin/int/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }
