#pragma once

#include <array>

namespace obfuscate
{
	template<typename BlockT>
	class XorCipher
	{
	public:
		static constexpr BlockT Encode(BlockT Input, BlockT Key, size_t Index)
		{
			return Input ^ Key;
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
		static constexpr BlockT Encode(BlockT Input, BlockT Key, size_t Index)
		{
			return Input + Key;
		}

		static constexpr BlockT Decode(BlockT input, BlockT key, size_t index)
		{
			return input - key;
		}
	};

	template<typename BlockT, size_t CipherIndex>
	struct CipherSelector
	{
		using type =
			std::conditional_t<CipherIndex == 0U, XorCipher<BlockT>,
			std::conditional_t<CipherIndex == 1U, AddSubCipher<BlockT>, void>>;
	};

	template<typename T, T* S, size_t Index>
	struct IndexSelector
	{
		static constexpr T I = S[Index];
	};

    template<typename CharT, size_t N>
	struct StringEncoder
	{
		// These need to be randomized
		using BlockT = uint32_t;
		using CipherT = CipherSelector<BlockT, RANDOM_UINT(size_t, 0, 1)>::type;
		using KeyBuilderT = KeyBuilder<BlockT, BlockT, RANDOM_UINT(size_t, 4, 10)>;

		// These will not change
		using IndexT = std::make_index_sequence<N + 1>;

		template<size_t N, size_t... Is>
		explicit constexpr StringEncoder(CharT(&S)[N], std::index_sequence<Is...>) noexcept :
			kb{}, data {
			(CipherT::Encode(S[Is], KeyBuilderT::GetKey<Is>(), Is))..., '\0'
		} {}

		BlockT* GetData()
		{
			return data;
		}
		
		std::remove_const_t<CharT>* Decode()
		{
			std::remove_const_t<CharT> decoded[N + 1]{ 0 };

			for (volatile size_t i = 0; i < N; ++i)
			{
				volatile CharT decode = CipherT::Decode(data[i], kb.GetKeyRuntime(i), i);

				decoded[i] = decode;
			}

			return decoded;
		}

		KeyBuilderT kb;
		BlockT data[N + 1];
	};
}