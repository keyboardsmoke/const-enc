#include "doctest.h"
#include "constant.h"

#define OBFUSCATE_STRING(x) ((obfuscate::String<const char, sizeof(x)>{ std::make_index_sequence<sizeof(x)>(), x}).Decode())
#define OBFUSCATE_WIDE_STRING(x) ((obfuscate::String<const wchar_t, sizeof(x) / 2>{ std::make_index_sequence<sizeof(x) / 2>(), x}).Decode());

__declspec(noinline) static void string_test()
{
    const char* s = OBFUSCATE_STRING("Hello, World!");

    printf("STRING [%s]\n", s);

    const wchar_t* w = OBFUSCATE_WIDE_STRING(L"Hello, World!");

    printf("STRING WIDE [%S]\n", w);

    [&]() {
        REQUIRE(strcmp(s, "Hello, World!") == 0);
    }();
}

TEST_CASE("String")
{
    string_test();
}