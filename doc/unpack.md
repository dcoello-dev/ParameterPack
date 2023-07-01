# Unpack

Once you finish updating your `TypePack` you can utilize it using a `Builder`.

## Builder

A builder is a metaprogramming function that uses the unpacked `TypePack` building or calling whatever you want:

```c++
// Builder that builds a tuple.
struct TupleBuilder
{
    template<typename ... Args>
    struct Build
    {
        using type = std::tuple<Args...>;
    };
};
```

```c++
// Builder that builds gtest testing::Types.
struct GTESTBuilder
{
    template<typename ... Args>
    struct Build
    {
        using type = ::testing::Types<Args...>;
    };
};
```

## Unpack

Given an existing `TypePack` call `unpack_t` with a `Builder` to unpack it:

```c++
using base = meta::TypePack<int, long>;

using my_tuple = base::unpack_t<TupleBuilder>;
using my_parametrized_test_types = base::unpack_t<GTESTBuilder>;

```