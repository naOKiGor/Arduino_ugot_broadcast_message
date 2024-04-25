#!/bin/bash

GCC=/usr/bin/x86_64-w64-mingw32-g++

baseDir=$(pushd $(dirname "$0") >/dev/null; pwd; popd >/dev/null)

mkdir "$baseDir/build/" 2>/dev/null
mkdir "$baseDir/build/win/" 2>/dev/null

$GCC -shared -g "$baseDir/AsyncUDP.cpp" "$baseDir"/../../src/*.cpp "$baseDir"/../../src/*.c "$baseDir"/../../src/nanopb/*.c "-I$baseDir/" "-I$baseDir/../../src/" -o "$baseDir/build/win/libugot_broadcast_message.dll" -static-libgcc -static-libstdc++ -lws2_32 -lpthread -D__ASYNC_UDP_IMPL_WIN__ -D__WIN_EXPORT_DLL__
$GCC -g "$baseDir/demo.cpp" "$baseDir/build/win/libugot_broadcast_message.dll" "-I$baseDir/" "-I$baseDir/../../src/" -o "$baseDir/build/win/ugot_broadcast_message_demo.exe" -static-libgcc -static-libstdc++ -D__ASYNC_UDP_IMPL_WIN__ -D__WIN_IMPORT_DLL__