#pragma once

#include <cstdint>

namespace obfuscate
{
	namespace time
	{
		namespace _detail
		{
			template<typename T>
			constexpr static T GetMonthFromDate()
			{
				return (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n') ? 0
					: (__DATE__[0] == 'F' && __DATE__[1] == 'e' && __DATE__[2] == 'b') ? 1
					: (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') ? 2
					: (__DATE__[0] == 'A' && __DATE__[1] == 'p' && __DATE__[2] == 'b') ? 3
					: (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') ? 4
					: (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') ? 5
					: (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') ? 6
					: (__DATE__[0] == 'A' && __DATE__[1] == 'u' && __DATE__[2] == 'g') ? 7
					: (__DATE__[0] == 'S' && __DATE__[1] == 'e' && __DATE__[2] == 'p') ? 8
					: (__DATE__[0] == 'O' && __DATE__[1] == 'c' && __DATE__[2] == 't') ? 9
					: (__DATE__[0] == 'N' && __DATE__[1] == 'o' && __DATE__[2] == 'v') ? 10
					: (__DATE__[0] == 'D' && __DATE__[1] == 'e' && __DATE__[2] == 'c') ? 11 : (-1);
			}

			template<typename T>
			constexpr static T GetDayFromDate()
			{
				return (__DATE__[4] - '0' < 0 ? 0 : __DATE__[4] - '0') * 10 + __DATE__[5] - '0';
			}

			template<typename T>
			constexpr static T GetYearFromDate()
			{
				return (__DATE__[7] - '0') * 1000 + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + __DATE__[10] - '0';
			}

			template<typename T>
			constexpr static bool IsLeapYear(T year)
			{
				return (year % 4 != 0) ? false : (year % 100 != 0) ? true : (year % 400 != 0) ? false : true;
			}

			template<typename T, size_t Index>
			constexpr static T GetTimeBitValue()
			{
				constexpr uint8_t a = static_cast<uint8_t>(__TIME__[Index] - '0'); // char to integer value
				constexpr uint8_t b = static_cast<uint8_t>(__TIME__[Index + 1] - '0'); // char to integer value
				return a * 10 + b;
			}

			template<typename T>
			constexpr static T GetTimeBitSeconds()
			{
				return GetTimeBitValue<T, 6>();
			}

			template<typename T>
			constexpr static T GetTimeBitMinutes()
			{
				return GetTimeBitValue<T, 3>();
			}

			template<typename T>
			constexpr static T GetTimeBitHours()
			{
				return GetTimeBitValue<T, 0>();
			}

			template<class T>
			constexpr T get_yday(T month, T day, T year, T sum = 0)
			{
				return (month == 0) ? (sum + day) :
					(month == 1) ? (get_yday(month - 1, day, year, sum + (IsLeapYear<T>(year) ? 29 : 28))) :
					(month == 2) ? (get_yday(month - 1, day, year, sum + 31)) :
					(month == 3) ? (get_yday(month - 1, day, year, sum + 30)) :
					(month == 4) ? (get_yday(month - 1, day, year, sum + 31)) :
					(month == 5) ? (get_yday(month - 1, day, year, sum + 30)) :
					(month == 6) ? (get_yday(month - 1, day, year, sum + 31)) :
					(month == 7) ? (get_yday(month - 1, day, year, sum + 31)) :
					(month == 8) ? (get_yday(month - 1, day, year, sum + 30)) :
					(month == 9) ? (get_yday(month - 1, day, year, sum + 31)) :
					(month == 10) ? (get_yday(month - 1, day, year, sum + 30)) :
					(month == 11) ? (get_yday(month - 1, day, year, sum + 31)) : (-1);
			}

			template<class T>
			constexpr T CreateUnixTimestamp()
			{
				constexpr auto tm_sec = GetTimeBitSeconds<T>();
				constexpr auto tm_min = GetTimeBitMinutes<T>();
				constexpr auto tm_hour = GetTimeBitHours<T>();
				constexpr auto tm_day = GetDayFromDate<T>();
				constexpr auto tm_month = GetMonthFromDate<T>();
				constexpr auto tm_year = GetYearFromDate<T>();
				constexpr auto tm_yday = get_yday(tm_month, tm_day, tm_year);

				return tm_sec + tm_min * 60 + tm_hour * 3600 + tm_yday * 86400 + (tm_year - 70) * 31536000 + ((tm_year - 69) / 4) * 86400 - ((tm_year - 1) / 100) * 86400 + ((tm_year + 299) / 400) * 86400;
			}
		}

		template<typename T>
		constexpr static T Get()
		{
			return _detail::CreateUnixTimestamp<T>();
		}
	}
}