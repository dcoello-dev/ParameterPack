template<typename T>
struct ExampleClass
{
    ExampleClass(
            T t)
        : _t(t)
    {

    }

    T get_t() noexcept
    {
        return _t;
    }

    T _t;
};