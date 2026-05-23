#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void writeToFile(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << content;
        file.close();
        std::cout << "[RyDesk] Created file: " << path.string() << "\n";
    }
}

void makeDir(const fs::path& path) {
    if (!fs::exists(path)) {
        fs::create_directories(path);
        std::cout << "[RyDesk] Created directory: " << path.string() << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "new") {
        std::cerr << "Usage: rydesk new <project_name> [--package <tld.company.project>]\n";
        return 1;
    }

    std::string projectName = argv[2];
    std::string packageName = "";

    if (argc >= 5 && std::string(argv[3]) == "--package") {
        packageName = argv[4];
    } else {
        std::cout << "Let's set up your project's package structure.\n";
        while (true) {
            std::cout << "> Enter package name: ";
            std::cin >> packageName;
            std::vector<std::string> segments = splitString(packageName, '.');
            if (segments.size() >= 2) break;
            
            std::cerr << "\n[!] Oops! The package name '" << packageName << "' is incomplete.\n";
            std::cerr << "    RyDesk requires a domain-style format with at least two segments separated by a dot (.).\n";
            std::cerr << "    Example: com.calculator or rightitech.ritems\n";
            std::cerr << "    Please try again.\n\n";
        }
    }

    std::vector<std::string> pkgParts = splitString(packageName, '.');
    std::string packagePath = "";
    for (const auto& p : pkgParts) {
        packagePath += p + "/";
    }

    fs::path rootPath = fs::current_path() / projectName;
    fs::path baseCppPath = rootPath / "src/main/cpp" / packagePath;
    fs::path baseResPath = rootPath / "src/main/resources";

    // 1. Create Directories
    makeDir(rootPath / "build/debug");
    makeDir(rootPath / "build/release");
    makeDir(rootPath / "build/engine");
    makeDir(rootPath / "rydesk/vendor/include");
    makeDir(rootPath / "rydesk/vendor/libraries");
    
    makeDir(baseCppPath / "engine");
    makeDir(baseCppPath / "presentation/components");
    makeDir(baseCppPath / "presentation/screens");
    
    makeDir(baseCppPath / "include/engine");
    makeDir(baseCppPath / "include/presentation/components");
    makeDir(baseCppPath / "include/presentation/screens");
    
    makeDir(baseResPath / "assets/fonts");
    makeDir(baseResPath / "assets/images");

    // ==========================================
    // Icon Configuration Logic (Updated Path)
    // ==========================================
    fs::path sourceIcon = fs::current_path() / "resources/assets/images/icon.ico";
    if (fs::exists(sourceIcon)) {
        fs::copy_file(sourceIcon, baseResPath / "assets/images/icon.ico", fs::copy_options::overwrite_existing);
        std::string rcContent = "MAINICON ICON \"src/main/resources/assets/images/icon.ico\"\n";
        writeToFile(rootPath / "resource.rc", rcContent);
        std::cout << "[RyDesk] Attached custom icon (icon.ico) to the new project.\n";
    } else {
        std::cout << "[RyDesk] Notice: 'resources/assets/images/icon.ico' not found. Default Windows icon will be used.\n";
    }

    // 2. Generate Configuration and Script Files
    std::string envContent = "APP_NAME=" + projectName + "\nENVIRONMENT=dev\n";
    writeToFile(rootPath / "app.env", envContent);

    std::string batScript = 
        "@echo off\n"
        "if not exist build\\engine\\build.exe (\n"
        "    g++ build.cpp -std=c++20 -o build\\engine\\build.exe\n"
        ")\n"
        "if \"%1\"==\"build\" (\n"
        "    .\\build\\engine\\build.exe build\n"
        ") else if \"%1\"==\"run\" (\n"
        "    .\\build\\engine\\build.exe run\n"
        ") else (\n"
        "    echo Usage: rydesk.bat [build^|run]\n"
        ")\n";
    writeToFile(rootPath / "rydesk.bat", batScript);

    std::string buildCpp = 
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <cstdlib>\n"
        "#include <filesystem>\n\n"
        "const std::string PROGRAM_NAME = \"" + projectName + "\";\n"
        "const std::string VERSION = \"v1.0.0\";\n\n"
        "int main(int argc, char* argv[]) {\n"
        "    std::string action = (argc > 1) ? argv[1] : \"\";\n"
        "    std::string srcFiles = \"src/main/cpp/" + packagePath + "Main.cpp \" \n"
        "                           \"src/main/cpp/" + packagePath + "engine/CounterController.cpp \" \n"
        "                           \"src/main/cpp/" + packagePath + "presentation/components/Button.cpp \" \n"
        "                           \"src/main/cpp/" + packagePath + "presentation/screens/MainScreen.cpp \";\n\n"
        "    std::string includeFlag = \"-I src/main/cpp/" + packagePath + "include\";\n"
        "    std::string linkers = \"-ldwmapi -lgdi32\";\n\n"
        "    std::string resourceObj = \"\";\n"
        "    if (std::filesystem::exists(\"resource.rc\")) {\n"
        "        std::system(\"windres resource.rc -o build/engine/resource.o\");\n"
        "        resourceObj = \" build/engine/resource.o \";\n"
        "    }\n\n"
        "    if (action == \"run\") {\n"
        "        std::string outPath = \"build/debug/\" + PROGRAM_NAME + \"_debug.exe\";\n"
        "        std::string runCmd = \".\\\\build\\\\debug\\\\\" + PROGRAM_NAME + \"_debug.exe\";\n"
        "        std::cout << \"[RyDesk] Compiling Debug GUI (C++20)...\\n\";\n"
        "        std::string cmd = \"g++ -std=c++20 \" + srcFiles + resourceObj + includeFlag + \" -mwindows -g -o \" + outPath + \" \" + linkers;\n"
        "        if (std::system(cmd.c_str()) == 0) {\n"
        "            std::cout << \"[RyDesk] Launching Application...\\n\";\n"
        "            std::system(runCmd.c_str());\n"
        "        } else {\n"
        "            std::cerr << \"[RyDesk] Build failed.\\n\";\n"
        "        }\n"
        "    } else if (action == \"build\") {\n"
        "        std::string outPath = \"build/release/\" + PROGRAM_NAME + \"_\" + VERSION + \".exe\";\n"
        "        std::cout << \"[RyDesk] Compiling Production GUI (C++20)...\\n\";\n"
        "        std::string cmd = \"g++ -std=c++20 \" + srcFiles + resourceObj + includeFlag + \" -mwindows -O3 -DNDEBUG -o \" + outPath + \" \" + linkers;\n"
        "        if (std::system(cmd.c_str()) == 0) {\n"
        "            std::cout << \"[RyDesk] Success! Production binary: \" << outPath << \"\\n\";\n"
        "        } else {\n"
        "            std::cerr << \"[RyDesk] Build failed.\\n\";\n"
        "        }\n"
        "    } else {\n"
        "        std::cout << \"Usage: rydesk.bat [build|run]\\n\";\n"
        "    }\n"
        "    return 0;\n"
        "}\n";
    writeToFile(rootPath / "build.cpp", buildCpp);

    // ==========================================
    // MVC Architecture
    // ==========================================

    std::string engineH = 
        "#pragma once\n"
        "class CounterController {\n"
        "private:\n"
        "    int count;\n"
        "public:\n"
        "    CounterController();\n"
        "    void increment();\n"
        "    int getCount() const;\n"
        "};\n";
    writeToFile(baseCppPath / "include/engine/CounterController.h", engineH);

    std::string engineCpp = 
        "#include \"engine/CounterController.h\"\n"
        "CounterController::CounterController() : count(0) {}\n"
        "void CounterController::increment() { count++; }\n"
        "int CounterController::getCount() const { return count; }\n";
    writeToFile(baseCppPath / "engine/CounterController.cpp", engineCpp);

    std::string btnH = 
        "#pragma once\n"
        "#include <windows.h>\n"
        "class Button {\n"
        "private:\n"
        "    HWND hwnd;\n"
        "public:\n"
        "    Button();\n"
        "    void create(HWND parent, int id, const char* text, int x, int y, int w, int h);\n"
        "};\n";
    writeToFile(baseCppPath / "include/presentation/components/Button.h", btnH);

    std::string btnCpp = 
        "#include \"presentation/components/Button.h\"\n"
        "Button::Button() : hwnd(NULL) {}\n"
        "void Button::create(HWND parent, int id, const char* text, int x, int y, int w, int h) {\n"
        "    hwnd = CreateWindow(\"BUTTON\", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,\n"
        "                        x, y, w, h, parent, (HMENU)(INT_PTR)id,\n"
        "                        (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);\n"
        "    HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, \"Segoe UI Variable\");\n"
        "    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);\n"
        "}\n";
    writeToFile(baseCppPath / "presentation/components/Button.cpp", btnCpp);

    std::string screenH = 
        "#pragma once\n"
        "#include <windows.h>\n"
        "#include <dwmapi.h>\n"
        "#include <string>\n"
        "#include \"engine/CounterController.h\"\n"
        "#include \"presentation/components/Button.h\"\n\n"
        "#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE\n"
        "#define DWMWA_USE_IMMERSIVE_DARK_MODE 20\n"
        "#endif\n"
        "#ifndef DWMWA_WINDOW_CORNER_PREFERENCE\n"
        "#define DWMWA_WINDOW_CORNER_PREFERENCE 33\n"
        "#endif\n"
        "#ifndef DWMWA_SYSTEMBACKDROP_TYPE\n"
        "#define DWMWA_SYSTEMBACKDROP_TYPE 38\n"
        "#endif\n\n"
        "class MainScreen {\n"
        "private:\n"
        "    HWND hwnd;\n"
        "    HWND hLabel;\n"
        "    Button clickButton;\n"
        "    CounterController counter;\n"
        "    static MainScreen* instance;\n"
        "    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);\n"
        "public:\n"
        "    MainScreen();\n"
        "    void render(const std::string& title);\n"
        "    void applyModernStyling();\n"
        "};\n";
    writeToFile(baseCppPath / "include/presentation/screens/MainScreen.h", screenH);

    std::string screenCpp = 
        "#include \"presentation/screens/MainScreen.h\"\n"
        "#include <string>\n\n"
        "MainScreen* MainScreen::instance = nullptr;\n\n"
        "MainScreen::MainScreen() : hwnd(NULL), hLabel(NULL) {\n"
        "    instance = this;\n"
        "}\n\n"
        "void MainScreen::applyModernStyling() {\n"
        "    BOOL dark = TRUE;\n"
        "    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));\n"
        "    int corners = 2;\n"
        "    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corners, sizeof(corners));\n"
        "    int backdrop = 2;\n"
        "    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(backdrop));\n"
        "}\n\n"
        "LRESULT CALLBACK MainScreen::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {\n"
        "    if (uMsg == WM_DESTROY) {\n"
        "        PostQuitMessage(0);\n"
        "        return 0;\n"
        "    }\n"
        "    if (uMsg == WM_CTLCOLORSTATIC) {\n"
        "        HDC hdcStatic = (HDC)wParam;\n"
        "        SetTextColor(hdcStatic, RGB(255, 255, 255));\n"
        "        SetBkMode(hdcStatic, TRANSPARENT);\n"
        "        return (LRESULT)GetStockObject(NULL_BRUSH);\n"
        "    }\n"
        "    if (uMsg == WM_COMMAND) {\n"
        "        if (LOWORD(wParam) == 1) {\n"
        "            if (instance) {\n"
        "                instance->counter.increment();\n"
        "                std::string newText = \"Current Count: \" + std::to_string(instance->counter.getCount());\n"
        "                SetWindowText(instance->hLabel, newText.c_str());\n"
        "                InvalidateRect(hwnd, NULL, TRUE);\n"
        "            }\n"
        "        }\n"
        "        return 0;\n"
        "    }\n"
        "    return DefWindowProc(hwnd, uMsg, wParam, lParam);\n"
        "}\n\n"
        "void MainScreen::render(const std::string& title) {\n"
        "    HINSTANCE hInstance = GetModuleHandle(NULL);\n"
        "    const char* CLASS_NAME = \"RyDeskWindowClass\";\n\n"
        "    WNDCLASS wc = {0};\n"
        "    wc.lpfnWndProc = WindowProc;\n"
        "    wc.hInstance = hInstance;\n"
        "    wc.lpszClassName = CLASS_NAME;\n"
        "    wc.hbrBackground = CreateSolidBrush(RGB(32, 32, 32));\n"
        "    wc.hIcon = LoadIcon(hInstance, \"MAINICON\");\n"
        "    RegisterClass(&wc);\n\n"
        "    hwnd = CreateWindowEx(0, CLASS_NAME, title.c_str(), WS_OVERLAPPEDWINDOW,\n"
        "        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);\n\n"
        "    if (hwnd == NULL) return;\n\n"
        "    applyModernStyling();\n\n"
        "    hLabel = CreateWindow(\"STATIC\", \"Current Count: 0\", WS_VISIBLE | WS_CHILD | SS_CENTER,\n"
        "        50, 40, 280, 25, hwnd, NULL, hInstance, NULL);\n"
        "    HFONT hFont = CreateFont(20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, \"Segoe UI Variable\");\n"
        "    SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);\n\n"
        "    clickButton.create(hwnd, 1, \"Click Here\", 120, 100, 140, 40);\n\n"
        "    ShowWindow(hwnd, SW_SHOW);\n"
        "    UpdateWindow(hwnd);\n\n"
        "    MSG msg = {0};\n"
        "    while (GetMessage(&msg, NULL, 0, 0)) {\n"
        "        TranslateMessage(&msg);\n"
        "        DispatchMessage(&msg);\n"
        "    }\n"
        "}\n";
    writeToFile(baseCppPath / "presentation/screens/MainScreen.cpp", screenCpp);

    std::string mainCpp = 
        "#include <windows.h>\n"
        "#include \"presentation/screens/MainScreen.h\"\n\n"
        "int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {\n"
        "    MainScreen app;\n"
        "    app.render(\"" + projectName + " - Powered by RyDesk\");\n"
        "    return 0;\n"
        "}\n";
    writeToFile(baseCppPath / "Main.cpp", mainCpp);

    std::cout << "\n[RyDesk] Success! Your project is ready. The build engine is neatly organized in the 'build/engine/' folder.\n";
    return 0;
}