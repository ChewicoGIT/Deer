require("vendor/premake-vscode")

workspace "Deer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"


OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "core"
include "Deer/Build-Service.lua"
include "Deer/Build.lua"
group ""

group "external"
include "Deer/vendor/spdlog/Build.lua"
include "Deer/vendor/GLFW/Build.lua"
include "Deer/vendor/glad/Build.lua"
include "Deer/vendor/imgui/Build.lua"
include "Deer/vendor/angelScript/Build.lua"
group ""

-- group "Runtime"
-- include "DeerRuntime/Build.lua"
-- group ""

group "Studio"
include "DeerStudio/Build.lua"
group ""