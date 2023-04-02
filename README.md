# ParameterPack

Parameter pack manipulation utility.

## Installation

Just add .hpp file to your project ;).

## Build examples

```bash
mkdir build && cd build
cmake .. && make
```

## Example

You can see similar example in [godbolt](https://godbolt.org/z/83PoKzcr1).

### The problem

You want to test a templatic class:

```c++
template<typename T>
struct ToCheck {
    T val {};

    bool check(T other) {
        return other < val;
    }
};
```

You want to use GTEST parametrized tests and you decide to use a simple templatic structure to define the typed tests:

```c++
template<size_t _S, size_t _N>
struct ValWrapper
{
    static constexpr size_t S = _S;
    static constexpr size_t N = _N;
};
```

And you write a simple test that checks `check` method works fine:

```c++
TYPED_TEST_P(ToCheckTest, CHECK_1){
    std::cerr << TypeParam::S << std::endl;
    std::cerr << TypeParam::N << std::endl;

    ToCheck<TypeParam::S> tocheck{};

    ASSERT_EQ(tocheck.val, TypeParam::S);
    ASSERT_TRUE(tocheck.check(TypeParam::N));
}

// More stuff here...

typedef ::testing::Types<ValWrapper<10, 1>> Common_Cases_Types;

// and here...
```
Thats nice but the problem here is that you dont want to define manually all the test cases and `::testing::Types` receives a parameter pack as argument.

You nedd a some way to manipulate a paramter pack on build time to have an smart way to create your test types.

```c++
typedef ::testing::Types</* smart generator here */> Common_Cases_Types;
```

### The solution

Lets start: `ParameterPack.hpp` provides a container-like functional utility to wrap the paramter pack so we start creating a Pack with an initial value:

```c++
using pack = ParameterPack<>::Pack<ValWrapper<256, 0>>;
```

We want to generate all ValWrappers from `ValWrapper<256, 0>` to `ValWrapper<1, 0>`, and for it we need 2 templatic generator functions:

Due C++ metaprograming is functional paradigm we need to use recursivity so we need a stop case.

```c++
template<class T, size_t S>
struct build_type
{
    using __pt = typename T::Packer::template push<ValWrapper<S, 0>>::pack;
    using type = typename build_type<__pt, S - 1>::type;
};

template<class T>
struct build_type<T, 1>
{
    using __pt = typename T::Packer::template push<ValWrapper<1, 0>>::pack;
    using type = __pt;
};
```

Next step is to generate the types:

```c++
// We already have 256 in our pack so we start in 255.
using built_types = typename build_type<pack2, 255>::type;
```

Once we have our pack we need a way to indicate that we want to use it to define the case types, for it, we need to define a `Factory`:

```c++
struct Factory
{
    template<typename ... Args>
    struct Builder
    {
        using type = ::testing::Types<Args...>;
    };
};
```

Factory can be called as you like but is mandatory to call Builder the nested templatic struct.

You can have different Factorys for different types, for example:

```c++
struct Factory
{
    template<typename ... Args>
    struct Builder
    {
        using type = std::tuple<Args...>;
    };
};
```

Now we are ready to define the types!, this is done calling the `Unpacker` nested in the Pack indicating the `Factory`:

```c++
using Case_Types = built_types::Unpacker<Factory>::unpack<built_types::size - 1, built_types::tuple>::type;

```

## Mix ParameterPacks

Example here: [godbolt](https://godbolt.org/z/GG3Y8sP6z).