# Concatenation

Given 2 existing `TypePacks` you can concatenate them generating a new one:

```c++
// given a left LPP [A, B, C] and right RPP [1, 2, 3] generates [A, B, C, 1, 2, 3].
using Types1 =
        meta::TypePack<std::string, double>;

using Types2 = meta::TypePack<int, float>;

using Concatenation =
        typename meta::concat_t<Types1, Types2>
```