#pragma once

#include <limits>

namespace obfuscate
{
    namespace random
    {
        static constexpr uint8_t seed8 = static_cast<uint8_t>(time::Get<uint32_t>());
        static constexpr uint32_t seed32 = time::Get<uint32_t>();
        static constexpr uint64_t seed64 = time::Get<uint64_t>();

        template<typename T, size_t Counter>
        struct RandomGenerator;

        template<size_t Counter> struct RandomGenerator<uint8_t, Counter>
        {
            static constexpr uint8_t Generate()
            {
                constexpr uint32_t local_seed = RandomGenerator<uint32_t, Counter - 1>::Generate();

                return static_cast<uint8_t>(local_seed * (Counter + 1));
            }
        };

        template<> struct RandomGenerator<uint8_t, 0UL>
        {
            static constexpr uint8_t Generate()
            {
                return seed8;
            }
        };

        template<size_t Counter> struct RandomGenerator<uint32_t, Counter>
        {
            static constexpr uint32_t Generate()
            {
                constexpr uint32_t local_seed = RandomGenerator<uint32_t, Counter - 1>::Generate();

                // implemented with G. Carta's optimisation: with 32-bit math and without division
                // https://www.firstpr.com.au/dsp/rand31/

                // The lowest such number for an LCG with a modulus constant of (231 - 1) is 16807.  
                // According to PM88 there are over 534 million other numbers which are also full period multipliers for a modulus constant of (231 - 1).
                // Lewis, Goodman and Miller (not Keith Miller) first suggested 16807 in 1969.
                constexpr uint32_t a = 16807;
                constexpr uint32_t lo_p1 = a * (local_seed & 0xffff);
                constexpr uint32_t hi_p1 = a * (local_seed >> 16);
                constexpr uint32_t lo_p2 = lo_p1 + ((hi_p1 & 0x7fff) << 16);
                constexpr uint32_t lo_p3 = lo_p2 + hi_p1;
                constexpr uint32_t losub = (lo_p3 > 0x7fffffff) ? lo_p3 - 0x7fffffff : lo_p3;
                
                return losub;
            }
        };

        template<> struct RandomGenerator<uint32_t, 0UL>
        {
            static constexpr uint32_t Generate()
            {
                return seed32;
            }
        };

        template<size_t Counter> struct RandomGenerator<uint64_t, Counter>
        {
            static constexpr uint64_t Generate()
            {
                constexpr uint64_t local_seed = RandomGenerator<uint64_t, Counter - 1>::Generate();

                // implemented with G. Carta's optimisation: with 32-bit math and without division
                // https://www.firstpr.com.au/dsp/rand31/
                // expanded to 64-bit by Andy :))

                constexpr uint64_t a = 1844674407370955161; // 2^64-1
                constexpr uint64_t lo_p1 = a * (local_seed & 0xffffffff);
                constexpr uint64_t hi_p1 = a * (local_seed >> 32);
                constexpr uint64_t lo_p2 = lo_p1 + ((hi_p1 & 0x7fffffff) << 32);
                constexpr uint64_t lo_p3 = lo_p2 + hi_p1;
                constexpr uint64_t losub = (lo_p3 > 0x7fffffffffffffff) ? lo_p3 - 0x7fffffffffffffff : lo_p3;
                return losub;
            }
        };

        template<> struct RandomGenerator<uint64_t, 0ULL>
        {
            static constexpr uint64_t Generate()
            {
                return seed64;
            }
        };
    }
}

// random::RandomGenerator<KeyT, Counter + Index>::Generate();

#define RANDOM_UINT(type, minrand, maxrand) (((obfuscate::random::RandomGenerator<type, __COUNTER__>::Generate()) % maxrand) + minrand)