#pragma once

#include <array>

namespace obfuscate
{
    template<typename T, T Const>
    struct ForceConstant
    {
        enum { value = Const };
    };

    namespace assign
    {
        namespace detail_
        {
            template<typename T, std::size_t N, std::size_t Position> struct assign_helper
            {
                template< typename FIRST, typename... REST >
                static void assign(T(&vec)[N], FIRST&& first, REST&&... rest)
                {
                    vec[Position] = std::forward<FIRST>(first);
                    assign_helper<T, N, Position + 1>::assign(vec, std::forward<REST>(rest)...);
                }
            };

            template <typename T, std::size_t N> struct assign_helper<T, N, N>
            {
                template<typename... REST> static void assign(T(&)[N], const REST&...) {}
            };
        }

        template<typename T, std::size_t N, typename... U>
        void array(T(&vec)[N], U&&... pack)
        {
            static_assert(sizeof...(pack) >= N, "too few values");
            detail_::assign_helper< T, N, 0 >::assign(vec, std::forward<U>(pack)...);
        }
    }
}