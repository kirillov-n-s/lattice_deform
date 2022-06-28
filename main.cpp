#include <iostream>
#include <chrono>
#include "Lattice/ObjIO/ObjIO.h"
#include "Lattice/Deform/Deform.h"
#include "Lattice/Demo/Window.h"
#include "Lattice/Rendering/OpenGL/Mesh.h"
#include "Lattice/Rendering/OpenGL/Shader.h"

using Timer = std::chrono::system_clock;
using TimeUnits = std::chrono::milliseconds;

int main()
{
    const std::string timeUnits = "ms";

    const std::string path = "..\\data\\";
    const std::string meshesPath = path + "meshes\\";
    const std::string shadersPath = path + "shaders\\";

    const std::string toDeformPath = meshesPath + "jacket\\jacket.obj";
    const std::string restPath = meshesPath + "jacket\\base.obj";
    const std::string deformedPath = meshesPath + "jacket\\FemaleBody.obj";
    const std::string resultPath = meshesPath + "result\\jacket_female.obj";
    const std::string houdiniResultPath = meshesPath + "example\\jacket_female.obj";

    auto then = Timer::now();
    const Lattice::Model toDeform = Lattice::ObjIO::read(toDeformPath);
    auto elapsed = std::chrono::duration_cast<TimeUnits>(Timer::now() - then).count();
    std::cout << "\n to-deform read : " << elapsed << ' ' << timeUnits << "\n\n";

    then = Timer::now();
    const Lattice::Model rest = Lattice::ObjIO::read(restPath);
    elapsed = std::chrono::duration_cast<TimeUnits>(Timer::now() - then).count();
    std::cout << " rest read : " << elapsed << ' ' << timeUnits << "\n\n";

    then = Timer::now();
    const Lattice::Model deformed = Lattice::ObjIO::read(deformedPath);
    elapsed = std::chrono::duration_cast<TimeUnits>(Timer::now() - then).count();
    std::cout << " deformed read : " << elapsed << ' ' << timeUnits << "\n\n";

    then = Timer::now();
    const Lattice::Model houdiniResult = Lattice::ObjIO::read(houdiniResultPath);
    elapsed = std::chrono::duration_cast<TimeUnits>(Timer::now() - then).count();
    std::cout << " houdini result read : " << elapsed << ' ' << timeUnits << "\n\n";

    const float radius = 8.f;
    const auto kernel =
    [](const float distance, const float radius)
    {
        const auto tmp = distance * distance / (radius * radius);
        return 1.f + tmp / 9.f * (-22.f + 17.f * tmp - 4.f * tmp * tmp);
    };

    then = Timer::now();
    const Lattice::Model result = Lattice::Deform::lattice<Lattice::Deform::SpatialViews::KDTree3D>(
        toDeform,
        rest,
        deformed,
        kernel,
        radius,
        32u);//radius < 2.f ? 2.f : (radius < 4.f ? 4.f : radius));
    elapsed = std::chrono::duration_cast<TimeUnits>(Timer::now() - then).count();
    std::cout << " radius = " << radius
              << "\n\tlattice points deform : " << elapsed << ' ' << timeUnits << "\n\n";

    float error = 0.f;
    const auto& resultPoints = result.points();
    const size_t nPoints = resultPoints.size();
    const auto& houdiniResultPoints = houdiniResult.points();
    for (size_t i = 0; i < nPoints; i++)
        error += glm::distance(
            resultPoints[i],
            houdiniResultPoints[i]);
    std::cout << " avg error : " << error / nPoints << "\n\n";

    Lattice::ObjIO::write(result, resultPath);

    Lattice::Demo::Window::create(1600, 900);
    auto* mesh = new Lattice::Rendering::OpenGL::Mesh(result);
    auto* shader = new Lattice::Rendering::OpenGL::Shader(
        shadersPath + "main.vert",
        shadersPath + "main.frag");
    Lattice::Demo::Window::load(
        mesh,
        shader,
        Lattice::Rendering::Camera {});
    Lattice::Demo::Window::run();
    Lattice::Demo::Window::destroy();

    std::cin.get();
}
