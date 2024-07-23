cmake_minimum_required(VERSION 3.10)
project(GTKTutorial)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(GTK3_INCLUDE_DIRS "C:/msys64/mingw64/include/")
set(GTK3_LIBRARY_DIRS "C:/msys64/mingw64/lib")
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(SettingsTransfer SettingsTransfer.cpp)
#set_target_properties(SettingsTransfer PROPERTIES WIN32_EXECUTABLE true)  # If you want console to run with your .exe
target_link_libraries(SettingsTransfer ${GTK4_LIBRARIES}) #i made some changes to test out git