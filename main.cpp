#include <iostream>
#include <chrono>
#include "Lattice/ObjIO/ObjIO.h"
#include "Lattice/Deform/Deform.h"
#include "Lattice/Demo/Window.h"
#include "Lattice/Rendering/OpenGL/Mesh.h"
#include "Lattice/Rendering/OpenGL/Shader.h"

using timer = std::chrono::system_clock;
using units = std::chrono::milliseconds;
std::string unit_str = "ms";

int main()
{
    auto path = std::string(R"(C:\Users\user\Desktop\University\!R3DS\internship\data\)");
    auto meshes = path + "meshes\\";
    auto shaders = path + "shaders\\";

    auto source_path = meshes + "jacket\\jacket.obj";
    auto rest_path = meshes + "jacket\\base.obj";
    auto deformed_path = meshes + "jacket\\FemaleBody.obj";
    auto result_path = meshes + "result\\jacket_female.obj";
    auto houdini_path = meshes + "example\\jacket_female.obj";

    size_t time = 0;

    auto then = timer::now();
    auto source = Lattice::ObjIO::read(source_path);
    auto elapsed = std::chrono::duration_cast<units>(timer::now() - then).count();
    std::cout << "\n to-deform read : " << elapsed << ' ' << unit_str << "\n\n";
    time += elapsed;

    then = timer::now();
    auto rest = Lattice::ObjIO::read(rest_path);
    elapsed = std::chrono::duration_cast<units>(timer::now() - then).count();
    std::cout << " rest read : " << elapsed << ' ' << unit_str << "\n\n";
    time += elapsed;

    then = timer::now();
    auto deformed = Lattice::ObjIO::read(deformed_path);
    elapsed = std::chrono::duration_cast<units>(timer::now() - then).count();
    std::cout << " deformed read : " << elapsed << ' ' << unit_str << "\n\n";
    time += elapsed;

    then = timer::now();
    auto houdini = Lattice::ObjIO::read(houdini_path);
    elapsed = std::chrono::duration_cast<units>(timer::now() - then).count();
    std::cout << " houdini read : " << elapsed << ' ' << unit_str << "\n\n";

    float radius = 8.f;
    auto kernel =
        [](float d, float r)
        {
            auto t = d * d / (r * r);
            return 1.f + t / 9.f * (-22.f + 17.f * t - 4.f * t * t);
        };

    then = timer::now();
    auto result = Lattice::Deform::lattice<Lattice::Deform::SpatialViews::KDTree3D>(
        source,
        rest,
        deformed,
        kernel,
        radius,
        32u);//radius < 2.f ? 2.f : (radius < 4.f ? 4.f : radius));
    elapsed = std::chrono::duration_cast<units>(timer::now() - then).count();
    std::cout << " radius = " << radius
              << "\n\tlattice points deform (grid) : " << elapsed << ' ' << unit_str << "\n\n";

    float max_error = 0.f, avg_error = 0.f;
    const auto& resultPoints = result.points();
    auto n = resultPoints.size();
    const auto& houdini_points = houdini.points();
    for (int64_t i = 0; i < n; i++)
    {
        auto dist = glm::distance(resultPoints[i], houdini_points[i]);
        avg_error += dist;
        if (max_error < dist)
            max_error = dist;
    }
    std::cout << " avg error : " << avg_error / n << "\n\n";

    Lattice::ObjIO::write(result, result_path);

    Lattice::Demo::Window::create(1600, 900);
    auto* mesh = new Lattice::Rendering::OpenGL::Mesh(result);
    auto* shader = new Lattice::Rendering::OpenGL::Shader(shaders + "main.vert", shaders + "main.frag");
    Lattice::Demo::Window::load(mesh, shader, Lattice::Rendering::Camera {});
    Lattice::Demo::Window::run();
    Lattice::Demo::Window::destroy();

    std::cin.get();
    return 0;
}
