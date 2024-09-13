#pragma once
#include "imgui.h"

void SetupImGuiStyle2();
void setNatureStyle() {
    SetupImGuiStyle2();

    //ImGuiStyle& style = ImGui::GetStyle();
    //ImVec4* colors = style.Colors;
    //
    //// Set background colors to earthy tones
    //colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.22f, 0.17f, 1.0f);  // Dark green (forest floor)
    //colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.20f, 0.15f, 1.0f);  // Similar but a bit lighter
    //
    //// Frame background for inputs and buttons
    //colors[ImGuiCol_FrameBg] = ImVec4(0.17f, 0.24f, 0.20f, 1.0f);  // Olive green
    //colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.34f, 0.24f, 1.0f);  // Lighter green when hovered
    //colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.44f, 0.32f, 1.0f);  // Even lighter green when active
    //
    //// Button colors (wood/bark-like colors)
    //colors[ImGuiCol_Button] = ImVec4(0.40f, 0.30f, 0.22f, 1.0f);  // Brown (wood)
    //colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.38f, 0.28f, 1.0f);  // Lighter brown
    //colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.46f, 0.34f, 1.0f);  // Even lighter brown when clicked
    //
    //// Text colors (cloudy white)
    //colors[ImGuiCol_Text] = ImVec4(0.87f, 0.92f, 0.88f, 1.0f);  // Light cloudy white
    //colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.55f, 0.52f, 1.0f);  // Faded grayish-green
    //
    //// Title bars (deep, rich green like moss)
    //colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.18f, 0.15f, 1.0f);  // Dark green
    //colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.25f, 0.18f, 1.0f);  // Lighter active title
    //colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.14f, 0.12f, 1.0f);  // Collapsed title
    //
    //// Scrollbar (rock/granite tones)
    //colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.16f, 0.12f, 1.0f);  // Dark rock
    //colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.30f, 0.25f, 1.0f);  // Granite gray/brown
    //colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.35f, 0.30f, 1.0f);  // Hovered granite
    //colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.45f, 0.40f, 1.0f);  // Active grab
    //
    //// Headers (deep sky/leaf green)
    //colors[ImGuiCol_Header] = ImVec4(0.20f, 0.36f, 0.26f, 1.0f);  // Forest green
    //colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.42f, 0.32f, 1.0f);  // Brighter forest green
    //colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.50f, 0.38f, 1.0f);  // Even brighter green when active
    //
    //// Borders and separators (stone/rock-like)
    //colors[ImGuiCol_Border] = ImVec4(0.30f, 0.32f, 0.28f, 1.0f);  // Grayish stone
    //colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.30f, 0.26f, 1.0f);  // Slightly lighter separator
    //
    //// Slider grab (water stream blue)
    //colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.60f, 0.75f, 1.0f);  // River blue
    //colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.70f, 0.85f, 1.0f);  // Brighter blue when active
    //
    //// Check mark (bright nature-inspired green)
    //colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.70f, 0.38f, 1.0f);  // Leaf green
    //
    //// Tab colors (deep earth tones)
    //colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.28f, 0.22f, 1.0f);  // Earthy green
    //colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.40f, 0.34f, 1.0f);  // Lighter earth tone
    //colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.44f, 0.38f, 1.0f);  // Bright green for active tab
    //colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.24f, 0.20f, 1.0f);  // Faded green for unfocused tab
    //colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.22f, 0.36f, 0.30f, 1.0f);  // Slightly brighter green for focused but inactive tab
    //
    //// Popups (shadows and natural depth)
    //colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.14f, 0.12f, 1.0f);  // Shadowy green
    //
    //// Plot lines and histograms (sky blue)
    //colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.75f, 0.90f, 1.0f);  // Light blue
    //colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.70f, 0.85f, 1.0f);  // Brighter blue for histograms
    //
    //// Text selections (highlights using sun yellow)
    //colors[ImGuiCol_TextSelectedBg] = ImVec4(0.85f, 0.70f, 0.30f, 0.35f); // Soft yellow highlight
    //
    //// Adjust rounding and padding for a natural flow
    //style.FrameRounding = 5.0f;
    //style.WindowRounding = 7.0f;
    //style.GrabRounding = 5.0f;
    //style.TabRounding = 5.0f;
    //style.ScrollbarRounding = 7.0f;
    //
    //style.FramePadding = ImVec2(8.0f, 4.0f);
    //style.ItemSpacing = ImVec2(8.0f, 6.0f);
}

// From https://github.com/procedural/gpulib/blob/master/gpulib_imgui.h
struct ImVec3 { float x, y, z; ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };

void imgui_easy_theming(ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops)
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Set the rounding for frames (e.g., buttons, input fields)
    style.FrameRounding = 5.0f; // Adjust this value for the desired rounding

    // Set the rounding for windows
    style.WindowRounding = 10.0f; // Adjust this value for the desired rounding

    // Optionally set the rounding for other elements
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;

    style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
    //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.58f);
    style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    //style.Colors[ImGuiCol_ComboBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    //style.Colors[ImGuiCol_Column] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.32f);
    //style.Colors[ImGuiCol_ColumnHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
    //style.Colors[ImGuiCol_ColumnActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    //style.Colors[ImGuiCol_CloseButton] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.16f);
    //style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.39f);
    //style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
    //style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);
}

void SetupImGuiStyle2()
{
    static ImVec3 color_for_text = ImVec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
    static ImVec3 color_for_head = ImVec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
    static ImVec3 color_for_area = ImVec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
    static ImVec3 color_for_body = ImVec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
    static ImVec3 color_for_pops = ImVec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);
    imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);
}
