#pragma once

#include <array>
#include <type_traits>

namespace obfuscate
{
	namespace constant
	{
		template<typename T, T a>
		struct XorShift;

		template<uint32_t a>
		struct XorShift<uint32_t, a>
		{
			static constexpr uint32_t Get()
			{
				uint32_t x = a;
				x ^= x << 13;
				x ^= x >> 17;
				x ^= x << 5;
				return x;
			}
		};

		template<uint64_t a>
		struct XorShift<uint64_t, a>
		{
			static constexpr uint64_t Get()
			{
				uint64_t r = a;
				r ^= (r << 21);
				r ^= (r >> 35);
				r ^= (r << 4);
				return r;
			}
		};

		namespace hash
		{
			template<typename T, size_t N>
			struct FNV1A
			{
				constexpr static T hash(const char* data, T basis = static_cast<T>(2166136261))
				{
					return FNV1A<T, N - 1>::hash(data + 1, (basis ^ static_cast<T>(data[0])) * static_cast<T>(16777619));
				}
			};

			struct CRC32
			{
				template <unsigned c, int k = 8>
				struct f : f<((c & 1) ? 0xedb88320 : 0) ^ (c >> 1), k - 1> {};
				template <unsigned c> struct f<c, 0> { enum : uint32_t { value = c }; };

#define A(x) B(x) B(x + 128)
#define B(x) C(x) C(x +  64)
#define C(x) D(x) D(x +  32)
#define D(x) E(x) E(x +  16)
#define E(x) F(x) F(x +   8)
#define F(x) G(x) G(x +   4)
#define G(x) H(x) H(x +   2)
#define H(x) I(x) I(x +   1)
#define I(x) f<x>::value ,

				constexpr static uint32_t crc_table[] = { A(0) };

#undef A
#undef B
#undef C
#undef D
#undef E
#undef F
#undef G
#undef H
#undef I

				constexpr static uint32_t crc32_impl(const char* p, size_t len, uint32_t crc)
				{
					return len ?
						crc32_impl(p + 1, len - 1, (crc >> 8) ^ crc_table[(crc & 0xFF) ^ *p])
						: crc;
				}

				constexpr static uint32_t crc32(const char* data, size_t length)
				{
					return ~crc32_impl(data, length, ~0);
				}

				constexpr static size_t strlen_c(const char* str)
				{
					return *str ? 1 + strlen_c(str + 1) : 0;
				}

				constexpr static uint32_t WSID(const char* str)
				{
					return crc32(str, strlen_c(str));
				}
			};

			struct CRC64
			{
				template <uint64_t c, int k = 8>
				struct f : f<((c & 1) ? 0x42F0E1EBA9EA3693ULL : 0) ^ (c >> 1), k - 1> {};
				template <uint64_t c> struct f<c, 0> { enum : uint64_t { value = c }; };

#define A(x) B(x) B(x + 128)
#define B(x) C(x) C(x +  64)
#define C(x) D(x) D(x +  32)
#define D(x) E(x) E(x +  16)
#define E(x) F(x) F(x +   8)
#define F(x) G(x) G(x +   4)
#define G(x) H(x) H(x +   2)
#define H(x) I(x) I(x +   1)
#define I(x) f<x>::value ,

				constexpr static uint64_t crc_table[] = { A(0) };

#undef A
#undef B
#undef C
#undef D
#undef E
#undef F
#undef G
#undef H
#undef I

				constexpr static uint64_t crc64_impl(const char* p, size_t len, uint64_t crc)
				{
					return len ?
						crc64_impl(p + 1, len - 1, (crc >> 8) ^ crc_table[(crc & 0xFF) ^ *p])
						: crc;
				}

				constexpr static uint64_t crc64(const char* data, size_t length)
				{
					return ~crc64_impl(data, length, ~0);
				}

				constexpr static size_t strlen_c(const char* str)
				{
					return *str ? 1 + strlen_c(str + 1) : 0;
				}

				constexpr static uint64_t WSID(const char* str)
				{
					return crc64(str, strlen_c(str));
				}
			};
		}
	}
}