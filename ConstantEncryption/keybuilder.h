#pragma once

#include "const_array.h"
#include "const_hash.h"
#include "const_random.h"

namespace obfuscate
{
    template<typename KeyT, typename DataT, size_t KeyCount>
    struct KeyBuilder
    {
        constexpr static size_t KeyMax = KeyCount;

        using KeyType = KeyT;
        using DataType = DataT;

        // Constructor
        constexpr KeyBuilder() :
            m_keys { InitializeKeyArray(std::make_index_sequence<KeyCount>()) } {}

        constexpr size_t GetKeyCount() const { return KeyCount; }

        template<size_t Index>
        static constexpr KeyT GetKey()
        {
            return random::RandomGenerator<KeyT, Index>::Generate();
        }

        KeyT GetKeyRuntime(size_t index)
        {
            return m_keys[index];
        }

        __declspec(noinline) constexpr DataT GetEncodedValueRuntime(const DataT& value) const
        {
            volatile DataT ret = value;

            for (size_t i = 0; i < m_keys.size(); ++i)
            {
                ret ^= m_keys[i];
            }

            return ret;
        }

        __forceinline DataT GetDecodedValueRuntime(const DataT& value)
        {
            volatile std::remove_const_t<DataT> ret = value;

            for (auto i = m_keys.size(); i--;)
            {
                ret ^= m_keys[i];
            }

            return ret;
        }

        template<size_t Index>
        struct KeyApplicator
        {
            template<DataT value>
            __forceinline constexpr DataT GetValue()
            {
                volatile constexpr KeyT a = random::RandomGenerator<KeyT, Index>::Generate();
                volatile constexpr DataT nextValue = static_cast<DataT>(a ^ value);

                using NextApplicatorT = KeyApplicator<Index + 1>;

                return NextApplicatorT().GetValue<nextValue>();
            }
        };

        template<>
        struct KeyApplicator<KeyCount>
        {
            // We need to pass index_sequence for the key sequence
            // then we can go thru them.... yeesh
            template<DataT value>
            __forceinline constexpr DataT GetValue()
            {
                return value;
            }
        };

        template<DataT value>
        __forceinline constexpr DataT GetEncodedValueConstant() const
        {
            using ApplicatorT = KeyApplicator<0U>;

            volatile auto v = ApplicatorT().GetValue<value>();

            return v;
        }

    private:
        template<typename T, size_t N, size_t... Is>
        constexpr auto InitializeKeyNativeArray(std::index_sequence<Is...>)
        {
            return { static_cast<KeyT>(random::RandomGenerator<KeyT, Is>::Generate())... };
        }

        template<std::size_t... Is>
        constexpr SimpleArray<KeyT, KeyCount> InitializeKeyArray(std::index_sequence<Is...>)
        {
            return SimpleArray<KeyT, KeyCount> { static_cast<KeyT>(GetKey<Is>())... };
        }

        SimpleArray<KeyT, KeyCount> m_keys;
    };
}