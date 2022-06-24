cmake_minimum_required(VERSION 3.22)
project(internship)

set(CMAKE_CXX_STANDARD 17)

add_executable(internship main.cpp
        Internship/Model.h Internship/Model.cpp
        Internship/ObjIO/ObjIO.h Internship/ObjIO/ObjIO.cpp
        Internship/Deform/Deform.h
        Internship/Rendering/Camera.h Internship/Rendering/Camera.cpp
        Internship/Rendering/OpenGL/Mesh.h Internship/Rendering/OpenGL/Mesh.cpp
        Internship/Rendering/OpenGL/Shader.h Internship/Rendering/OpenGL/Shader.cpp
        Internship/Demo/Window.h Internship/Demo/Window.cpp
        Internship/Deform/BoundingBox.h Internship/Deform/BoundingBox.cpp
        Internship/Deform/SpatialViews/UniformGrid.h Internship/Deform/SpatialViews/UniformGrid.cpp
        Internship/Deform/SpatialViews/KDTree3D.h Internship/Deform/SpatialViews/KDTree3D.cpp
        Internship/Rendering/Conversion.h Internship/Rendering/Conversion.cpp
        Internship/ObjIO/ReadHelpers.h Internship/ObjIO/ReadHelpers.cpp
        Internship/ObjIO/WriteHelpers.h Internship/ObjIO/WriteHelpers.cpp)

target_link_libraries(internship opengl32.lib)
target_link_libraries(internship glew32.lib)
target_link_libraries(internship glfw3.lib)
