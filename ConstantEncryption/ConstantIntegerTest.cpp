#include "doctest.h"

#include "constant.h"

#define OBFUSCATE_VALUE(t, x) obfuscate::ConstantInteger<t, x>().Decode()

// obfuscate::Constant<t>(x).Decode();

/*
__declspec(noinline) static void int_test()
{
    volatile uint64_t sane = 0x1337133713371337;
    volatile uint64_t v = OBFUSCATE_VALUE(uint64_t, 0x1337133713371337);

    if (sane == v)
        printf("[OK!] ");
    else
        printf("[!!! ERROR !!!] ");

    volatile auto __ = printf("uint64 = 0x%I64X (sanity: 0x%I64X)\n", v, sane);
}*/

TEST_CASE("Integer")
{
    //int_test();
}

/*
#define MAKE_CONSTANT_SUBCLASS(vt, sc) namespace obfuscate { template<> struct Constant<vt> : public sc<vt> { \
        Constant(vt value) : sc(value) {} \
    }; }

// MAKE_CONSTANT_SUBCLASS(__int8, ConstantInteger);
// MAKE_CONSTANT_SUBCLASS(unsigned __int8, ConstantInteger);
// MAKE_CONSTANT_SUBCLASS(__int16, ConstantInteger);
// MAKE_CONSTANT_SUBCLASS(unsigned __int16, ConstantInteger);
// MAKE_CONSTANT_SUBCLASS(__int32, ConstantInteger);
MAKE_CONSTANT_SUBCLASS(unsigned __int32, ConstantInteger);
// MAKE_CONSTANT_SUBCLASS(__int64, ConstantInteger);
MAKE_CONSTANT_SUBCLASS(unsigned __int64, ConstantInteger);

#undef MAKE_CONSTANT_SUBCLASS
*/