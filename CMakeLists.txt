cmake_minimum_required(VERSION 3.12)
project(MiniMShadow)

set(CMAKE_CXX_STANDARD 14)
include_directories("/home/ace/AceDev/C++/library_read/mshadow")


add_executable(MiniMShadow main.cpp Allocator/allocator.h Basic/logging.h Basic/base.h Basic/shape.h Expression/expression.h)