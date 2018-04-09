@echo off

:start
if exist src goto changetobin
if exist .\CMakeLists.txt xcopy /q /i /e /y .\CMakeLists.txt .\bin\CMakeLists.txt
if exist .\conanfile.txt xcopy /q /i /e /y .\conanfile.txt .\bin\conanfile.txt

:conan
conan install --build missing
goto cmake

:cmake
cmake .. -G "Visual Studio 15 2017 Win64"
goto end

:changetobin
cd bin
goto start

:end
if exist bin goto quit
cd ..
goto quit

:quit
echo.
echo.
echo INFO: Conan and CMake ran successfully. Open up your solution file (listed below) to open your project.
echo.
dir /b *.sln