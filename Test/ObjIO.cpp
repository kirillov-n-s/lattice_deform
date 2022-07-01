#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include "../Lattice/Model.h"
#include "../Lattice/ObjIO/ObjIO.h"

namespace Test {
    TEST_CASE("ObjIO: successful")
    {
        std::vector<glm::vec4> points {
            { 1, 1, 1, 1 },
            { 2, 1, 1, 1 },
            { 1, 2, 1, 1 },
            { 1, 1, 2, 1 }
        };
        std::vector<glm::vec3> texcoords {
            { 1, 1, 0 },
            { 1, 2, 0 },
            { 2, 1, 0 },
            { 2, 2, 0 }
        };
        std::vector<glm::vec3> normals {
            { 1, 1, 1 },
            { 1, 2, 1 },
            { 2, 1, 1 },
            { 2, 2, 1 }
        };

        std::vector<int64_t> pointIndices {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3, 2,
            1, 2, 3, 0
        };
        std::vector<int64_t> texcoordIndices {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3, 2,
            1, 2, 3, 0
        };
        std::vector<int64_t> normalIndices {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3, 2,
            1, 2, 3, 0
        };

        std::vector<size_t> faceSizes { 3, 3, 4, 4 };

        SECTION("read: points only")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/Good/v.obj");

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

        SECTION("read: points and texcoords")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/Good/v_vt.obj");

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

        SECTION("read: points and normals")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/Good/v_vn.obj");

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

        SECTION("read: points, texcoords and normals")
        {
            const Lattice::Model& model = Lattice::ObjIO::read("../Test/Files/Good/v_vt_vn.obj");

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

        SECTION("write: points only")
        {
            Lattice::Model original {
                points,
                pointIndices,
                faceSizes
            };

            const char* path = "../Test/Files/Written/v.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }

        SECTION("write: points and texcoords")
        {
            Lattice::Model original {
                points,
                texcoords,
                {},
                pointIndices,
                texcoordIndices,
                {},
                faceSizes
            };

            const char* path = "../Test/Files/Written/v_vt.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }

        SECTION("write: points and normals")
        {
            Lattice::Model original {
                points,
                {},
                normals,
                pointIndices,
                {},
                normalIndices,
                faceSizes
            };

            const char* path = "../Test/Files/Written/v_vn.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }

        SECTION("write: points, texcoords and normals")
        {
            Lattice::Model original {
                points,
                texcoords,
                normals,
                pointIndices,
                texcoordIndices,
                normalIndices,
                faceSizes
            };

            const char* path = "../Test/Files/Written/v_vt_vn.obj";
            Lattice::ObjIO::write(original, path);
            const Lattice::Model& written = Lattice::ObjIO::read(path);

            REQUIRE(original == written);
        }
    }

    TEST_CASE("ObjIO: bad read")
    {
        SECTION("bad point")
        {
            REQUIRE_THROWS_MATCHES(
                Lattice::ObjIO::read("../Test/Files/Bad/v.obj"),
                std::runtime_error,
                Catch::Matchers::Message("Bad vertex coordinates."));
        }

        SECTION("bad texcoord")
        {
            REQUIRE_THROWS_MATCHES(
                Lattice::ObjIO::read("../Test/Files/Bad/vt.obj"),
                std::runtime_error,
                Catch::Matchers::Message("Bad texture coordinates."));
    }

        SECTION("bad normal")
        {
            REQUIRE_THROWS_MATCHES(
                Lattice::ObjIO::read("../Test/Files/Bad/vn.obj"),
                std::runtime_error,
                Catch::Matchers::Message("Bad normal coordinates."));
        }

        SECTION("bad face")
        {
            REQUIRE_THROWS_MATCHES(
                Lattice::ObjIO::read("../Test/Files/Bad/bad.obj"),
                std::runtime_error,
                Catch::Matchers::Message("Bad face format."));
        }

        SECTION("inconsistent face")
        {
            REQUIRE_THROWS_MATCHES(
                Lattice::ObjIO::read("../Test/Files/Bad/incon.obj"),
                std::runtime_error,
                Catch::Matchers::Message("Inconsistent face format."));
        }

        SECTION("not enough face elements")
        {
            REQUIRE_THROWS_MATCHES(
                Lattice::ObjIO::read("../Test/Files/Bad/small.obj"),
                std::runtime_error,
                Catch::Matchers::Message("Not enough face elements."));
        }
    }
}