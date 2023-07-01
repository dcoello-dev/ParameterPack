# Basic Functionality

`TypePack` offers an API for manipulating it and generating a new `TypePack` with modifications.

Once all modifications are done you can unpack `TypePack` to use it in a `Builder`.

## ADD

Add a new type to an existing `TypePack`:

```c++
// Existing TypePack with only one type.
using base = meta::TypePack<int>;

// add_t<type> allows create a new TypePack whith new type added.
using final = base::add_t<float>;
```

## POP

Remove types from the back of an existing `TypePack`:

```c++
// Existing TypePack with 3 types.
using base = meta::TypePack<int, float, long>;

// pop_t<N> remove N elements from the back of base.
using final = base::pop_t<2>;
```


## GET

Get type from `TypePack` by index:

```c++
// Existing TypePack with 3 types.
using base = meta::TypePack<int, std::string, long>;

// get<1> returns std::string type.
typename base::get<1> name = "my name";
```

## GET_VAL

Get value from `TypePack` by index:

```c++
// Existing TypePack with 3 Constant types.
using base = meta::TypePack<meta::Constant<size_t, 0>,
                    meta::Constant<size_t, 1>,
                    meta::Constant<size_t, 2>>;

// get_val<1> returns 1.
auto name = base::get_val<1>;
```
