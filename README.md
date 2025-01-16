# MCProject

MC Project. Team name: Echipa

This is the handout for setting up dependencies.

INSTALLING LIBRARIES:

To use vcpkg and the necessary libraries, the following steps were performed:

- Clone the vcpkg repository from: https://github.com/microsoft/vcpkg or
- Open the command prompt.
- Run the following command: .\vcpkg\bootstrap-vcpkg.bat.
- For using the Crow routes, follow these steps:

- Run the following command: vcpkg install crow:x64-windows.
  If the command is not recognized, try the following: .\vcpkg.exe install crow:x64-windows.
  For using HTTP routes, the CPR library is used, and the following steps should be taken:

- Run the following command: vcpkg install cpr:x64-windows.
  If the command is not recognized, try the following: .\vcpkg.exe install cpr:x64-windows.
  For using SQL databases (SQLite), the following steps should be taken:

- Run the following command: vcpkg install sqlite-orm:x64-windows.
  If the command is not recognized, try the following: .\vcpkg.exe install sqlite-orm:x64-windows.
  To use libraries in Visual Studio, run the following command: vcpkg integrate install.

For using websockets, follow these steps:

- Run the following command: vcpkg install ixwebsocket.
  If the command is not recognized, try the following: .\vcpkg install ixwebsocket.
  To create interfaces, use Qt, and install Qt version 6.8.0 from Qt Creator.

SETTING UP DEPENDENCIES:

For Qt elements, the following dependencies should be set:

- Add the include directory for Qt:
  Property > Configuration Properties > VC++ Directories > Include Directories > C:\Qt6.8\6.8.0\msvc2022_64\include

- Add the library directory for Qt:
  Property > Configuration Properties > VC++ Directories > Library Directories > C:\Qt6.8\6.8.0\msvc2022_64\lib

- Add the path to the Qt folder for the application version:
  Property > Configuration Properties > Qt Project Settings > Qt Installation > QT 6_8

- Add the modules following the path:
  Property > Configuration Properties > Qt Project Settings > Qt Modules, and select: core, gui, quick, widgets.

- Link the library by adding the path to the library directory in the Qt folder:
  Property > Linker > General > Additional Library Directories > C:\Qt6.8\6.8.0\msvc2022_64\lib

- Add the additional dependencies:
  Property > Linker > General > Additional Dependencies > Qt6Core.lib; Qt6Widgets.lib; Qt6Gui.lib; Qt6Cored.lib

To add a Qt version, follow these steps:

- Go to the main menu in Visual Studio: Qt VS Tools > Qt Versions, and click Add.
- Select the main directory of the installed Qt version: C:\Qt6.8\6.8.0.

For websocket elements, the following dependencies should be set:

These dependencies differ for Release and Debug configurations.

RELEASE:

- Add the include directory for websocket: Property > Configuration Properties > VC++ Directories > Include Directories > C:\Computers\vcpkg-2024.12.16\vcpkg-2024.12.16\installed\x64-windows\include

- Add the library directory for websocket: Property > Configuration Properties > VC++ Directories > Library Directories > C:\Computers\vcpkg-2024.12.16\vcpkg-2024.12.16\installed\x64-windows\lib

DEBUG:

- Add the include directory for websocket: Property > Configuration Properties > VC++ Directories > Include Directories > C:\Computers\vcpkg-2024.12.16\vcpkg-2024.12.16\installed\x64-windows\include

- Add the library directory for websocket: Property > Configuration Properties > VC++ Directories > Library Directories > C:\Computers\vcpkg-2024.12.16\vcpkg-2024.12.16\installed\x64-windows\debug\lib

- Add the additional dependencies:
  Property > Linker > General > Additional Dependencies > ixwebsocket.lib; mbedtls.lib; mbedx509.lib; mbedcrypto.lib; Crypt32.lib; bcrypt.lib
