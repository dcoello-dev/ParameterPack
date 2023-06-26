#include <algorithm>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ParameterPack.hpp"

template < typename T, typename U >
using equality_comparison_t = decltype(std::declval<T&>() == std::declval<U&>());

template < typename T, typename U, typename = std::void_t<>>
struct is_equality_comparable
    : std::false_type
{};

template      < typename T, typename U >
struct is_equality_comparable < T, U, std::void_t< equality_comparison_t<T, U>>>
    : std::is_same< equality_comparison_t<T, U>, bool >
{};

template <typename T>
struct DEQ
{
    template<bool comparable = is_equality_comparable<T, T>::value>
    typename std::enable_if<comparable, bool>::type
    operator ()(
            const T& r,
            const T& l)
    {
        return r == l;
    }

    template<bool comparable = is_equality_comparable<T, T>::value,
            bool pod = std::is_pod<T>::value,
            bool check = !comparable && pod>
    typename std::enable_if<check, bool>::type
    operator ()(
            const T& v,
            const T& l)
    {
        return memcmp(&v, &l, std::min(sizeof(v), sizeof(size_t))) == 0;
    }

};

template<typename T>
struct DBuilder
{

    template<bool assignable = std::is_assignable<T&, size_t>::value>
    typename std::enable_if<assignable, void>::type
    operator ()(
            T& v,
            size_t i)
    {
        v = i;
    }

    template<bool assignable = std::is_assignable<T&, size_t>::value,
            bool pod = std::is_pod<T>::value>
    typename std::enable_if<!assignable && pod, void>::type
    operator ()(
            T& v,
            size_t i)
    {
        memcpy(&v, &i, std::min(sizeof(v), sizeof(size_t)));
    }

};


template<typename T,
        typename Builder = DBuilder<T>,
        typename EQ = DEQ<T>>
struct TestType
{
    using type = T;
    using builder = Builder;
    using eq = EQ;
};

struct GTESTBuilder
{
    template<typename ... Args>
    struct Build
    {
        using type = ::testing::Types<Args...>;
    };
};