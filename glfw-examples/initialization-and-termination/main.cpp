#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cstdlib>

void* my_malloc(size_t size, void* user)
{
    std::cout << "\t Allocator: malloc(" << size << ") called" << std::endl;
    return malloc(size);
}

void* my_realloc(void* block, size_t size, void* user)
{
    std::cout << "\t Allocator: realloc(" << size << ") called" << std::endl;
    return realloc(block, size);
}

void my_free(void* block, void* user)
{
    std::cout << "\t Allocator: free called" << std::endl;
    free(block);
}

int main(int argc, char* argv[]) {
    // 1. Вывести compile-time версию GLFW
    int major, minor, revision;
    major = GLFW_VERSION_MAJOR;
    minor = GLFW_VERSION_MINOR;
    revision = GLFW_VERSION_REVISION;
    std::cout << "Compiled against GLFW: " << major << "." << minor << "." << revision << std::endl;

    // 2. Проверить аргументы командной строки (например, --platform=x11)
    //    и установить GLFW_PLATFORM hint
    int glfw_platform = GLFW_ANY_PLATFORM;
    std::string platform = "GLFW_ANY_PLATFORM";
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--platform=") == 0) {
            platform = arg.substr(11);

            if (platform == "X11") glfw_platform = GLFW_PLATFORM_X11;
            else if (platform == "WAYLAND") glfw_platform = GLFW_PLATFORM_WAYLAND;
            else if (platform == "WIN32") glfw_platform = GLFW_PLATFORM_WIN32;
            else if (platform == "COCOA") glfw_platform = GLFW_PLATFORM_COCOA;
            else {
                std::cout << "Unknown platform. Use default platform - GLFW_ANY_PLATFORM" << std::endl;
                glfw_platform = GLFW_ANY_PLATFORM;
                platform = "GLFW_ANY_PLATFORM";
            }
        }
    }

    std::cout << "Attempting to initialize with platform: " << platform << std::endl;

    if (glfw_platform != GLFW_ANY_PLATFORM && !glfwPlatformSupported(glfw_platform)) {
        std::cout << platform << " unsupported. Use default platform - GLFW_ANY_PLATFORM" << std::endl;
        glfw_platform = GLFW_ANY_PLATFORM;
        platform = "GLFW_ANY_PLATFORM";
    }

    glfwInitHint(GLFW_PLATFORM, glfw_platform);

    // 3. Установить другие initialization hints:
    //    - На macOS: GLFW_COCOA_MENUBAR, GLFW_COCOA_CHDIR_RESOURCES
    //    - На X11: GLFW_X11_XCB_VULKAN_SURFACE
    //    - На Wayland: GLFW_WAYLAND_APP_ID, GLFW_WAYLAND_LIBDECOR
    if (platform == "X11") {
        glfwInitHint(GLFW_X11_XCB_VULKAN_SURFACE, GLFW_TRUE);
        std::cout << "Setting X11 hint: GLFW_X11_XCB_VULKAN_SURFACE = 1" << std::endl;
    } else if (platform == "WAYLAND") {
        glfwInitHint(GLFW_WAYLAND_APP_ID, GLFW_TRUE);
        glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_PREFER_LIBDECOR);
        std::cout << "Setting WAYLAND hint: GLFW_WAYLAND_APP_ID = 1 GLFW_WAYLAND_LIBDECOR = GLFW_WAYLAND_PREFER_LIBDECOR" << std::endl;
    } else if (platform == "COCOA") {
        glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_TRUE);
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_TRUE);
        std::cout << "Setting COCOA hint: GLFW_COCOA_MENUBAR = 1 GLFW_COCOA_CHDIR_RESOURCES = 1" << std::endl;
    }

    // 4. Инициализировать GLFW с кастомным аллокатором (просто логирующий)
    std::cout << "Initializing GLFW with custom allocator..." << std::endl;
    GLFWallocator allocator;
    allocator.allocate = my_malloc;
    allocator.reallocate = my_realloc;
    allocator.deallocate = my_free;
    allocator.user = NULL;

    glfwInitAllocator(&allocator);

    // 5. Вывести run-time версию GLFW
    if (!glfwInit()) {
        std::cout << "GLFW initialization error" << std::endl;
        return 1;
    }
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "Successfully initialized GLFW (run-time version: " << major << "." << minor << "." << revision << ")" << std::endl;

    // 6. Вывести используемую платформу (через glfwGetPlatform)
    glfw_platform = glfwGetPlatform();
    switch (glfw_platform) {
        case GLFW_PLATFORM_COCOA: platform = "COCOA"; break;
        case GLFW_PLATFORM_WIN32: platform = "WIN32"; break;
        case GLFW_PLATFORM_WAYLAND: platform = "WAYLAND"; break;
        case GLFW_PLATFORM_X11: platform = "X11"; break;
        default: platform = "GLFW_ANY_PLATFORM"; break;
    }
    std::cout << "Active platform: " << platform << std::endl;

    // 8. Вывести список доступных платформ (glfwPlatformSupported)
    std::cout << "Platform X11 supported: " << (glfwPlatformSupported(GLFW_PLATFORM_X11) ? "YES" : "NO") << std::endl;
    std::cout << "Platform Wayland supported: " << (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND) ? "YES" : "NO") << std::endl;
    std::cout << "Platform Win32 supported: " << (glfwPlatformSupported(GLFW_PLATFORM_WIN32) ? "YES" : "NO") << std::endl;

    // 7. Создать скрытое окно (GLFW_VISIBLE = false) для проверки
    std::cout << "Creating hidden window..." << std::endl;
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "initialization-and-termination", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Window initialization error" << std::endl;
        exit(1);
    }
    std::cout << "\tWindow created successfully" << std::endl;

    // 9. Корректно завершить работу
    std::cout << "Terminating GLFW..." << std::endl;
    glfwTerminate();

    return 0;
}
