cmake_minimum_required(VERSION 3.15)
project(Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(plugin_api INTERFACE)
target_include_directories(plugin_api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(calc 
    src/main.cpp
    src/PluginManager.cpp
    src/Lexer.cpp
    src/Parser.cpp
 )
target_link_libraries(calc PRIVATE plugin_api)

add_library(funcsin SHARED plugins/funcsin/sin_plugin.cpp )
target_link_libraries(funcsin PRIVATE plugin_api)
set_target_properties(funcsin PROPERTIES PREFIX "" SUFFIX ".dll")

add_library(funcln SHARED plugins/funcln/ln_plugin.cpp )
target_link_libraries(funcln PRIVATE plugin_api)
set_target_properties(funcln PROPERTIES PREFIX "" SUFFIX ".dll")

add_library(funcbad SHARED plugins/funcbad/bad_plugin.cpp )
target_link_libraries(funcbad PRIVATE plugin_api)
set_target_properties(funcbad PROPERTIES PREFIX "" SUFFIX ".dll")

add_custom_command(TARGET calc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:calc>/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:funcsin> $<TARGET_FILE_DIR:calc>/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:funcln> $<TARGET_FILE_DIR:calc>/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:funcbad> $<TARGET_FILE_DIR:calc>/plugins/
    COMMENT "Копирование плагинов в папку plugins..."
)
