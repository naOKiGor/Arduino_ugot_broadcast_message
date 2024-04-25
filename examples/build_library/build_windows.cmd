@echo off

set "MINGW64_PATH=E:\Tools\mingw64\bin"
set "PATH=%MINGW64_PATH%;%PATH%"

set "baseDir=%~dp0"

mkdir "%baseDir%\build" 2>nul
mkdir "%baseDir%\build\win" 2>nul

g++.exe -shared -g "%baseDir%AsyncUDP.cpp" "%baseDir%..\..\src\*.cpp" "%baseDir%..\..\src\*.c" "%baseDir%..\..\src\nanopb\*.c" -I%baseDir% -I%baseDir%..\..\src\ -o "%baseDir%build\win\libugot_broadcast_message.dll" -static-libgcc -static-libstdc++ -lws2_32 -D__ASYNC_UDP_IMPL_WIN__ -D__WIN_EXPORT_DLL__
g++.exe -g "%baseDir%demo.cpp" "%baseDir%build\win\libugot_broadcast_message.dll" -I%baseDir%  -I%baseDir%..\..\src\ -o "%baseDir%build\win\ugot_broadcast_message_demo.exe" -static-libgcc -static-libstdc++ -D__ASYNC_UDP_IMPL_WIN__ -D__WIN_IMPORT_DLL__
