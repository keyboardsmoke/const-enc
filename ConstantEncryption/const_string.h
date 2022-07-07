#pragma once

#include <array>

// Sequence of char
template <char... Cs> struct char_sequence
{
	template <char C> using push_back = char_sequence<Cs..., C>;
};

// Remove all chars from char_sequence from '\0'
template <typename, char...> struct strip_sequence;

template <char... Cs>
struct strip_sequence<char_sequence<>, Cs...>
{
	using type = char_sequence<Cs...>;
};

template <char...Cs, char...Cs2>
struct strip_sequence<char_sequence<'\0', Cs...>, Cs2...>
{
	using type = char_sequence<Cs2...>;
};

template <char... Cs, char C, char... Cs2>
struct strip_sequence<char_sequence<C, Cs...>, Cs2...>
{
	using type = typename strip_sequence<char_sequence<Cs...>, Cs2..., C>::type;
};

// helper to get the i_th character (`\0` for out of bound)
template <std::size_t I, std::size_t N>
constexpr char at(const char(&a)[N]) { return I < N ? a[I] : '\0'; }

// helper to check if the c-string will not be truncated
template <std::size_t max_size, std::size_t N>
constexpr bool check_size(const char(&)[N])
{
	static_assert(N <= max_size, "string too long");
	return N <= max_size;
}

// Helper macros to build char_sequence from c-string
#define PUSH_BACK_8(S, I) \
    ::push_back<at<(I) + 0>(S)>::push_back<at<(I) + 1>(S)> \
    ::push_back<at<(I) + 2>(S)>::push_back<at<(I) + 3>(S)> \
    ::push_back<at<(I) + 4>(S)>::push_back<at<(I) + 5>(S)> \
    ::push_back<at<(I) + 6>(S)>::push_back<at<(I) + 7>(S)>

#define PUSH_BACK_32(S, I) \
        PUSH_BACK_8(S, (I) + 0) PUSH_BACK_8(S, (I) + 8) \
        PUSH_BACK_8(S, (I) + 16) PUSH_BACK_8(S, (I) + 24)

#define PUSH_BACK_128(S, I) \
    PUSH_BACK_32(S, (I) + 0) PUSH_BACK_32(S, (I) + 32) \
    PUSH_BACK_32(S, (I) + 64) PUSH_BACK_32(S, (I) + 96)

// Macro to create char_sequence from c-string (limited to 128 chars)
#define MAKE_CHAR_SEQUENCE(S) \
    strip_sequence<char_sequence<> \
    PUSH_BACK_128(S, 0) \
    >::type::template push_back<check_size<128>(S) ? '\0' : '\0'>

namespace obfuscate
{
	template<typename BlockT>
	class XorCipher
	{
	public:
		static constexpr BlockT Encode(BlockT input, BlockT key, size_t index)
		{
			return input ^ key;
		}

		static constexpr BlockT Decode(BlockT input, BlockT key, size_t index)
		{
			return input ^ key;
		}
	};

	template<typename BlockT>
	class AddSubCipher
	{
	public:
		static constexpr BlockT Encode(BlockT input, BlockT key, size_t index)
		{
			return input + key;
		}

		static constexpr BlockT Decode(BlockT input, BlockT key, size_t index)
		{
			return input - key;
		}
	};

	template<typename BlockT, size_t CipherIndex>
	using CipherSelector =
		std::conditional_t<CipherIndex == 0U, XorCipher<BlockT>,
		std::conditional_t<CipherIndex == 1U, AddSubCipher<BlockT>, void>>;

	template<size_t N>
	struct BlockSelector
	{
		static constexpr bool isBlock8 = (N == 1);
		static constexpr bool isBlock16 = ((N % 2) == 0);
		static constexpr bool isBlock32 = ((N % 4) == 0);
		static constexpr bool isBlock64 = ((N % 8) == 0);

		using BlockT = std::conditional_t<isBlock8, uint8_t, std::conditional_t<isBlock64, uint64_t, std::conditional_t<isBlock32, uint32_t, std::conditional_t<isBlock16, uint16_t, uint8_t>>>>;
	};

	/*template <typename StringProvider> using explode = typename explode_impl<StringProvider, c_strlen(StringProvider::data())>::result;*/

	template<typename T, T... Chars>
	struct String
	{
		static constexpr auto N = sizeof...(Chars);

		static_assert(N != 0, "Length cannot be zero.");
		static_assert(std::is_const_v<T>, "String char type must be const.");

		using BlockT = uint8_t; // BlockSelector<N>::template BlockT;
		using CipherT = CipherSelector<BlockT, 1U>;
		using KeyBuilderT = KeyBuilder<BlockT, T, RANDOM_UINT(BlockT, 4, 16)>;

		__forceinline explicit constexpr String() noexcept : 
			enc{}, encoded{ enc.Encode<Chars>()... } {}

		__forceinline char* Decode()
		{
			char* enc_p = (char*)encoded;

			for (volatile size_t i = 0; i < N; ++i)
			{
				enc_p[i] = static_cast<char>(enc.Decode(encoded[i]));
			}

			enc_p[N] = 0;

			return enc_p;
		}

		ConstantInteger<BlockT> enc;
		BlockT encoded[N + 1];
	};
}