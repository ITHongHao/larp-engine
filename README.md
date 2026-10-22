# larp-engine

Currently, the project should work only on Windows VSCode. Additional Details are in this google document: https://docs.google.com/document/d/1UKn2X3P2jFwoNOQF_8iubNSY57hThXdCsAAMWXAo3TM/edit?usp=sharing <br>

<u>VSCode Extensions and Additional Downloads:</u>
1. C/C++ Extension Pack by microsoft.com
2. CMake by twxs
3. With Python, ```pip install jinja2```. This is for GLAD2 to work.

<u>Build and Run in Windows:</u>
1. Go to https://visualstudio.microsoft.com/downloads/?q=build+tools and download "Build Tools for Visual Studio 2026" and within the program download "Desktop development with C++." Get the additional downloads when downloading the desktop development tool:
- MSVC Build Tools for x64/x86 (Latest)
- Windows 11 SDK
- C++ Cmake Tools for Windows
- C++ Clang tools for Windows
2. (Optional?) If CMake is unavailable in the VSCode window for Windows, launch Visual Studio Built Tools 2026 in Visual Studio Installer and go to the location of the folder containing the project in the terminal.
Example: ```cd C:\Users\[user]\project```, ```code .```
3. ```cmake -B bin```
4. Repeat this if code or packages have been edited: ```cmake --build bin```
5. Run code: ```.\bin\Debug\LarpEngine.exe```