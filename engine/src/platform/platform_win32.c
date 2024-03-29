#include "platform.h"

#if KPLATFORM_WINDOWS

#include "containers/darray.h"

#include "core/logger.h"
#include "core/input.h"
#include "core/event.h"

#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include "renderer/vulkan/vulkan_types.inl"

typedef struct platform_state {
    HINSTANCE h_instance;
    HWND hwnd;
    VkSurfaceKHR surface;
} platform_state;

static platform_state* state_ptr;

static Double clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win_32_process_message(HWND hwnd, UInt32 msg, WPARAM w_param, LPARAM l_param);

void clock_setup() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (Double)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
}

Boolean platform_system_startup(
    UInt64 *memory_requirement,
    void *state,
    const char* application_name,
    Int32 x,
    Int32 y,
    Int32 width,
    Int32 height) {

    *memory_requirement = sizeof(platform_state);

    if (state == 0) {
        return TRUE;
    }

    state_ptr = state;
    state_ptr->h_instance = GetModuleHandleA(0);

    HICON icon = LoadIcon(state_ptr->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = win_32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state_ptr->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "kohi_window_class";
    
    if(!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }
    
    UInt32 client_x = x;
    UInt32 client_y = y;
    UInt32 client_width = width;
    UInt32 client_height = height;
    
    UInt32 window_x = client_x;
    UInt32 window_y = client_y;
    UInt32 window_width = client_width;
    UInt32 window_height = client_height;
    
    UInt32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    UInt32 window_ex_style = WS_EX_APPWINDOW;
    
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;
    
    RECT border_rect = { 0, 0, 0, 0 };
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);
    
    window_x += border_rect.left;
    window_y += border_rect.top;
    
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;
    
    HWND handle = CreateWindowExA(
        window_ex_style, "kohi_window_class", application_name, window_style, window_x, window_y, 
        window_width, window_height, 0, 0, state_ptr->h_instance, 0);
    
    if (handle == 0) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        KFATAL("Window creation failed.");
        return FALSE;
    }
    else {
        state_ptr->hwnd = handle;
    }
    
    Bool32 should_activate = 1;
    Int32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(state_ptr->hwnd, show_window_command_flags);
    
    clock_setup();
    
    return TRUE;
}

void platform_system_shutdown(void *plat_state) {
    if (state_ptr && state_ptr->hwnd) {
        DestroyWindow(state_ptr->hwnd);
        state_ptr->hwnd = 0;
    }
}

Boolean platform_pump_messages() {
    if (state_ptr) {
        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
    return TRUE;
}

void* platform_allocate(UInt64 size, Boolean aligned) {
    return malloc(size);
}

void platform_free(void* block, Boolean aligned) {
    free(block);
}

void* platform_zero_memory(void* block, UInt64 size) {
    return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, UInt64 size) {
    return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, Int32 value, UInt64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char* message, UInt8 color) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static UInt8 levels[6] = { 64, 4, 6, 2, 1, 8 };
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(message);
    UInt64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, UInt8 color) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    static UInt8 levels[6] = { 64, 4, 6, 2, 1, 8 };
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(message);
    UInt64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

Double platform_get_absolute_time() {
    if (!clock_frequency) {
        clock_setup();
    }

    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (Double)now_time.QuadPart * clock_frequency;
}

void platform_sleep(UInt64 ms) {
    Sleep(ms);
}

void platform_get_required_extension_names(const char*** names_darray) {
    darray_push(*names_darray, &"VK_KHR_win32_surface");
}

Boolean platform_create_vulkan_surface(vulkan_context *context) {
    VkWin32SurfaceCreateInfoKHR create_info = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
    create_info.hinstance = state_ptr->h_instance;
    create_info.hwnd = state_ptr->hwnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &state_ptr->surface);
    if (result != VK_SUCCESS) {
        KFATAL("Vulkan surface creation failed.");
        return FALSE;
    }

    context->surface = state_ptr->surface;
    
    return TRUE;
}

LRESULT CALLBACK win_32_process_message(HWND hwnd, UInt32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg)
    {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            event_context data = { };
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
            return TRUE;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
        {
            RECT r;
            GetClientRect(hwnd, &r);
            UInt32 width = r.right - r.left;
            UInt32 height = r.bottom - r.top;

            event_context context;
            context.data.u16[0] = (UInt16)width;
            context.data.u16[1] = (UInt16)height;
            event_fire(EVENT_CODE_RESIZED, 0, context);
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            Boolean pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            keys key = (UInt16)w_param;

            Boolean is_extended = (HIWORD(l_param) & KF_EXTENDED) == KF_EXTENDED;

            if (w_param == VK_MENU) {
               key = is_extended ? KEY_RALT : KEY_LALT;
            }
            else if (w_param == VK_SHIFT) {
                UInt32 left_shift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                UInt32 scancode = ((l_param & (0xFF << 16)) >> 16);
                key = scancode == left_shift ? KEY_LSHIFT : KEY_RSHIFT;
            }
            else if (w_param == VK_CONTROL) {
                key = is_extended ? KEY_RCONTROL : KEY_LCONTROL;
            }
             
            input_process_key(key, pressed);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            Int32 x_position = GET_X_LPARAM(l_param);
            Int32 y_position = GET_Y_LPARAM(l_param);

            input_process_mouse_move(x_position, y_position);
        } break;
        case WM_MOUSEWHEEL:
        {
            Int32 zDelta = GET_WHEEL_DELTA_WPARAM(w_param);
            if(zDelta != 0) {
                zDelta = (zDelta < 0) ? -1 : 1;
                input_process_mouse_wheel(zDelta);
            }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            Boolean pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN;
            buttons mouse_button = BUTTON_MAX_BUTTONS;
            switch (msg) {
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
                mouse_button = BUTTON_LEFT;
                break;
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
                mouse_button = BUTTON_MIDDLE;
                break;
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                mouse_button = BUTTON_RIGHT;
                break;
            }
            
            if (mouse_button != BUTTON_MAX_BUTTONS)
                input_process_mouse_button(mouse_button, pressed);
        } break;
    }

    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif