project "angelScript"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files {
   "src/**.h",
   "src/**.cpp",
   "src/as_callfunc_x64_msvc_asm.asm"
   }

   includedirs
   {
      "src",
	  "include"
   }

	targetdir ("../../../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../../bin/int/" .. OutputDir .. "/%{prj.name}")

	defines {
	"ANGELSCRIPT_EXPORT",
    "_LIB",
	"AS_X64_MSVC"}

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" , 
		"AS_DEBUG",
		"_DEBUG" }
		
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" , 
		"NDEBUG" }
		
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" , 
		"NDEBUG"}
		
       runtime "Release"
       optimize "On"
       symbols "Off"