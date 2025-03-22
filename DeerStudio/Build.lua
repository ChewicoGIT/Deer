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
	  "../Deer/Include",
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
	  "ImGui",
      "angelScript"
   }
   defines { "DEER_RENDER" }

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

   filter "system:linux"
       toolset "clang"
       defines { "LINUX" }
       links { "GL", "glfw" }
       links { "glad" }
       buildoptions { "-std=c++20" }
        -- Link libraries for GTK3 and its dependencies
        links {
            "gtk-3",           -- GTK3 library
            "gdk-3",           -- GDK library
            "glib-2.0",        -- GLib library
            "pango-1.0",       -- Pango library
            "atk-1.0",         -- ATK library
            "cairo",           -- Cairo graphics library
            "gdk_pixbuf-2.0",  -- GDK Pixbuf library
            "gio-2.0",        -- GIO library
            "gobject-2.0",     -- GObject library
            "pthread"         -- POSIX threads library
        }

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