#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <typeinfo>
#include "utils.hpp"

#include "../ParameterPack.hpp"

using Sizes =
        meta::TypePack<meta::Constant<size_t, 0>, meta::Constant<size_t, 1>>;

struct SizesBuilder
{
    template <typename ... Args> struct Build
    {
        using type = meta::TypePack<Args...>;
    };
};
struct SizeCombination
{
    template <typename P, typename M> struct Type
    {
        using prealloc_ = P;
        using max_ = M;
    };
};

using PreallocAndMaxSizes =
        meta::mix_t<SizesBuilder, SizeCombination, Sizes, Sizes>;

struct MixTypeSize
{
    template <typename T, typename S> struct Type
    {
        using type = T;
        using prealloc_ = typename S::prealloc_;
        using max_ = typename S::max_;
    };
};

template <typename T, size_t P, size_t N> struct MyType
{
    using T_ = T;
    static constexpr size_t P_ = P;
    static constexpr size_t N_ = N;
};

struct MyTypeBuilder
{
    template <typename ... Args> struct Build
    {
        template <typename P>
        using get_type =
                MyType<typename P::type, P::prealloc_::value, P::max_::value>;
        using type = std::tuple<get_type<Args>...>;
    };
};

using TestTypes = meta::TypePack<int, float>;

using FinalTypes =
        meta::mix_t<MyTypeBuilder, MixTypeSize, TestTypes, PreallocAndMaxSizes>;



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