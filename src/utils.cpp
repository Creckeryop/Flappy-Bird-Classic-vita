/*this code was written by creckeryop*/
#include <chrono>
#include "utils.h"

using namespace std::chrono;
long long get_time()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

long long atoi_m(const char* c)
{
	long long tmp = 0;
	while (*c && !(*c - 48 < 10 && *c - 48 >= 0)) c++;
	while (*c)
	{
		tmp *= 10;
		if (*c - 48 < 10 && *c - 48 >= 0)
			tmp += (*(c++) - 48);
		else
			return tmp/10;
	}
	return tmp;
}
void itoa_m(long long n, char* buff)
{
	long long tmp = n, len = 1;
	while (tmp / 10 != 0) tmp /= 10, len++;
	buff += len - 1;
	*(buff + 1) = '\0';
	do {
		*buff = n % 10+48;
		buff -= 1;
		n /= 10;
	} while (n != 0);
}
float mod(float a, int x)
{
    while (a<0) a+=x;
    while (a>x) a-=x;
    return a;
}