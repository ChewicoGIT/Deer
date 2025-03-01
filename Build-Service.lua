require("premakeExtensions/premake-vscode")

workspace "Deer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"


OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "core"
include "Deer/Build-Service.lua"
group ""

group "external"
include "Deer/vendor/spdlog/Build.lua"
include "Deer/vendor/angelScript/Build.lua"
group ""

group "Runtime"
include "DeerServer/Build.lua"
group ""
