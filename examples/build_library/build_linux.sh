#!/bin/bash

GCC=g++

baseDir=$(pushd $(dirname "$0") >/dev/null; pwd; popd >/dev/null)

mkdir "$baseDir/build/" 2>/dev/null
mkdir "$baseDir/build/linux/" 2>/dev/null

$GCC -shared -fPIC -g "$baseDir/AsyncUDP.cpp" "$baseDir"/../../src/*.cpp "$baseDir"/../../src/*.c "$baseDir"/../../src/nanopb/*.c "-I$baseDir/" "-I$baseDir/../../src/" -o "$baseDir/build/linux/libugot_broadcast_message.so" -lpthread -D__ASYNC_UDP_IMPL_LINUX__
$GCC -g "$baseDir/demo.cpp" "-I$baseDir/" "-I$baseDir/../../src/" -o "$baseDir/build/linux/ugot_broadcast_message_demo" -lugot_broadcast_message -L "$baseDir/build/linux" -D__ASYNC_UDP_IMPL_LINUX__
