#pragma once

namespace obfuscate
{
    template<typename T>
    struct ConstantInteger
    {
        static_assert(std::is_integral<T>::value, "Integral required.");

        using LimitsT = std::numeric_limits<T>;
        using StorageT = std::remove_const_t<std::make_unsigned_t<T>>;

        static constexpr size_t KeyCount = RANDOM_UINT(StorageT, 8, 16);

        KeyBuilder<StorageT, T, KeyCount> m_keys;

        __forceinline explicit constexpr ConstantInteger() noexcept : m_keys() { }

        constexpr __forceinline StorageT EncodeConstant(T value) const noexcept
        {
            return m_keys.GetEncodedValueConstant(value);
        }

        constexpr __forceinline StorageT Encode(T value) const noexcept
        {
            return m_keys.GetEncodedValueRuntime(value);
        }

        constexpr __forceinline T Decode(StorageT encoded) const noexcept
        {
            volatile T a = static_cast<T>(m_keys.GetDecodedValueRuntime(encoded));
            return a;
        }
    };
}