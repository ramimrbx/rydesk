<div align="center">

&#x20; <h1>🚀 RyDesk</h1>

&#x20; <p><b>A Blazing-Fast C++ Scaffolding \& Desktop Application Framework for Windows</b></p>

</div>



\## 📌 Overview

\*\*RyDesk\*\* is a lightweight, modern CLI tool and framework designed to eliminate the complexity of C++ Windows desktop development. Say goodbye to messy `CMakeLists.txt` or complex `Makefiles`. RyDesk scaffolds a complete, ready-to-run Windows desktop application with a built-in C++ build engine, modern UI styling, and a clean MVC architecture in seconds!



\## ✨ Key Features

\* 🚀 \*\*Instant Scaffolding:\*\* Generate a fully structured C++ desktop project with a single command.

\* 🎨 \*\*Modern Windows UI:\*\* Out-of-the-box support for Windows 11 DWM APIs (Immersive Dark Mode, Rounded Corners, System Backdrops).

\* 🏗️ \*\*MVC Architecture:\*\* Pre-configured Model-View-Controller pattern to keep your enterprise codebase clean and scalable.

\* ⚙️ \*\*Custom C++ Build Engine:\*\* No external build tools required. Projects are compiled using a native `build.cpp` engine perfectly hidden in the `build/engine/` directory.

\* 🖼️ \*\*Asset \& Icon Management:\*\* Automatic handling of `.rc` resource files and custom `.exe` icons.



\## 🛠️ Prerequisites

Before you begin, ensure you have the following installed:

\* \*\*Windows OS\*\* (Windows 10/11 recommended for DWM styling)

\* \*\*MinGW-w64 (GCC)\*\* with `g++` and `windres` available in your system PATH.

\* C++20 supported compiler.



\## 🚀 Getting Started



\### 1. Build the RyDesk CLI

Clone the repository and run the setup script to build the main CLI tool:

```bash

git clone \[https://github.com/your-username/rydesk.git](https://github.com/your-username/rydesk.git)

cd rydesk

.\\build.bat

(This will generate the rydesk.exe CLI tool with its embedded icon).



2\. Create a New Project

Use the CLI to scaffold your new desktop application:



Bash

rydesk new Calculator --package com.basic.calculator

3\. Run Your Application

Navigate to your newly created project folder and start the engine:



Bash

cd Calculator



\# To compile and run in Debug mode:

.\\rydesk.bat run



\# To compile a highly optimized Production binary:

.\\rydesk.bat build

📂 Generated Directory Structure

RyDesk automatically organizes your project following enterprise standards:



Plaintext

📦 ProjectRoot

&#x20;┣ 📂 build/                # All compiled binaries and the build engine reside here

&#x20;┃ ┣ 📂 debug/

&#x20;┃ ┣ 📂 release/

&#x20;┃ ┗ 📂 engine/             # The custom build.exe engine

&#x20;┣ 📂 src/main/

&#x20;┃ ┣ 📂 cpp/com/basic/calculator/

&#x20;┃ ┃ ┣ 📂 engine/           # Controllers \& Logic (MVC)

&#x20;┃ ┃ ┣ 📂 presentation/     # UI Components \& Screens

&#x20;┃ ┃ ┗ 📜 Main.cpp          # Entry Point

&#x20;┃ ┗ 📂 resources/

&#x20;┃   ┗ 📂 assets/images/    # Default icons and assets

&#x20;┣ 📜 build.cpp             # Project specific build logic

&#x20;┣ 📜 rydesk.bat            # Command runner

&#x20;┣ 📜 resource.rc           # Auto-generated resource file

&#x20;┗ 📜 app.env               # Environment variables

🤝 Contributing

Contributions, issues, and feature requests are welcome!

Feel free to check the issues page.



📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

