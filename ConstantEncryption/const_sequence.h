#pragma once

#include <type_traits>

namespace obfuscate
{
    namespace detail
    {
        // Note: This is a minimalistic implementation of index_sequence available in C++14
        //       It should be replaced once the project is moved to C++14 or beyond
        template <std::size_t... Ints>
        struct index_sequence
        {
        };

        template <typename T, std::size_t N>
        struct make_index_sequence_impl;

        template <std::size_t N, std::size_t... Ints>
        struct make_index_sequence_impl<index_sequence<Ints...>, N>
        {
            using type = typename make_index_sequence_impl<index_sequence<Ints..., sizeof...(Ints)>, N - 1>::type;
        };

        template <std::size_t... Ints>
        struct make_index_sequence_impl<index_sequence<Ints...>, 0>
        {
            using type = index_sequence<Ints...>;
        };

        template <std::size_t N>
        using make_index_sequence = typename make_index_sequence_impl<index_sequence<>, N>::type;
    }

    template <char... Cs>
    struct char_sequence
    {
        template <typename Context>
        static const char* char_ptr()
        {
            static char s[] = { Cs..., '\0' };
            return s;
        }
    };

    template <std::size_t N, const char* s, typename T>
    struct make_char_sequence_impl;

    template <std::size_t N, const char* s, std::size_t... i>
    struct make_char_sequence_impl<N, s, detail::index_sequence<i...>>
    {
        using type = char_sequence<s[i]...>;
    };

    template <std::size_t N, const char* Input>
    using make_char_sequence =
        typename make_char_sequence_impl<N, Input, detail::make_index_sequence<N - 1>>::type;
}