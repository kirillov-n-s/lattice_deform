#ifdef _DEBUG

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include "../Lattice/Model.h"
#include "../Lattice/ObjIO/ObjIO.h"

namespace Test {
    TEST_CASE("ObjIO: good read-write")
    {
        const Lattice::Model::Points points {
            { 1, 1, 1, 1 },
            { 2, 1, 1, 1 },
            { 1, 2, 1, 1 },
            { 1, 1, 2, 1 }
        };
        const Lattice::Model::Texcoords texcoords {
            { 1, 1, 0 },
            { 1, 2, 0 },
            { 2, 1, 0 },
            { 2, 2, 0 }
        };
        const Lattice::Model::Normals normals {
            { 1, 1, 1 },
            { 1, 2, 1 },
            { 2, 1, 1 },
            { 2, 2, 1 }
        };

        const Lattice::Model::Indices pointIndices {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3, 2,
            1, 2, 3, 0
        };
        const Lattice::Model::Indices texcoordIndices {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3, 2,
            1, 2, 3, 0
        };
        const Lattice::Model::Indices normalIndices {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3, 2,
            1, 2, 3, 0
        };

        const Lattice::Model::FaceSizes faceSizes { 3, 3, 4, 4 };

        SECTION("Read: points only")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/GoodRead/v.obj");

            REQUIRE(points == model.points());
            REQUIRE(pointIndices == model.pointIndices());
            REQUIRE(faceSizes == model.faceSizes());

            REQUIRE(model.texcoords().empty());
            REQUIRE(model.normals().empty());
            REQUIRE(model.texcoordIndices().empty());
            REQUIRE(model.normalIndices().empty());

            REQUIRE(!model.hasNormals());
            REQUIRE(!model.hasTexcoords());
        }

        SECTION("Read: points and texcoords")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/GoodRead/v_vt.obj");

            REQUIRE(points == model.points());
            REQUIRE(pointIndices == model.pointIndices());
            REQUIRE(texcoords == model.texcoords());
            REQUIRE(texcoordIndices == model.texcoordIndices());
            REQUIRE(faceSizes == model.faceSizes());

            REQUIRE(model.normals().empty());
            REQUIRE(model.normalIndices().empty());

            REQUIRE(!model.hasNormals());
            REQUIRE(model.hasTexcoords());
        }

        SECTION("Read: points and normals")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/GoodRead/v_vn.obj");

            REQUIRE(points == model.points());
            REQUIRE(pointIndices == model.pointIndices());
            REQUIRE(normals == model.normals());
            REQUIRE(normalIndices == model.normalIndices());
            REQUIRE(faceSizes == model.faceSizes());

            REQUIRE(model.texcoords().empty());
            REQUIRE(model.texcoordIndices().empty());

            REQUIRE(model.hasNormals());
            REQUIRE(!model.hasTexcoords());
        }

        SECTION("Read: points, texcoords and normals")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/GoodRead/v_vt_vn.obj");

            REQUIRE(points == model.points());
            REQUIRE(pointIndices == model.pointIndices());
            REQUIRE(texcoords == model.texcoords());
            REQUIRE(texcoordIndices == model.texcoordIndices());
            REQUIRE(normals == model.normals());
            REQUIRE(normalIndices == model.normalIndices());
            REQUIRE(faceSizes == model.faceSizes());

            REQUIRE(model.hasNormals());
            REQUIRE(model.hasTexcoords());
        }

        SECTION("Write: points only")
        {
            const Lattice::Model original {
                points,
                pointIndices,
                faceSizes
            };

            const char* path = "../Test/Files/Write/v.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }

        SECTION("Write: points and texcoords")
        {
            const Lattice::Model original {
                points,
                texcoords,
                {},
                pointIndices,
                texcoordIndices,
                {},
                faceSizes
            };

            const char* path = "../Test/Files/Write/v_vt.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }

        SECTION("Write: points and normals")
        {
            const Lattice::Model original {
                points,
                {},
                normals,
                pointIndices,
                {},
                normalIndices,
                faceSizes
            };

            const char* path = "../Test/Files/Write/v_vn.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }

        SECTION("Write: points, texcoords and normals")
        {
            const Lattice::Model original {
                points,
                texcoords,
                normals,
                pointIndices,
                texcoordIndices,
                normalIndices,
                faceSizes
            };

            const char* path = "../Test/Files/Write/v_vt_vn.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }
    }

    TEST_CASE("ObjIO: Read: bad point")
    {
        REQUIRE_THROWS_MATCHES(
            Lattice::ObjIO::read("../Test/Files/BadRead/v.obj"),
            std::runtime_error,
            Catch::Matchers::Message("Bad vertex coordinates."));
    }

    TEST_CASE("ObjIO: Read: bad texcoord")
    {
        REQUIRE_THROWS_MATCHES(
            Lattice::ObjIO::read("../Test/Files/BadRead/vt.obj"),
            std::runtime_error,
            Catch::Matchers::Message("Bad texture coordinates."));
    }

    TEST_CASE("ObjIO: Read: bad normal")
    {
        REQUIRE_THROWS_MATCHES(
            Lattice::ObjIO::read("../Test/Files/BadRead/vn.obj"),
            std::runtime_error,
            Catch::Matchers::Message("Bad normal coordinates."));
    }

    TEST_CASE("ObjIO: Read: bad face")
    {
        REQUIRE_THROWS_MATCHES(
            Lattice::ObjIO::read("../Test/Files/BadRead/bad.obj"),
            std::runtime_error,
            Catch::Matchers::Message("Bad face format."));
    }

    TEST_CASE("ObjIO: Read: inconsistent face")
    {
        REQUIRE_THROWS_MATCHES(
            Lattice::ObjIO::read("../Test/Files/BadRead/incon.obj"),
            std::runtime_error,
            Catch::Matchers::Message("Inconsistent face format."));
    }

    TEST_CASE("ObjIO: Read: not enough face elements")
    {
        REQUIRE_THROWS_MATCHES(
            Lattice::ObjIO::read("../Test/Files/BadRead/small.obj"),
            std::runtime_error,
            Catch::Matchers::Message("Not enough face elements."));
    }
}

#endif
