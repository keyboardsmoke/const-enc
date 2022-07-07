#pragma once

namespace obfuscate
{
	template<typename T, size_t N>
	struct String
	{
		static_assert(N != 0, "Length cannot be zero.");
		static_assert(std::is_const_v<T>, "String char type must be const.");

		static constexpr size_t DataSize = N * sizeof(T);

		static constexpr bool IsBlock8 = (DataSize == 1);
		static constexpr bool IsBlock16 = ((DataSize % 2) == 0);
		static constexpr bool IsBlock32 = ((DataSize % 4) == 0);
		static constexpr bool IsBlock64 = ((DataSize % 8) == 0);

		// Select the granularity to encrypt the string at
		// It can use uint8, uint16, uint32, uint64
		using BlockT =
			std::conditional_t<IsBlock8 == true, uint8_t,
			std::conditional_t<IsBlock64 == true, uint64_t,
			std::conditional_t<IsBlock32 == true, uint32_t,
			std::conditional_t<IsBlock16 == true, uint16_t, void>>>>;

		static_assert(!std::is_same_v<BlockT, void>, "Invalid input size.");

		static constexpr size_t BlockSize = sizeof(BlockT);

		template<size_t... Indices>
		__forceinline explicit constexpr String(std::index_sequence<Indices...>, T (&S)[N]) noexcept :
			enc{}, encoded{ (enc.EncodeConstant(S[Indices]))... }
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