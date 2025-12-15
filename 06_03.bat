@echo off
echo === Build solution 06.01 (header files) ===
echo.

echo Compiling rational.o...
powershell -Command "Measure-Command { g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 -c rational.cpp -o rational_06_01.o }"

echo Compiling main.o for 06.01...
powershell -Command "Measure-Command { g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 -c 06_01.cpp -o main_06_01.o }"

echo Linking 06.01...
powershell -Command "Measure-Command { g++ rational_06_01.o main_06_01.o -o program_06_01.exe }"

echo === File sizes 06.01 ===
for %%f in (rational_06_01.o) do (
    if exist "%%f" (
        for %%i in ("%%f") do echo %%f: %%~zi bytes
    ) else (
        echo %%f: not found
    )
)

for %%f in (main_06_01.o) do (
    if exist "%%f" (
        for %%i in ("%%f") do echo %%f: %%~zi bytes
    ) else (
        echo %%f: not found
    )
)

for %%f in (program_06_01.exe) do (
    if exist "%%f" (
        for %%i in ("%%f") do echo %%f: %%~zi bytes
    ) else (
        echo %%f: not found
    )
)

echo.
echo === Simple comparison method ===
echo.
echo 06.01 - single command:
echo Full build time:
powershell -Command "Measure-Command { g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 rational.cpp 06_01.cpp -o 06_01.exe }"

echo Sizes:
for %%f in (06_01.exe) do (
    if exist "%%f" (
        for %%i in ("%%f") do echo 06_01.exe: %%~zi bytes
    )
)

echo.
echo 06.02 - if adapted for modules:
echo Build time (if modules work):
powershell -Command "Measure-Command { g++ -std=c++23 -fmodules-ts rational.ixx 06_02.cpp -o 06_02.exe }" 2>nul || echo Modules not supported or files not adapted

for %%f in (06_02.exe) do (
    if exist "%%f" (
        for %%i in ("%%f") do echo 06_02.exe: %%~zi bytes
    ) else (
        echo 06_02.exe: not built
    )
)

echo.
echo === Cleanup ===
del /Q rational_06_01.o main_06_01.o program_06_01.exe 06_01.exe 06_02.exe 2>nul