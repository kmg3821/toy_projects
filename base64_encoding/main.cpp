#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

#include <omp.h>

int main()
{
    freopen("test.txt", "r", stdin);
    std::string str;
    std::cin >> str;
    int cnt = 0;
    while (str.length() % 3 != 0)
    {
        str.push_back(0);
        cnt++;
    }

    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    typedef union
    {
        uint8_t data[3];
        struct
        {
            uint32_t b1 : 6;
            uint32_t b2 : 6;
            uint32_t b3 : 6;
            uint32_t b4 : 6;
        };
    } parser;
    const int n = str.length() / 3;
    std::vector<parser> parsers(n);
    std::string buf(4 * n, 0);

    for (int tc = 1; tc <= 100; ++tc)
    {
        const auto t0 = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
        for (int i = 0; i < n; ++i)
        {
            parsers[i].data[0] = str[3 * i + 2];
            parsers[i].data[1] = str[3 * i + 1];
            parsers[i].data[2] = str[3 * i + 0];

            buf[4 * (n - 1 - i) + 0] = base64[parsers[i].b1];
            buf[4 * (n - 1 - i) + 1] = base64[parsers[i].b2];
            buf[4 * (n - 1 - i) + 2] = base64[parsers[i].b3];
            buf[4 * (n - 1 - i) + 3] = base64[parsers[i].b4];
        }

        for (int i = 0; i < cnt; ++i)
        {
            buf[i] = '=';
        }
        std::reverse(buf.begin(), buf.end());
        const auto t1 = std::chrono::high_resolution_clock::now();

        std::cout << "#" << tc << "\n";
        //std::cout << buf << "\n";
        std::cout << std::chrono::duration<double>(t1 - t0).count() << " sec\n";
    }

    return 0;
}
