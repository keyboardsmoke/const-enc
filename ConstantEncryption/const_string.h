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
	template<typename T, size_t N>
	struct String
	{
		static_assert(N != 0, "Length cannot be zero.");
		static_assert(std::is_const_v<T>, "String char type must be const.");

		static constexpr bool IsBlock8 = (N == 1);
		static constexpr bool IsBlock16 = ((N % 2) == 0);
		static constexpr bool IsBlock32 = ((N % 4) == 0);
		static constexpr bool IsBlock64 = ((N % 8) == 0);

		// Select the granularity to encrypt the string at
		// It can use uint8, uint16, uint32, uint64
		using BlockT =
			std::conditional_t<IsBlock64 == true, uint64_t,
			std::conditional_t<IsBlock32 == true, uint32_t, uint8_t>>;
			//std::conditional_t<IsBlock16 == true, uint16_t, uint8_t>>>>;

		static constexpr size_t BlockSize = sizeof(BlockT);

		struct StringGetter
		{
			__forceinline constexpr StringGetter(T* S) : S(S) {}
			__forceinline constexpr T* value() { return S; }

			T* S;
		};

		struct ForceEncoder
		{
			template<size_t Index>
			__forceinline static constexpr BlockT EncodeValue(const ConstantInteger<BlockT>& Enc, T (&S)[N])
			{
				
				// TODO: WTF! This says: expression did not evaluate to a constant
				// constexpr auto a = S[Index];

				// TODO: WTF! This says: expected compile-time constant expression if i try to pass directly
				// return Enc.EncodeConstant<a>();

				// However, if I use the runtime version... it does a compile-time expression of that!?!?!?!
				// This is so dumb!
				return Enc.EncodeConstant(S[Index]);
			}
		};

		template<size_t... Indices>
		__forceinline explicit constexpr String(std::index_sequence<Indices...>, T (&S)[N]) noexcept :
			enc{}, encoded{ (ForceEncoder::EncodeValue<Indices>(enc, S))... }
		{
		}

		__forceinline T* Decode()
		{
			using NonConstT = std::remove_const_t<T>;

			NonConstT* enc_p = (NonConstT*)encoded;

			for (volatile size_t i = 0; i < N; ++i)
			{
				enc_p[i] = static_cast<NonConstT>(enc.Decode(encoded[i]));
			}

			enc_p[N] = 0;

			return enc_p;
		}

		ConstantInteger<BlockT> enc;
		BlockT encoded[N];
	};
}