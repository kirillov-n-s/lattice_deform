#ifdef _DEBUG

#include <catch2/catch_test_macros.hpp>

#include "../Lattice/Model.h"
#include "../Lattice/Conversion.h"
#include "../Lattice/ObjIO/ObjIO.h"

namespace Test {
    TEST_CASE("Conversion: is triangle")
    {
        const Lattice::Model model {
            {},
            {},
            { 3, 3, 3, 3 }
        };

        REQUIRE(Lattice::isTriangle(model));
    }

    TEST_CASE("Conversion: is not triangle")
    {
        const Lattice::Model model {
            {},
            {},
            { 3, 3, 4, 4 }
        };

        REQUIRE(!Lattice::isTriangle(model));
    }

    TEST_CASE("Conversion: triangulate")
    {
        const Lattice::Model::Points points {
            { 1, 1, 1, 1 },
            { 2, 1, 1, 1 },
            { 1, 2, 1, 1 },
            { 1, 1, 2, 1 },
            { 2, 2, 2, 1 }
        };

        const Lattice::Model model {
            points,
            {
                0, 2, 1,
                0, 3, 2,
                0, 1, 3, 2,
                1, 2, 3, 0, 4
            },
            { 3, 3, 4, 5 }
        };
        const Lattice::Model triangleModel {
            points,
            {
                0, 2, 1,
                0, 3, 2,
                0, 1, 3,
                0, 3, 2,
                1, 2, 3,
                1, 3, 0,
                1, 0, 4
            },
            { 3, 3, 3, 3, 3, 3, 3 }
        };

        const Lattice::Model triangulatedModel = Lattice::triangulate(model);

        REQUIRE(triangleModel == triangulatedModel);
    }

    TEST_CASE("Conversion: indexed vertices")
    {
        const std::vector<Lattice::Vertex> expectedVertices {
            {
                { 1, 1, 1, 1 },
                { 1, 1, 0 },
                { 1, 1, 1 },
            },
            {
                { 1, 2, 1, 1 },
                { 2, 1, 0 },
                { 2, 1, 1 },
            },
            {
                { 2, 1, 1, 1 },
                { 1, 2, 0 },
                { 1, 2, 1 },
            },
            {
                { 1, 1, 2, 1 },
                { 2, 2, 0 },
                { 2, 2, 1 },
            },
            {
                { 1, 2, 1, 1 },
                { 1, 2, 0 },
                { 2, 1, 1 },
            },
            {
                { 2, 1, 1, 1 },
                { 1, 2, 0 },
                { 2, 2, 1 },
            },
            {
                { 1, 1, 1, 1 },
                { 1, 2, 0 },
                { 1, 1, 1 },
            },
        };

        const std::vector<unsigned int> expectedIndices {
            0, 1, 2,
            0, 3, 4,
            0, 2, 3,
            0, 3, 4,
            5, 1, 3,
            5, 3, 6
        };

        const char* path = "../Test/Files/Conversion/conv.obj";
        const Lattice::Model& model = Lattice::ObjIO::read(path);
        const auto& [actualVertices, actualIndices] = Lattice::indexedVertices(model);

        REQUIRE(expectedVertices == actualVertices);
        REQUIRE(expectedIndices == actualIndices);
    }
}

#endif
