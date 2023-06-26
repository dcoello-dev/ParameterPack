#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <typeinfo>
#include "utils.hpp"

#include "../ParameterPack.hpp"

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



int main(
        void)
{
    auto mytuple = FinalTypes{};
    int size = std::tuple_size<decltype(mytuple)>::value;
    std::cout << size << " types in the tuple" << std::endl;
    std::istringstream type_name(type(mytuple));
    std::string nested_type{};
    std::string to_print{};
    while (getline(type_name, nested_type, ','))
    {
        if (nested_type.find("MyType") != std::string::npos)
        {
            std::cout << to_print << std::endl;
            to_print = nested_type;
        }
        else
        {
            to_print += nested_type;
        }
    }
    std::cout << to_print << std::endl;
    return 0;
}