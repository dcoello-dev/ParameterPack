#ifndef PARAMETER_PACK_H_
#define PARAMETER_PACK_H_

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace meta {

/**
 * @class ParameterPack
 *
 * @brief Holds a pack of types.
 *
 * @warning All classes and variables that starts with `_` are for internal use.
 *
 * @tparam _TYPE_BUFFER... Holds pack of types, it acts like internal memory of types.
 */
template <typename ... _TYPE_BUFFER>
struct ParameterPack
{
    /**
     * @class pack
     *
     * @brief Pack functionality and usefull accessors.
     *
     * @tparam _INPUT_TYPES... holds pack input types, first call acts like constructor.
     */
    template<typename ... _INPUT_TYPES>
    struct pack
    {
        /**
         * @class __Unpacker
         *
         * @brief Unpack type pack using BUILDER hook.
         *
         * @warning internal use.
         *
         * @tparam BUILDER hook to specify in wich structure unpack,
         *      BUILDER type must have nested Build<Types...>.
         * @tparam _BUFFER hold unpacked tuple in a parameter pack.
         */
        template <typename BUILDER, typename ... _BUFFER>
        struct __Unpacker
        {
            /**
             * @class __unpack_elem
             *
             * @brief recursive function to unpack elements, one by one. Transition from
             * tuple type types to parameter pack.
             *
             * @warning internal use.
             *
             * @tparam __REM remaining elements, plus stop condition.
             * @tparam _FROM input tuple types.
             */
            template<size_t __REM, typename _FROM>
            struct __unpack_elem
            {
                //! recursive internal variable.
                using __tp = __Unpacker<BUILDER, typename std::tuple_element<__REM, _FROM>::type, _BUFFER...>;
                //! holds output type.
                using type = typename __tp::template __unpack_elem<__REM - 1, _FROM>::type;
            };

            /**
             * @class __unpack_elem
             *
             * @brief stop condicion when __REM == 0.
             *
             * @warning internal use.
             *
             * @tparam _FROM input tuple types.
             */
            template<typename _FROM>
            struct __unpack_elem<0, _FROM>
            {
                //! invoke BUILDER::Build with parameter pack, hold output value on type.
                using type = typename BUILDER::Build<typename std::tuple_element<0, _FROM>::type, _BUFFER...>::type;
            };
        };

        //! alias for ParameterPack mapping _INPUT_TYPES.
        using __packer = ParameterPack<_INPUT_TYPES...>;

        // PUBLIC interface

        //! tuple holding types pack.
        using tuple = std::tuple<_INPUT_TYPES...>;
        //! types index sequence.
        using sequence = std::index_sequence_for<_INPUT_TYPES...>;
        //! number of types in pack.
        static constexpr auto size = sizeof...(_INPUT_TYPES);

        /**
         * @class unpack
         *
         * @brief public accessor for unpacking.
         *
         * @details BUILDER hook to specify in wich structure unpack,
         *      BUILDER type must have nested Build<Types...>.
         *
         * @tparam BUILDER output type builder.
         */
        template<typename BUILDER>
        using unpack = typename __Unpacker<BUILDER>::__unpack_elem<size - 1, tuple>;

        //! alias for unpacking output type.
        template<typename B>
        using unpack_t = typename __Unpacker<B>::__unpack_elem<size - 1, tuple>::type;

        /**
         * @class get
         *
         * @brief get Nth element type of the type pack.
         *
         * @tparam N type index.
         */
        template <size_t N>
        using get = typename std::tuple_element<N, tuple>::type;

        /**
         * @class get_val
         *
         * @brief get Nth element value of the type pack in case its a meta::Constant.
         *
         * @tparam N value index.
         */
        template <size_t N>
        static constexpr auto get_val = std::tuple_element<N, tuple>::type::value;

        /**
         * @class add
         *
         * @brief push_back a new type element into the pack.
         *
         * @tparam E new element.
         */
        template <typename E>
        using add = typename __packer::_push<E>;

        /**
         * @class add_t
         *
         * @brief alias returning type.
         *
         * @tparam E new element.
         */
        template <typename E>
        using add_t = typename add<E>::type;

        /**
         * @class pop
         *
         * @brief pop N element types.
         *
         * @tparam N number of elements to remove from the pack.
         */
        template <size_t N>
        using pop = typename __packer::_pop<N>;

        /**
         * @class pop_t
         *
         * @brief alias returning type.
         *
         * @tparam N number of elements to remove from the pack.
         */
        template <size_t N>
        using pop_t = typename pop<N>::type;

    };

    // PRIVATE internal packer functionallity
    template <typename C, typename ... R>
    struct _push
    {
        using __tp = ParameterPack<_TYPE_BUFFER..., C>;
        using type = typename __tp::template concat<R...>::type;
    };

    template <typename C>
    struct _push<C>
    {
        using __tp = ParameterPack<_TYPE_BUFFER..., C>;
        using type = pack<_TYPE_BUFFER..., C>;
    };

    template <size_t N, typename C, typename ... R>
    struct _remove
    {
        using type = typename _remove<N - 1, R...>::type;
    };

    template <typename C, typename ... R>
    struct _remove<0, C, R...>
    {
        using type = pack<C, R...>;
    };

    template <size_t N>
    struct _pop
    {
        using type = typename _remove<N, _TYPE_BUFFER...>::type;
    };
};

/**
 * @class TypePack
 *
 * @brief syntactic sugar to intantiate a new pack.
 *
 * @tparam Types parameter pack with input types.
 */
template <typename ... Types>
using TypePack = meta::ParameterPack<>::pack<Types...>;

/**
 * @class Constant
 *
 * @brief syntactic sugar to hold an integral constant.
 *
 * @tparam T is the type of the constant.
 * @tparam __v is the value of the constant.
 */
template<typename T, T __v>
using Constant = std::integral_constant<T, __v>;

} // namespace meta

#endif // PARAMETER_PACK_H_