# Mix

Given 2 existing `TypePacks`, a `MixType` and a `Builder` you can mix them into a new one.

## MixType

You need to indicate in what kind of type both input parameters of mix are going to be mixed in.

For each possible combination a new `MixType` is going to be created:

```c++
struct SizeCombination
{
    template <typename T, typename S> struct Type
    {
        using test_type = T;
        using size = S;
    };
};
```

## Builder

`Builder` indicates final postprocessing of genrated mix, it can generate a tuple, another TypePack or gtest test cases, or maybe filter generated types:

```c++
struct MyTypeBuilder
{
    template <typename ... Args> struct Build
    {
        using type = std::tuple<Args ...>;
    };
};
```

## mix_t

```c++
// given a left LPP [A, B, C] and right RPP [1, 2, 3] generates an
// output BUILDER::Build = [A1, A2, A3, B1, B2, B3, C1, C2, C3] of types OT.
using TestTypes = meta::TypePack<int, float>;

using Sizes =
        meta::TypePack<meta::Constant<size_t, 0>, meta::Constant<size_t, 1>>;

using FinalTypes =
        meta::mix_t<MyTypeBuilder, MixTypeSize, TestTypes, PreallocAndMaxSizes>;
```