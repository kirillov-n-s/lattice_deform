cmake_minimum_required(VERSION 3.22)
project(internship)

set(CMAKE_CXX_STANDARD 17)

add_executable(internship main.cpp
        Lattice/Model.h Lattice/Model.cpp
        Lattice/ObjIO/ObjIO.h Lattice/ObjIO/ObjIO.cpp
        Lattice/Deform/Deform.h
        Lattice/Rendering/Camera.h Lattice/Rendering/Camera.cpp
        Lattice/Rendering/OpenGL/Mesh.h Lattice/Rendering/OpenGL/Mesh.cpp
        Lattice/Rendering/OpenGL/Shader.h Lattice/Rendering/OpenGL/Shader.cpp
        Lattice/Demo/Window.h Lattice/Demo/Window.cpp
        Lattice/Deform/BoundingBox.h Lattice/Deform/BoundingBox.cpp
        Lattice/Deform/SpatialViews/UniformGrid.h Lattice/Deform/SpatialViews/UniformGrid.cpp
        Lattice/Deform/SpatialViews/KDTree3D.h Lattice/Deform/SpatialViews/KDTree3D.cpp
        Lattice/Rendering/Conversion.h Lattice/Rendering/Conversion.cpp
        Lattice/ObjIO/ReadHelpers.h Lattice/ObjIO/ReadHelpers.cpp
        Lattice/ObjIO/WriteHelpers.h Lattice/ObjIO/WriteHelpers.cpp)

target_link_libraries(internship opengl32.lib)
target_link_libraries(internship glew32.lib)
target_link_libraries(internship glfw3.lib)
