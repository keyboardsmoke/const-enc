#include "doctest.h"

#include "constant.h"

#define HELLO_WORLD "Hello, World!\n"
#define HELLO_WORLD_LEN sizeof(HELLO_WORLD)

#define OBFUSCATE_STRING(x) ((obfuscate::StringEncoder<const char, sizeof(x)>(x, std::make_index_sequence<sizeof(x)>())))

__declspec(noinline) static void string_test()
{

    auto s = OBFUSCATE_STRING(HELLO_WORLD);
    auto c = s.Decode();

    printf("STRING [%s]\n", c);
}

TEST_CASE("String")
{
    string_test();
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