#include "doctest.h"

#include "constant.h"

#define OBFUSCATE_VALUE(t, x) 

__declspec(noinline) static void int_test()
{
    volatile uint64_t sane = 0x1337133713371337;
    auto v = obfuscate::ConstantInteger<uint64_t>();
    auto e = v.EncodeConstant(0x1337133713371337);

    [&]() {
        REQUIRE(sane == v.Decode(e));
    }();
}

TEST_CASE("Integer")
{
    int_test();
}