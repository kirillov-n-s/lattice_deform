#ifdef _DEBUG

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "../Lattice/Model.h"
#include "../Lattice/Deform/Deform.h"
#include "../Lattice/ObjIO/ObjIO.h"

namespace Test {
    TEST_CASE("Deform")
    {
        const Lattice::Model& rest = Lattice::ObjIO::read("../Test/Files/Deform/rest.obj");
        const Lattice::Model& deformed = Lattice::ObjIO::read("../Test/Files/Deform/deformed.obj");
        const Lattice::Model& toDeform = Lattice::ObjIO::read("../Test/Files/Deform/to_deform.obj");
        const Lattice::Model& expectedResult = Lattice::ObjIO::read("../Test/Files/Deform/result.obj");

        const auto& expectedPoints = expectedResult.points();
        const auto& expectedTexcoords = expectedResult.texcoords();
        const auto& expectedNormals = expectedResult.normals();

        const float radius = 1.f,
                    epsilon = 1e-6f;
        const auto kernel =
        [](const float distance, const float radius)
        {
            const auto tmp = distance * distance / (radius * radius);
            return 1.f + tmp / 9.f * (-22.f + 17.f * tmp - 4.f * tmp * tmp);
        };

        SECTION("Uniform grid")
        {
            const Lattice::Model& actualResult = Lattice::Deform::lattice<Lattice::Deform::SpatialViews::UniformGrid>(
                toDeform,
                rest,
                deformed,
                kernel,
                radius,
                2.f);

            const auto& actualPoints = actualResult.points();
            const auto& actualTexcoords = actualResult.texcoords();
            const auto& actualNormals = actualResult.normals();

            const auto vecApproxEqual =
            [&epsilon](const auto& lhs, const auto& rhs)
            {
                return glm::all(glm::epsilonEqual(
                    lhs,
                    rhs,
                    epsilon));
            };

            REQUIRE(std::equal(
                expectedPoints.begin(),
                expectedPoints.end(),
                actualPoints.begin(),
                vecApproxEqual));
            REQUIRE(std::equal(
                expectedTexcoords.begin(),
                expectedTexcoords.end(),
                actualTexcoords.begin(),
                vecApproxEqual));
            REQUIRE(std::equal(
                expectedNormals.begin(),
                expectedNormals.end(),
                actualNormals.begin(),
                vecApproxEqual));
        }

        SECTION("KD-Tree")
        {
            const Lattice::Model& actualResult = Lattice::Deform::lattice<Lattice::Deform::SpatialViews::KDTree3D>(
                    toDeform,
                    rest,
                    deformed,
                    kernel,
                    radius,
                    16u);

            const auto& actualPoints = actualResult.points();
            const auto& actualTexcoords = actualResult.texcoords();
            const auto& actualNormals = actualResult.normals();

            const auto vecApproxEqual =
            [&epsilon](const auto& lhs, const auto& rhs)
            {
                return glm::all(glm::epsilonEqual(
                    lhs,
                    rhs,
                    epsilon));
            };

            REQUIRE(std::equal(
                expectedPoints.begin(),
                expectedPoints.end(),
                actualPoints.begin(),
                vecApproxEqual));
            REQUIRE(std::equal(
                expectedTexcoords.begin(),
                expectedTexcoords.end(),
                actualTexcoords.begin(),
                vecApproxEqual));
            REQUIRE(std::equal(
                expectedNormals.begin(),
                expectedNormals.end(),
                actualNormals.begin(),
                vecApproxEqual));
        }
    }
}

#endif
