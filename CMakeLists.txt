cmake_minimum_required(VERSION 3.22)
project(internship)

set(CMAKE_CXX_STANDARD 17)

add_executable(internship main.cpp
        R3DS/Model.h R3DS/Model.cpp
        R3DS/ObjIO/ObjIO.h R3DS/ObjIO/ObjIO.cpp
        R3DS/Deform/Deform.h
        R3DS/Rendering/Camera.h R3DS/Rendering/Camera.cpp
        R3DS/Rendering/OpenGL/Mesh.h R3DS/Rendering/OpenGL/Mesh.cpp
        R3DS/Rendering/OpenGL/Shader.h R3DS/Rendering/OpenGL/Shader.cpp
        window.h window.cpp
        R3DS/Deform/BoundingBox.h R3DS/Deform/BoundingBox.cpp
        R3DS/Deform/SpatialViews/UniformGrid.h R3DS/Deform/SpatialViews/UniformGrid.cpp
        R3DS/Deform/SpatialViews/KDTree3D.h R3DS/Deform/SpatialViews/KDTree3D.cpp
        R3DS/Rendering/Conversion.h R3DS/Rendering/Conversion.cpp
        R3DS/ObjIO/ReadHelpers.h R3DS/ObjIO/ReadHelpers.cpp
        R3DS/ObjIO/WriteHelpers.h R3DS/ObjIO/WriteHelpers.cpp)

target_link_libraries(internship opengl32.lib)
target_link_libraries(internship glew32.lib)
target_link_libraries(internship glfw3.lib)
