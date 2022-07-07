#pragma once

#include "const_array.h"
#include "const_hash.h"
#include "const_random.h"

namespace obfuscate
{
    // TODO: CipherT would allow for more than XOR.

    enum class Ciphers
    {
        Xor,
        SubAdd,
        Caesar
    };

    template<typename DataT, typename KeyT, Ciphers CipherType>
    struct Cipher;

    template<typename DataT, typename KeyT>
    struct Cipher<DataT, KeyT, Ciphers::Xor>
    {
        static constexpr KeyT Encode(const DataT value, const KeyT key)
        {
            return key ^ value;
        }

        static constexpr DataT Decode(const KeyT value, const KeyT key)
        {
            return key ^ value;
        }
    };

    template<typename DataT, typename KeyT>
    struct Cipher<DataT, KeyT, Ciphers::SubAdd>
    {
        static constexpr KeyT Encode(const DataT value, const KeyT key)
        {
            return value - key;
        }

        static constexpr DataT Decode(const KeyT value, const KeyT key)
        {
            return value + key;
        }
    };

    template<typename KeyT, typename DataT, size_t KeyCount>
    struct KeyBuilder
    {
        constexpr static size_t KeyMax = KeyCount;

        using CipherT = Cipher<DataT, KeyT, static_cast<Ciphers>(RANDOM_UINT(size_t, 0, 1))>;

        // Constructor
        __forceinline constexpr KeyBuilder() :
            m_keys { InitializeKeyArray(std::make_index_sequence<KeyCount>()) } {}

        __forceinline constexpr size_t GetKeyCount() const { return KeyCount; }

        template<size_t Index>
        __forceinline static constexpr KeyT GetKey()
        {
            return random::RandomGenerator<KeyT, Index>::Generate();
        }

        __forceinline KeyT GetKeyRuntime(size_t index)
        {
            return m_keys[index];
        }

        __forceinline constexpr DataT GetEncodedValueRuntime(const DataT& value) const
        {
            volatile DataT ret = value;

            for (size_t i = 0; i < m_keys.size(); ++i)
            {
                ret = CipherT::Encode(ret, m_keys[i]);
            }

            return ret;
        }

        __forceinline DataT GetDecodedValueRuntime(const DataT& value) const
        {
            volatile std::remove_const_t<DataT> ret = value;

            for (auto i = m_keys.size(); i--;)
            {
                ret = CipherT::Encode(ret, m_keys[i]);
            }

            return ret;
        }

        template<size_t Index>
        struct KeyApplicator
        {
            constexpr KeyApplicator(DataT value) : value(value) {}

            __forceinline constexpr DataT GetValue()
            {
                using NextApplicatorT = KeyApplicator<Index + 1>;

                return NextApplicatorT(CipherT::Encode(value, random::RandomGenerator<KeyT, Index>::Generate())).GetValue();
            }

            DataT value;
        };

        template<>
        struct KeyApplicator<KeyCount>
        {
            constexpr KeyApplicator(DataT value) : value(value) {}

            __forceinline constexpr DataT GetValue()
            {
                return value;
            }

            DataT value;
        };

        __forceinline constexpr DataT GetEncodedValueConstant(DataT value) const
        {
            using ApplicatorT = KeyApplicator<0U>;

            volatile auto v = ApplicatorT(value).GetValue();

            return v;
        }

    private:
        template<std::size_t... Is>
        __forceinline constexpr SimpleArray<KeyT, KeyCount> InitializeKeyArray(std::index_sequence<Is...>)
        {
            return SimpleArray<KeyT, KeyCount> { static_cast<KeyT>(GetKey<Is>())... };
        }

        SimpleArray<KeyT, KeyCount> m_keys;
    };
}