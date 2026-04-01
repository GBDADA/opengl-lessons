## Задание: "Мониторинг системы инициализации"

Напиши программу, которая:
1. Выводит информацию о версии GLFW (compile-time и run-time)
2. Позволяет выбрать платформу (X11/Wayland/macOS/Win32) через аргумент командной строки
3. Использует разные initialization hints в зависимости от платформы
4. Выводит информацию о том, что получилось, и корректно завершает работу

### Требования к программе:
// Псевдо-структура того, что должно быть:

int main(int argc, char** argv) {
    // 1. Вывести compile-time версию GLFW
    
    // 2. Проверить аргументы командной строки (например, --platform=x11)
    //    и установить GLFW_PLATFORM hint
    
    // 3. Установить другие initialization hints:
    //    - На macOS: GLFW_COCOA_MENUBAR, GLFW_COCOA_CHDIR_RESOURCES
    //    - На X11: GLFW_X11_XCB_VULKAN_SURFACE
    //    - На Wayland: GLFW_WAYLAND_APP_ID, GLFW_WAYLAND_LIBDECOR
    
    // 4. Инициализировать GLFW с кастомным аллокатором (просто логирующий)
    
    // 5. Вывести run-time версию GLFW
    
    // 6. Вывести используемую платформу (через glfwGetPlatform)
    
    // 7. Создать скрытое окно (GLFW_VISIBLE = false) для проверки
    
    // 8. Вывести список доступных платформ (glfwPlatformSupported)
    
    // 9. Корректно завершить работу
    
    return 0;
}

### Ожидаемый вывод программы:

Compiled against GLFW: 3.4.0
Attempting to initialize with platform: X11
Setting X11 hint: GLFW_X11_XCB_VULKAN_SURFACE = 1
Initializing GLFW with custom allocator...
  Allocator: malloc(512) called
  Allocator: malloc(128) called
Successfully initialized GLFW (run-time version: 3.4.0)
Active platform: X11
Platform X11 supported: YES
Platform Wayland supported: YES
Platform Win32 supported: NO
Creating hidden window...
  Window created successfully
Terminating GLFW...
  Allocator: free(0x...) called
  Allocator: free(0x...) called
Done.