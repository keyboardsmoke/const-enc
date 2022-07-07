#include "doctest.h"
#include "constant.h"

#define OBFUSCATE_STRING(x) ((obfuscate::String<const char, sizeof(x)>{ std::make_index_sequence<sizeof(x)>(), x}).Decode())

__declspec(noinline) static void string_test()
{
    printf("STRING [%s]\n", OBFUSCATE_STRING("Hello, World!"));
}

TEST_CASE("String")
{
    string_test();
}