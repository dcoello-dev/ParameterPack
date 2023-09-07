#include <gtest/gtest.h>

#include "utils.hpp"

#include <ParameterPack.hpp>

struct MyTypeBuilder
{
    template <typename ... Args> struct Build
    {
        using type = std::tuple<Args...>;
    };
};

using Types1 =
        meta::TypePack<std::string, double>;

using Types2 = meta::TypePack<int, float>;

using FinalTypes =
        typename meta::concat_t<Types1, Types2>::unpack_t<MyTypeBuilder>;


struct ConcatTest : public ::testing::Test
{};


TEST_F(ConcatTest, TEST1){
    auto mytuple = FinalTypes{};
    int size = std::tuple_size<decltype(mytuple)>::value;

    ASSERT_EQ(size, 4);

    std::cerr << size << " types in the tuple" << std::endl;
    std::istringstream type_name(type(mytuple));
    std::string nested_type{};
    std::string to_print{};
    while (getline(type_name, nested_type, ','))
    {
        if (nested_type.find("MyType") != std::string::npos)
        {
            std::cerr << to_print << std::endl;
            to_print = nested_type;
        }
        else
        {
            to_print += nested_type;
        }
    }
    std::cerr << to_print << std::endl;
}
