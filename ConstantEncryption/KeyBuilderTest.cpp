#include "doctest.h"
#include "constant.h"

__declspec(noinline) static void TestKeyBuilder32()
{
    using KeyT = uint32_t;
    using DataT = uint32_t;

    constexpr DataT valueUL = 1337UL;
    obfuscate::KeyBuilder<KeyT, DataT, 4> kb;
    printf("keyBuilderUL Key Builder [%X, %X, %X, %X]\n", kb.GetKey<0>(), kb.GetKey<1>(), kb.GetKey<2>(), kb.GetKey<3>());

    constexpr DataT ceUL = kb.GetEncodedValueConstant<1337UL>(); // Wrong...
    // printf("uint32_t Encoded [Constant]: %d\n", ceUL);

    DataT reUL = kb.GetEncodedValueRuntime(valueUL); // Correct
    // printf("uint32_t Encoded [Runtime]: %d\n", reUL);

    DataT rdUL = kb.GetDecodedValueRuntime(reUL);
    // printf("uint32_t Decoded [Runtime]: %d\n", rdUL);

    DataT cdUL = kb.GetDecodedValueRuntime(ceUL);
    // printf("uint32_t Decoded [Constant]: %d\n", cdUL);

    printf("[32] Encoded C [0x%X] R [0x%X], Decoded C [0x%X] R [0x%X]\n", ceUL, reUL, cdUL, rdUL);

    [&]() {
        REQUIRE(ceUL == reUL);
        REQUIRE(valueUL == rdUL);
    }();
}

__declspec(noinline) static void TestKeyBuilder64()
{
    using KeyT = uint64_t;
    using DataT = uint64_t;

    constexpr DataT valueULL = 1337ULL;
    obfuscate::KeyBuilder<KeyT, DataT, 4> kb;
    printf("keyBuilderULL Key Builder [%I64X, %I64X, %I64X, %I64X]\n", kb.GetKey<0>(), kb.GetKey<1>(), kb.GetKey<2>(), kb.GetKey<3>());

    constexpr DataT ceULL = kb.GetEncodedValueConstant<1337ULL>();
    // printf("uint64_t Encoded [Constant]: %I64d\n", ceULL);

    DataT reULL = kb.GetEncodedValueRuntime(valueULL);
    // printf("uint64_t Encoded [Runtime]: %I64d\n", reULL);

    DataT rdULL = kb.GetDecodedValueRuntime(reULL);
    // printf("uint64_t Decoded [Runtime]: %I64d\n", rdULL);

    DataT cdULL = kb.GetDecodedValueRuntime(ceULL);
    // printf("uint64_t Decoded [Constant]: %I64d\n", cdULL);

    printf("[64] Encoded C [0x%I64X] R [0x%I64X], Decoded C [0x%I64X] R [0x%I64X]\n", ceULL, reULL, cdULL, rdULL);

    // Decoded properly...
    [&]() {
        REQUIRE(ceULL == reULL);
        REQUIRE(valueULL == rdULL);
        REQUIRE(cdULL == rdULL);
    }();
}

TEST_CASE("KeyBuilder")
{
    TestKeyBuilder32();
    TestKeyBuilder64();
}