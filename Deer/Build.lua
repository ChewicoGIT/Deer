project "Deer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files {
   "src/Deer/**.h",
   "src/Deer/**.cpp",
   "src/DeerRender/**.h",
   "src/DeerRender/**.cpp",
   "src/Plattform/OpenGL/**.h",
   "src/Plattform/OpenGL/**.cpp",
   "src/Plattform/ImGUI/**.h",
   "src/Plattform/ImGUI/**.cpp",
   "vendor/ImGuizmo/**.cpp",
   "vendor/stb/stb_image.cpp"
   }

   links { "spdlog", "GLFW", "glad", "ImGui", "angelScript" } 
   defines { "DEER_RENDER" }

   includedirs
   {
      "src",
      "Include",
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
       defines {"WINDOWS"}
	   files {
		"src/Plattform/windows/**.h",
		"src/Plattform/windows/**.cpp"
	   }
    
   filter "system:linux"
       toolset "clang"
       defines {"LINUX"}
       buildoptions { "-std=c++20" }
       
       local gtk_libs = os.outputof("pkg-config --libs gtk+-3.0")

       links { "GL" }
       -- Files for GTK
       includedirs {
            "/usr/include/gtk-3.0",               -- GTK3 headers
            "/usr/include/gdk-pixbuf-2.0",        -- GDK Pixbuf headers
            "/usr/include/cairo",                 -- Cairo graphics library headers
            "/usr/include/pango-1.0",            -- Pango text layout and rendering headers
            "/usr/include/atk-1.0",              -- ATK accessibility toolkit headers
            "/usr/include/gdk-3.0",              -- GDK headers
            "/usr/include/glib-2.0",             -- GLib headers
            "/usr/lib/x86_64-linux-gnu/glib-2.0/include", -- GLib additional headers
            "/usr/include/harfbuzz"
        }

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


        links { "pthread" }
        buildoptions { "-pthread" }
        linkoptions { "-pthread" }

       files {
           "src/Plattform/Linux/**.h",
           "src/Plattform/Linux/**.cpp"
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