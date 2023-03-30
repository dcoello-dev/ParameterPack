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
                using type =
                        typename BUILDER::template Build<typename std::tuple_element<0, _FROM>::type,
                                _BUFFER...>::type;
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
        using unpack = typename __Unpacker<BUILDER>::template __unpack_elem<size - 1, tuple>;

        //! alias for unpacking output type.
        template<typename B>
        using unpack_t = typename __Unpacker<B>::template __unpack_elem<size - 1, tuple>::type;

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
        using add = typename __packer::template _push<E>;

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
        using pop = typename __packer::template _pop<N>;

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

/**
 * @class __Mix
 *
 * @brief Mix two ParameterPacks in to one.
 *
 * @details given a left LPP [A, B, C] and right RPP [1, 2, 3] generates an
 *  output BUILDER::Build = [A1, A2, A3, B1, B2, B3, C1, C2, C3] of types OT.
 *
 * @warning private use only.
 *
 * @tparam _BUILDER final builder, need to have a nester Build method.
 * @tparam _OT output type, neet to have 2 templatic parameters that will handle
 *  combination of RPP and LPP element types.
 * @tparam _LPP left input ParameterPack.
 * @tparam _RPP right input ParameterPack.
 */
template<typename _BUILDER, typename _OT, typename _LPP, typename _RPP>
struct __Mix
{
    /**
     * @class __lpp_foreach
     *
     * @brief LPP foreach.
     *
     * @details for each of LPP elements doing for each of LPP elements for each one.
     *
     * @warning private use only.
     *
     * @tparam _HEAD current type element of LPP.
     * @tparam _TAIL remaining elements of LPP.
     */
    template <typename _HEAD, typename ... _TAIL>
    struct __lpp_foreach
    {
        /**
         * @class __rpp_foreach
         *
         * @brief RPP foreach.
         *
         * @details foreach RPP build _OT type with LPP and RPP current elements.
         *
         * @warning private use only.
         *
         * @tparam __REM counter.
         * @tparam S LPP.
         */
        template<size_t __REM, typename S>
        struct __rpp_foreach
        {
            using __s = typename S::template get<__REM>;
            using _tp = typename _OT::template Type<_HEAD, __s>;
            //! continue with rpp and add new element.
            using type = typename __rpp_foreach<__REM - 1, S>::type::template add_t<_tp>;
        };

        //! stop condition when counter == 0.
        template<typename S>
        struct __rpp_foreach<0, S>
        {
            using __s = typename S::template get<0>;
            using _tp = typename _OT::template Type<_HEAD, __s>;
            //! trigger lpp foreach and add last element.
            using type = typename __lpp_foreach<_TAIL...>::type::template add_t<_tp>;
        };

        //! Built BUILDER::Build Type of OTs.
        using type = typename __rpp_foreach<_RPP::size - 1, _RPP>::type;
    };

    // Stop condition for lpp foreach.
    template <typename H>
    struct __lpp_foreach<H>
    {
        template<size_t __REM, typename S>
        struct __rpp_foreach
        {
            using __s = typename S::template get<__REM>;
            using _tp = typename _OT::template Type<H, __s>;
            using type = typename __rpp_foreach<__REM - 1, S>::type::template add_t<_tp>;
        };

        //! Last LPP and RPP element creates ParameterPack.
        template<typename S>
        struct __rpp_foreach<0, S>
        {
            using __s = typename S::template get<0>;
            using _tp = typename _OT::template Type<H, __s>;
            template <typename ... Types>
            using __PP = ParameterPack<>::pack<Types...>;
            //! Creates ParameterPack that is going to be pushed to the top.
            using type = __PP<_tp>;
        };

        using type = typename __rpp_foreach<_RPP::size - 1, _RPP>::type;
    };


    /**
     * @class __trigger
     *
     * @brief entry point to trigger mix.
     *
     * @details Mix process involves two nested unpack processes.
     *
     * @warning private use only.
     */
    struct __trigger
    {
        /**
         * @class _NestedBuilder
         *
         * @brief Builder for high order unpack process.
         *
         * @warning private use only.
         */
        struct _NestedBuilder
        {
            template<typename ... Args>
            struct Build
            {
                //! Calls internal __lpp_foreach with LPP parameter pack and triggers RPP unpack with given BUILDER
                using type = typename __lpp_foreach<Args...>::type::template unpack_t<_BUILDER>;
            };
        };

        //! triggers LPP unpack with _NestedBuilder
        using type = typename _LPP::template unpack_t<_NestedBuilder>;
    };

    //! Output type.
    using type = typename __trigger::type;
};

/**
 * @class mix_t
 *
 * @brief mix RPP ParameterPack with LPP ParameterPack.
 *
 * @details given a left LPP [A, B, C] and right RPP [1, 2, 3] generates an
 *  output BUILDER::Build = [A1, A2, A3, B1, B2, B3, C1, C2, C3] of types OT.
 *
 * @tparam _BUILDER final builder, need to have a nester Build method.
 * @tparam _OT output type, neet to have 2 templatic parameters that will handle
 *  combination of RPP and LPP element types.
 * @tparam _LPP left input ParameterPack.
 * @tparam _RPP right input ParameterPack.
 */
template<typename B, typename CT, typename _LPP, typename _RPP>
using mix_t = typename __Mix<B, CT, _LPP, _RPP>::type;

} // namespace meta

#endif // PARAMETER_PACK_H_