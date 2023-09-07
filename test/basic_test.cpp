#include <gtest/gtest.h>

#include "utils.hpp"
#include <ParameterPack.hpp>


struct BasicTest : public ::testing::Test
{};

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

TEST_F(BasicTest, ADD){
    using base = meta::TypePack<int>;
    ASSERT_EQ(base::size, 1);

    using add_float = base::add_t<float>;
    ASSERT_EQ(add_float::size, 2);

    using add_repeated = add_float::add_t<float>;
    ASSERT_EQ(add_repeated::size, 3);
}

TEST_F(BasicTest, POP){
    using base = meta::TypePack<int, float>;
    ASSERT_EQ(base::size, 2);

    using removed_one = base::pop_t<1>;
    ASSERT_EQ(removed_one::size, 1);
    ASSERT_TRUE(typeid(removed_one::get<0>) == typeid(float));

    using base2 = meta::TypePack<int, float, long>;
    ASSERT_EQ(base2::size, 3);

    using removed_two = base2::pop_t<2>;
    ASSERT_EQ(removed_two::size, 1);
    ASSERT_TRUE(typeid(removed_two::get<0>) == typeid(long));
}
