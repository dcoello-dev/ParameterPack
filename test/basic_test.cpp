#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "utils.hpp"

#include <ParameterPack.hpp>

struct BasicTest : public ::testing::Test
{};


TEST_F(BasicTest, ADD){
    using base = meta::TypePack<int>;

    ASSERT_EQ(base::size, 1);

    using final = base::add_t<float>;

    ASSERT_EQ(final ::size, 2);
}


TEST_F(BasicTest, POP){
    using base = meta::TypePack<int, float>;

    ASSERT_EQ(base::size, 2);

    using final = base::pop_t<1>;

    ASSERT_EQ(final ::size, 1);
    ASSERT_TRUE(typeid(base::get<0>) == typeid(int));
}

TEST_F(BasicTest, POP_2){
    using base = meta::TypePack<int, float, long>;

    ASSERT_EQ(base::size, 3);

    using final = base::pop_t<2>;

    ASSERT_EQ(final ::size, 1);
    ASSERT_TRUE(typeid(base::get<0>) == typeid(int));
}

TEST_F(BasicTest, GET){
    using base = meta::TypePack<int, float, long>;

    ASSERT_TRUE(typeid(base::get<0>) == typeid(int));
    ASSERT_TRUE(typeid(base::get<1>) == typeid(float));
    ASSERT_TRUE(typeid(base::get<2>) == typeid(long));
}

TEST_F(BasicTest, GET_VAL){
    using base = meta::TypePack<meta::Constant<size_t, 0>,
                    meta::Constant<size_t, 1>,
                    meta::Constant<size_t, 2>>;

    ASSERT_EQ(base::get_val<0>, 0);
    ASSERT_EQ(base::get_val<1>, 1);
    ASSERT_EQ(base::get_val<2>, 2);
}
