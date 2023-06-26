#include <cassert>
#include <TypeManager.hpp>

using my_type = TestType<int>;

int main (
        void)
{
    typename my_type::type t{0};
    assert(t == 0);

    my_type::builder{} (t, 1);
    assert(t == 1);
    typename my_type::eq EQ{};
    assert(EQ(t, 1));

    my_type::builder{} (t, 0);

    return t;
}