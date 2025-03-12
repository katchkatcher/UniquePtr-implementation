cmake_minimum_required(VERSION 3.10)
project(MyUniquePtr)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем исполняемый файл из example/
add_executable(example_MyUniquePtr example/example_MyUniquePtr.cpp)

# Указываем путь к заголовочным файлам
target_include_directories(example_MyUniquePtr PRIVATE include)
