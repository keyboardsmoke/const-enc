#include "doctest.h"
#include "constant.h"

#define HELLO_WORLD "Hello, World!\n"
#define HELLO_WORLD_LEN sizeof(HELLO_WORLD)

#define EXPAND_STRING(x)
#define OBFUSCATE_STRING(x) obfuscate::String<const char, x>().Decode()

template<typename T, typename A, size_t N>
struct StringExpander
{
    template <T... chars> struct exploded_string
    {
        using type = obfuscate::String<T, chars...>;
    };

    template<typename StrProvider, size_t len, T... chars> struct explode_impl
    {
        using result = typename explode_impl<StrProvider, len - 1, StrProvider::str[len - 1], chars... > ::result;
    };

    template<typename StrProvider, T... chars> struct explode_impl<StrProvider, 0, chars...>
    {
        using result = exploded_string<chars...>;
    };

    struct StringProvider
    {
        static constexpr T* str = A::data();
        static constexpr size_t length = N;
    };

    using explode = explode_impl<StringProvider, N>;
};

/*
// helper function
constexpr unsigned c_strlen( char const* str, unsigned count = 0 )
{
    return ('\0' == str[0]) ? count : c_strlen(str+1, count+1);
}

// destination "template string" type
template < char... chars >
struct exploded_string
{
    static void print()
    {
        char const str[] = { chars... };
        std::cout.write(str, sizeof(str));
    }
};

// struct to explode a `char const*` to an `exploded_string` type
template < typename StrProvider, unsigned len, char... chars  >
struct explode_impl
{
    using result =
        typename explode_impl < StrProvider, len-1,
                                StrProvider::str()[len-1],
                                chars... > :: result;
};

    // recursion end
    template < typename StrProvider, char... chars >
    struct explode_impl < StrProvider, 0, chars... >
    {
         using result = exploded_string < chars... >;
    };

// syntactical sugar
template < typename StrProvider >
using explode =
    typename explode_impl < StrProvider,
                            c_strlen(StrProvider::str()) > :: result;
*/

template<typename T>
struct BuildArgument
{
    //
};

__declspec(noinline) static void string_test()
{
    struct p { static constexpr const char* data() { return "Hello, World!"; } };
    printf("STRING [%s]\n", StringExpander<const char, p, 14 >::explode::result::type().Decode());
    printf("STRING [%s]\n", obfuscate::String<const char, 'A', 'n', 'd', 'y', '\0'>().Decode());
}

TEST_CASE("String")
{
    string_test();
}