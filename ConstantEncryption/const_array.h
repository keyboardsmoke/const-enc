#pragma once

#include <cstring>
#include <string>
#include <typeinfo>

#define STATIC_SIZE_CHECK(SizeValue) static_assert((SizeValue) < 0x7fffffff, "Total size of array must not exceed 0x7fffffff");

namespace obfuscate
{
	template<typename ArrayT, size_t N>
	class SimpleArray
	{
		STATIC_SIZE_CHECK(N);

	public:

		using type = ArrayT;

		// Iterator implementations
		struct iterator {
			constexpr iterator(const SimpleArray<ArrayT, N>& Array, const size_t Index) : m_arr(Array), m_index(Index) {}

			constexpr void operator++() { ++m_index; }
			constexpr void operator--() { --m_index; }
			constexpr ArrayT const& operator* () const { return m_arr[m_index]; }
			constexpr bool operator==(const iterator& rhs) const { return m_index == rhs.m_index; }
			constexpr bool operator!=(const iterator& rhs) const { return !(*this == rhs); }

		private:
			size_t m_index;
			SimpleArray<ArrayT, N> m_arr;
		};

		constexpr iterator begin() const { return iterator(this, 0U); }
		constexpr iterator end() const { return iterator(this, N + 1); }

		SimpleArray() = delete;

		template<typename... Values>
		constexpr SimpleArray(const Values&... values) : raw_data{ values... } {}

		constexpr char* data()
		{
			return reinterpret_cast<char*>(raw_data);
		}

		constexpr ArrayT* udata()
		{
			return raw_data;
		}

		constexpr size_t size() const
		{
			return N;
		}

		constexpr ArrayT operator[](size_t index) const
		{
			return raw_data[index];
		}

		constexpr ArrayT& operator[](size_t index)
		{
			return raw_data[index];
		}

		ArrayT raw_data[N];
	};
}