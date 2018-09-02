#include "computerGeometry3D.hpp"
#include "gtest/gtest.h"

TEST(MultiVector, UserDefinedLiterals) {
    using namespace cf::literals;
    using Blade = cf::dMultiVector::Blade;

    // e0
    {
        const auto vec0 = cf::dMultiVector(Blade(Blade::TYPE::E0, 1.0));
        const auto vec1 = 1.0_e0;
        ASSERT_EQ(vec0, vec1);
    }

    // e1
    {
        const auto vec0 = cf::dMultiVector(Blade(Blade::TYPE::E1, 127312.0));
        const auto vec1 = 127312.0_e1;
        ASSERT_EQ(vec0, vec1);
    }

    // e2
    {
        const auto vec0 = cf::dMultiVector(Blade(Blade::TYPE::E2, 3.141592));
        const auto vec1 = 3.141592_e2;
        ASSERT_EQ(vec0, vec1);
    }

    // e3
    {
        const auto vec0 = cf::dMultiVector(Blade(Blade::TYPE::E3, 2.718281828));
        const auto vec1 = 2.718281828_e3;
        ASSERT_EQ(vec0, vec1);
    }

    // einf
    {
        const auto vec0 = cf::dMultiVector(Blade(Blade::TYPE::EINF, -0.123));
        const auto vec1 = -0.123_einf;
        ASSERT_EQ(vec0, vec1);
    }
}

TEST(MultiVector, Abs) {}

TEST(MultiVector, OuterProductValue) {
    using namespace cf::literals;
    using Blade = cf::dMultiVector::Blade;

    // only one type of blade
    {
        auto vec = 1.0_e1;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, 2.0));
        vec %= 2.0_value;
        ASSERT_EQ(vec, res);
        vec %= 3.14_value;
        res = cf::dMultiVector(Blade(Blade::TYPE::E1, 3.14 * 2));
        ASSERT_EQ(vec, res);
    }

    // multiple blades
    {
        auto vec = 1.30_e1 + 2.71_e2 + 3.14_e3 + 4.60_e0 + 1.41_einf;

        constexpr const double factor = 123.321;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, factor * 1.30), Blade(Blade::TYPE::E2, factor * 2.71),
                             Blade(Blade::TYPE::E3, factor * 3.14), Blade(Blade::TYPE::E0, factor * 4.60),
                             Blade(Blade::TYPE::EINF, factor * 1.41));
        vec %= 1.0_value * factor;
        ASSERT_EQ(vec, res);
    }
}

TEST(MultiVector, SimpleOuterProduct) {
    /// TODO
}

TEST(MultiVector, ComplexOuterProduct) {
    /// TODO
}

TEST(MultiVector, InnerProductValue) {
    using namespace cf::literals;
    using Blade = cf::dMultiVector::Blade;

    // multiply with none multi vector type
    {
        auto vec = 1.0_e1;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, 2.0));
        vec *= 2;
        ASSERT_EQ(vec, res);
        vec *= 3.14;
        res = cf::dMultiVector(Blade(Blade::TYPE::E1, 3.14 * 2));
        ASSERT_EQ(vec, res);
    }

    // only one type of blade
    {
        auto vec = 1.0_e1;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, 2.0));
        vec *= 2.0_value;
        ASSERT_EQ(vec, res);
        vec *= 3.14_value;
        res = cf::dMultiVector(Blade(Blade::TYPE::E1, 3.14 * 2));
        ASSERT_EQ(vec, res);
    }

    // multiple blades
    {
        auto vec = 1.30_e1 + 2.71_e2 + 3.14_e3 + 4.60_e0 + 1.41_einf;

        constexpr const double factor = 123.321;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, factor * 1.30), Blade(Blade::TYPE::E2, factor * 2.71),
                             Blade(Blade::TYPE::E3, factor * 3.14), Blade(Blade::TYPE::E0, factor * 4.60),
                             Blade(Blade::TYPE::EINF, factor * 1.41));
        vec *= factor;
        ASSERT_EQ(vec, res);
    }
}

TEST(MultiVector, SimpleInnerProduct) {
    using namespace cf::literals;
    using Blade = cf::dMultiVector::Blade;
    using BType = Blade::TYPE;

    static_assert(int(BType::E0) - int(BType::E1) == 4, "Enum might have changed!");

    for (int i = int(BType::E1); i < int(BType::E0); ++i) {     // all enums
        for (int k = int(BType::E1); k < int(BType::E3); ++k) { // only e1-e3
            const cf::dMultiVector vec0(Blade(BType(i), 1.0));
            const cf::dMultiVector vec1(Blade(BType(k), 1.0));
            const double res = double(vec0 * vec1);
            const double expected = i == k ? 1.0 : 0.0;
            ASSERT_DOUBLE_EQ(res, expected);
        }
    }

    for (int i = int(BType::E1); i < int(BType::E0); ++i) {     // all enums
        for (int k = int(BType::EINF); k < int(BType::E0); ++k) { // only einf-e0
            const cf::dMultiVector vec0(Blade(BType(i), 1.0));
            const cf::dMultiVector vec1(Blade(BType(k), 1.0));
            const double res = double(vec0 * vec1);
            double expected = 0.0;
            if (i != k && i >= int(BType::EINF))
                expected = 1.0;

            ASSERT_DOUBLE_EQ(res, expected);
        }
    }
}

TEST(MultiVector, ComplexInnerProduct) {
    using namespace cf::literals;

    {
        auto vec0 = 1.0_e1 % 1.0_e2;
        auto vec1 = 1.0_e2 % 1.0_e1;
        const auto res = vec0 * vec1;
        ASSERT_DOUBLE_EQ(double(res), -1.0);
    }
}

TEST(MultiVector, DualityOperator) {
    using namespace cf::literals;
    return;

    auto vec = 1.0_e0;
    ASSERT_EQ(*vec, 1.0_e2 % 1.0_e3 % 1.0_e0 % 1.0_einf);
    vec = 1.0_e1;
    ASSERT_EQ(*vec, 1.0_e1 % 1.0_e2 % 1.0_e3 % 1.0_einf);
    vec = 1.0_e2;
    ASSERT_EQ(*vec, 1.0_e1 % 1.0_e3 % 1.0_e0 % 1.0_einf);
    vec = 1.0_e3;
    ASSERT_EQ(*vec, 1.0_e1 % 1.0_e2 % 1.0_e0 % 1.0_einf);
    vec = 1.0_einf;
    ASSERT_EQ(*vec, 1.0_e1 % 1.0_e2 % 1.0_e3 % 1.0_e0);
}

TEST(MultiVector, GemetricProductValue) {
    using namespace cf::literals;
    using Blade = cf::dMultiVector::Blade;

    // only one type of blade
    {
        auto vec = 1.0_e1;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, 2.0));
        vec &= 2.0_value;
        ASSERT_EQ(vec, res);
        vec &= 3.14_value;
        res = cf::dMultiVector(Blade(Blade::TYPE::E1, 3.14 * 2));
        ASSERT_EQ(vec, res);
    }

    // multiple blades
    {
        auto vec = 1.30_e1 + 2.71_e2 + 3.14_e3 + 4.60_e0 + 1.41_einf;

        constexpr const double factor = 123.321;
        cf::dMultiVector res(Blade(Blade::TYPE::E1, factor * 1.30), Blade(Blade::TYPE::E2, factor * 2.71),
                             Blade(Blade::TYPE::E3, factor * 3.14), Blade(Blade::TYPE::E0, factor * 4.60),
                             Blade(Blade::TYPE::EINF, factor * 1.41));
        vec &= 1.0_value * factor;
        ASSERT_EQ(vec, res);
    }
}

TEST(MultiVector, SimpleGemetricProduct) {}

TEST(MultiVector, ComplexGemetricProduct) {}
