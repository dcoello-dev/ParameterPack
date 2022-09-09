#ifndef PARAMETER_PACK_H_
#define PARAMETER_PACK_H_

#include <tuple>

namespace meta {

template <typename ... A>
struct ParameterPack
{
    template<typename ... T>
    struct Pack
    {
        template <typename F, typename ... W>
        struct Unpacker
        {
            template<size_t rem, typename From>
            struct unpack
            {
                using __tp = Unpacker<F, typename std::tuple_element<rem, From>::type, W...>;
                using type = typename __tp::template unpack<rem - 1, From>::type;
            };

            template<typename From>
            struct unpack<0, From>
            {
                using type = typename F::Builder<typename std::tuple_element<0, From>::type, W...>::type;
            };
        };

        using tuple = std::tuple<T...>;
        using sequence = std::index_sequence_for<T...>;
        using Packer = ParameterPack<T...>;
        static constexpr auto size = sizeof...(T);

        template <std::size_t N>
        using get = typename std::tuple_element<N, tuple>::type;
    };

    template <typename C, typename ... R>
    struct push
    {
        using __tp = ParameterPack<A..., C>;
        using pack = typename __tp::template concat<R...>::pack;
    };

    template <typename C>
    struct push<C>
    {
        using __tp = ParameterPack<A..., C>;
        using pack = Pack<A..., C>;
    };

    template <std::size_t N, typename C, typename ... R>
    struct _remove
    {
        using pack = typename _remove<N - 1, R...>::pack;
    };

    template <typename C, typename ... R>
    struct _remove<0, C, R...>
    {
        using pack = Pack<C, R...>;
    };

    template <std::size_t N>
    struct pop
    {
        using pack = typename _remove<N, A...>::pack;
    };
};


} // namespace meta

#endif // PARAMETER_PACK_H_