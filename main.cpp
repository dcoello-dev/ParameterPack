#include <cstdint>
#include <iostream>

#include "ParameterPack.hpp"

template<size_t _S, size_t _N>
struct ValWrapper
{
    static constexpr size_t S = _S;
    static constexpr size_t N = _N;
};

template<class T, size_t S>
struct build_type
{
    using pt = typename T::Packer::template push<ValWrapper<S, S>>::pack;
    using type = typename build_type<pt, S - 4>::type;
};

template<class T>
struct build_type<T, 0>
{
    using pt = typename T::Packer::template push<ValWrapper<0, 0>>::pack;
    using type = pt;
};

int main(
        void)
{
    using pack = ParameterPack<>::Pack<ValWrapper<256, 256>, ValWrapper<256, 256>>;
    std::cout << pack::size << std::endl;
    using pack2 = pack::Packer::template push<ValWrapper<256, 256>>::pack;
    std::cout << pack2::size << std::endl;
    using pack3 = pack2::Packer::template pop<2>::pack;
    std::cout << pack3::size << std::endl;

    using built_types = typename build_type<pack2, 252>::type;
    std::cout << built_types::size << std::endl;

    using Case_Types = built_types::Unpacker<>::unpack<built_types::size - 1, built_types::tuple>::type;

    return 0;
}