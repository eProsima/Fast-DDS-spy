#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

void write_console_input(HANDLE hInput, const std::string& text)
{
    std::vector<INPUT_RECORD> records;
    for (char ch : text)
    {
        INPUT_RECORD ir = {};
        ir.EventType = KEY_EVENT;
        ir.Event.KeyEvent.bKeyDown = TRUE;
        ir.Event.KeyEvent.uChar.AsciiChar = ch;
        ir.Event.KeyEvent.wVirtualKeyCode = VkKeyScanA(ch);
        ir.Event.KeyEvent.wRepeatCount = 1;
        ir.Event.KeyEvent.dwControlKeyState = 0;

        records.push_back(ir);

        ir.Event.KeyEvent.bKeyDown = FALSE; // key up
        records.push_back(ir);
    }

    DWORD written = 0;
    WriteConsoleInputA(hInput, records.data(), static_cast<DWORD>(records.size()), &written);
}

int main()
{
    // Lanzar fastddsspy con consola nueva
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL result = CreateProcessA(
        nullptr,
        (LPSTR)"fastddsspy.exe",
        nullptr, nullptr, TRUE,
        CREATE_NEW_CONSOLE,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (!result)
    {
        std::cerr << "Failed to launch fastddsspy.exe" << std::endl;
        return 1;
    }

    // Esperar a que la consola esté lista
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Conectarse a la consola del proceso hijo
    if (!AttachConsole(pi.dwProcessId))
    {
        std::cerr << "Failed to attach to child console" << std::endl;
        return 1;
    }

    // Obtener handle de entrada estándar de esa consola
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    if (hInput == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Invalid input handle" << std::endl;
        return 1;
    }

    // Enviar texto simulado
    write_console_input(hInput, "show all\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    write_console_input(hInput, "exit\n");

    // Esperar a que termine
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
