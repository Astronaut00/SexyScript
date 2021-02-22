// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Settings.h"
#include "Utils.h"
#include "Recoil.h"
#include "Globals.h"
#include <d3d9.h>
#include "skCrypter.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <thread>

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

settings g_settings;

using namespace std;

static inline void SetupImGuiStyle()
{
    constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b, uint8_t a=255)
    {
        return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0);
    };

    auto& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    const ImVec4 bgColor = ColorFromBytes(41, 43, 47);
    const ImVec4 titleColor = ColorFromBytes(32, 34, 37);
    const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
    const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

    const ImVec4 panelColor = ColorFromBytes(32, 34, 37);
    const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
    const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

    const ImVec4 textColor = ColorFromBytes(255, 255, 255);
    const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
    const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

    colors[ImGuiCol_Text] = textColor;
    colors[ImGuiCol_TextDisabled] = textDisabledColor;
    colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = bgColor;
    colors[ImGuiCol_PopupBg] = bgColor;
    colors[ImGuiCol_Border] = borderColor;
    colors[ImGuiCol_BorderShadow] = borderColor;
    colors[ImGuiCol_FrameBg] = panelColor;
    colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
    colors[ImGuiCol_FrameBgActive] = panelActiveColor;
    colors[ImGuiCol_TitleBg] = titleColor;
    colors[ImGuiCol_TitleBgActive] = titleColor;
    colors[ImGuiCol_TitleBgCollapsed] = titleColor;
    colors[ImGuiCol_MenuBarBg] = panelColor;
    colors[ImGuiCol_ScrollbarBg] = panelColor;
    colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
    colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
    colors[ImGuiCol_CheckMark] = panelActiveColor;
    colors[ImGuiCol_SliderGrab] = panelHoverColor;
    colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
    colors[ImGuiCol_Button] = panelColor;
    colors[ImGuiCol_ButtonHovered] = panelHoverColor;
    colors[ImGuiCol_ButtonActive] = panelHoverColor;
    colors[ImGuiCol_Header] = panelColor;
    colors[ImGuiCol_HeaderHovered] = panelHoverColor;
    colors[ImGuiCol_HeaderActive] = panelActiveColor;
    colors[ImGuiCol_Separator] = borderColor;
    colors[ImGuiCol_SeparatorHovered] = borderColor;
    colors[ImGuiCol_SeparatorActive] = borderColor;
    colors[ImGuiCol_ResizeGrip] = bgColor;
    colors[ImGuiCol_ResizeGripHovered] = panelColor;
    colors[ImGuiCol_ResizeGripActive] = lightBgColor;
    colors[ImGuiCol_PlotLines] = panelActiveColor;
    colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
    colors[ImGuiCol_PlotHistogram] = panelActiveColor;
    colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
    colors[ImGuiCol_ModalWindowDarkening] = bgColor;
    colors[ImGuiCol_DragDropTarget] = bgColor;
    colors[ImGuiCol_NavHighlight] = bgColor;
    colors[ImGuiCol_Tab] = bgColor;
    colors[ImGuiCol_TabActive] = panelActiveColor;
    colors[ImGuiCol_TabUnfocused] = bgColor;
    colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
    colors[ImGuiCol_TabHovered] = panelHoverColor;

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
}

void menu_control()
{
    try
    {
        static bool pressed_insert = false;
        static bool was_tabbed_out_while_active = false;

        static HWND TaskBar = ::FindWindow(skCrypt(L"Shell_TrayWnd"), NULL);
        HWND foreground_window = GetForegroundWindow();
        bool insert_held = (GetKeyState(VK_INSERT) & 0x8000);
        bool tabbed_into_rust_while_menu_active = (globals::menu_active && foreground_window == globals::rust);

        bool tabbed_out_while_menu_inactive = (foreground_window != globals::rust && !globals::menu_active);
        bool insert_instance = (!pressed_insert && insert_held);
        bool nigger_in_my_context = (foreground_window == globals::overlay || foreground_window == globals::rust);
        bool tabbed_out_while_menu_active = (!nigger_in_my_context && globals::menu_active);

        if (tabbed_out_while_menu_active)
        {
            utils::SetWindowClickable(false, globals::overlay);
            globals::menu_active = false;
            //SetWindowVisible(true, tWnd);
            utils::SetWindowVisible(true, TaskBar);
        }
        else if (insert_instance) //insert pressed ONCE
        {
            if (nigger_in_my_context) //if the person is either tabbed into the overlay or tabbed into the game
            {
                globals::menu_active = !globals::menu_active;

                if (globals::menu_active) //Menu is now OPENING
                {
                    utils::SetWindowClickable(true, globals::overlay);
                    utils::SetWindowVisible(false, TaskBar);
                    SetForegroundWindow(globals::overlay); // bring the window to the front
                }
                else if (!globals::menu_active) // Menu is now CLOSING
                {
                    utils::SetWindowClickable(false, globals::overlay);
                    SetForegroundWindow(globals::rust); //move the game back to the front
                    utils::SetWindowVisible(true, globals::rust);
                    utils::SetWindowVisible(true, TaskBar);
                }
            }
            pressed_insert = true;
        }

        if (!globals::menu_active) //if the window is not active we attatch it to the game
        {
            SetWindowPos(globals::rust, globals::overlay, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
        else
        {
            if (!nigger_in_my_context)
                SetWindowPos(globals::overlay, globals::rust, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        if (!insert_held && pressed_insert)
            pressed_insert = false;
    }
    catch (std::exception e) {
        utils::echo_error(e, 0x08);
    }
}

int menu()
{
    // Create application window
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = (LPCWSTR)" ";
    wc.lpszMenuName = (LPCWSTR)" ";
    wc.style = CS_VREDRAW | CS_HREDRAW;

    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    try
    {

        if (!RegisterClassEx(&wc))
            throw std::exception("Failed to register class");

        RECT desktop;

        const HWND hDesktop = GetDesktopWindow();

        GetWindowRect(hDesktop, &desktop);

        int horizontal = desktop.right;
        int vertical = desktop.bottom;

        int w = utils::random_float(58, 109);
        int h = utils::random_float(44, 127);

        globals::full_screen = screen_size{ horizontal + h, vertical + w };

        globals::small_screen = screen_size{ 80 + h, 100 + w };

        globals::overlay = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, (LPCWSTR)" ", (LPCWSTR)" ", WS_POPUP, 1, 1, globals::small_screen.w, globals::small_screen.h, 0, 0, 0, 0);
        SetLayeredWindowAttributes(globals::overlay, 0, 1.0f, LWA_ALPHA);
        SetLayeredWindowAttributes(globals::overlay, 0, RGB(0, 0, 0), LWA_COLORKEY);
        SetForegroundWindow(globals::rust);
        ShowWindow(globals::overlay, SW_SHOW);

        if (!CreateDeviceD3D(globals::overlay))
        {
            CleanupDeviceD3D();
            ::UnregisterClass(wc.lpszClassName, wc.hInstance);
            return 1;
        }

        ::ShowWindow(globals::overlay, SW_SHOWDEFAULT);
        ::UpdateWindow(globals::overlay);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = NULL;

        SetupImGuiStyle();

        ImGui_ImplWin32_Init(globals::overlay);
        ImGui_ImplDX9_Init(g_pd3dDevice);
    }
    catch (std::exception e) {
        utils::echo_error(e, 0x06);
    }

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (!IsWindow(globals::rust))
            break;

        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        const char* weapons[] = { "None", "AK-47", "LR-300", "M249", "MP5A4", "Thompson", "Custom" };
        const char* scopes[] = { "None", "Handmade", "Holosight", "8X", "16X" };
        const char* barrels[] = { "None", "Silencer", "Muzzle boost", "Muzzle brake" };
        static const char* current_weapon = "None";
        static const char* current_scope = "None";
        static const char* current_barrel = "None";
        static std::vector <const char*> current_item = { current_weapon, current_scope, current_barrel };
        bool* p_open = NULL;

        if (globals::menu_active)
        {
            try
            {
                ImGui::Begin(skCrypt("Sexycheats"), p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);  // Create our main window and append into it.
                ImGui::SetWindowSize(ImVec2{ 235, 285 });

                static bool setpos = false;

                if (setpos == false)
                {
                   ImGui::SetWindowPos(ImVec2{ 10, 10 });
                   setpos = true;
                }

                ImGui::Checkbox(skCrypt("Enable"), &g_settings.enabled);

                ImGui::Text(skCrypt("Humanization"));

                ImGui::SetNextItemWidth(220);
                ImGui::SliderFloat(skCrypt("##humaniztion"), &g_settings.humanize, 0.f, 1.f, skCrypt("%.2f"));

                ImGui::Text(skCrypt("Weapons"));

                ImGui::SetNextItemWidth(220);
                if (ImGui::BeginCombo(skCrypt("##weapons"), current_item[0], ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_PopupAlignLeft)) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < IM_ARRAYSIZE(weapons); n++)
                    {
                        bool is_selected = (current_item[0] == weapons[n]); // You can store your selection however you want, outside or inside your objects
                        if (ImGui::Selectable(weapons[n], is_selected))
                            current_item[0] = weapons[n];
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        }
                    }
                    ImGui::EndCombo();
                }

                ImGui::Text(skCrypt("Scopes"));

                ImGui::SetNextItemWidth(220);
                if (ImGui::BeginCombo(skCrypt("##scopes"), current_item[1], ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < IM_ARRAYSIZE(scopes); n++)
                    {
                        bool is_selected = (current_item[1] == scopes[n]); // You can store your selection however you want, outside or inside your objects
                        if (ImGui::Selectable(scopes[n], is_selected))
                            current_item[1] = scopes[n];
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        }
                    }
                    ImGui::EndCombo();
                }

                ImGui::Text(skCrypt("Barrels"));

                ImGui::SetNextItemWidth(220);
                if (ImGui::BeginCombo(skCrypt("##barrels"), current_item[2], ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < IM_ARRAYSIZE(barrels); n++)
                    {
                        bool is_selected = (current_item[2] == barrels[n]); // You can store your selection however you want, outside or inside your objects
                        if (ImGui::Selectable(barrels[n], is_selected))
                            current_item[2] = barrels[n];
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        }
                    }
                    ImGui::EndCombo();
                }

                ImGui::Hotkey(skCrypt("Weapon cycler    "), &g_settings.weapon_cycler, ImVec2{ 220, 20 });
                ImGui::Hotkey(skCrypt("Scope cycler     "), &g_settings.scope_cycler, ImVec2{ 220, 20 });
                ImGui::Hotkey(skCrypt("Barrel cycler    "), &g_settings.barrel_cycler, ImVec2{ 220, 20 });

                ImGui::End();

            }
            catch (std::exception e) {
                utils::echo_error(e, 0x04);
            }
        }
        else
        {
            if (g_settings.enabled)
            {
                ImGui::Begin(skCrypt("Settings"), p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);  // Create our main window and append into it.
                ImGui::SetWindowSize(ImVec2{ 100, 80 });
                ImGui::SetWindowPos(ImVec2{ 0, 0 });

                ImGui::Text(current_item[0]);
                ImGui::Text(current_item[1]);
                ImGui::Text(current_item[2]);
                ImGui::End();
            }
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        menu_control();
        recoil::recoil();

        static bool pressed_1 = false;
        try
        {
            if (utils::key_down(g_settings.weapon_cycler) && !pressed_1)
            {
                int current_index = std::distance(weapons, std::find(weapons, weapons + IM_ARRAYSIZE(weapons), current_item[0]));
                int next_index;

                if (current_index + 1 >= IM_ARRAYSIZE(weapons))
                {
                    next_index = 0;
                }
                else
                {
                    next_index = std::distance(weapons, std::find(weapons, weapons + IM_ARRAYSIZE(weapons), weapons[current_index + 1]));
                }
                current_item[0] = weapons[next_index];
                pressed_1 = true;
            }
            else if (!utils::key_down(g_settings.weapon_cycler) && pressed_1)
            {
                pressed_1 = false;
            }
            g_settings.weapon = std::distance(weapons, std::find(weapons, weapons + IM_ARRAYSIZE(weapons), current_item[0]));

            static bool pressed_2 = false;

            if (utils::key_down(g_settings.scope_cycler) && !pressed_2)
            {
                int current_index = std::distance(scopes, std::find(scopes, scopes + IM_ARRAYSIZE(scopes), current_item[1]));
                int next_index;

                if (current_index + 1 >= IM_ARRAYSIZE(scopes))
                {
                    next_index = 0;
                }
                else
                {
                    next_index = std::distance(scopes, std::find(scopes, scopes + IM_ARRAYSIZE(scopes), scopes[current_index + 1]));
                }
                current_item[1] = scopes[next_index];
                pressed_2 = true;
            }
            else if (!utils::key_down(g_settings.scope_cycler) && pressed_2)
            {
                pressed_2 = false;
            }
            g_settings.scope = std::distance(scopes, std::find(scopes, scopes + IM_ARRAYSIZE(scopes), current_item[1]));

            static bool pressed_3 = false;

            if (utils::key_down(g_settings.barrel_cycler) && !pressed_3)
            {
                int current_index = std::distance(barrels, std::find(barrels, barrels + IM_ARRAYSIZE(barrels), current_item[2]));
                int next_index;

                if (current_index + 1 >= IM_ARRAYSIZE(barrels))
                {
                    next_index = 0;
                }
                else
                {
                    next_index = std::distance(barrels, std::find(barrels, barrels + IM_ARRAYSIZE(barrels), barrels[current_index + 1]));
                }
                current_item[2] = barrels[next_index];
                pressed_3 = true;
            }
            else if (!utils::key_down(g_settings.barrel_cycler) && pressed_3)
            {
                pressed_3 = false;
            }
            g_settings.barrel = std::distance(barrels, std::find(barrels, barrels + IM_ARRAYSIZE(barrels), current_item[2]));

        }
        catch (std::exception e) {
            utils::echo_error(e, 0x05);
        }

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(globals::overlay);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

void find_rust()
{
    while (true)
    {
        try
        {
            globals::rust = FindWindow(0, skCrypt(L"Rust"));

            RECT rect;
        
            if (globals::rust && GetWindowRect(globals::rust, &rect))
            {
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;

                if (height > 720)
                {
                    break;
                }
            }
        }
        catch (std::exception e) {
            utils::echo_error(e, 0x07);
        }
        Sleep(2000);
    }
}

// Main code
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    find_rust();
    SetThreadPriority(GetCurrentThread(), 15);

    menu();
    
    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}