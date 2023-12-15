#include <cassert>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <stdint.h>

void show_ip_pool(std::vector<std::vector<uint8_t>>& ip_pool)
{
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part) {
            if (ip_part != ip->cbegin()) {
                std::cout << ".";

            }
            std::cout << (int)*ip_part;
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<uint8_t>> filter_any(std::vector<std::vector<uint8_t>>& ip_pool, uint8_t arg)
{
    std::vector<std::vector<uint8_t>> ip_pool_after_filter;

    for (auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            if (arg == *ip_part) {
                ip_pool_after_filter.push_back(*ip);
                break;
            }
    }

    return ip_pool_after_filter;
}

std::vector<std::vector<uint8_t>> filter_for_block(std::vector<std::vector<uint8_t>>& ip_pool, uint8_t arg, uint8_t num)
{
    std::vector<std::vector<uint8_t>> ip_pool_after_filter;

    for (auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        auto ip_part = ip->cbegin();
        if (num> ip->size() - 1)
            continue;;
        ip_part += num;
        if (arg == *ip_part) {
            ip_pool_after_filter.push_back(*ip);
        }
    }

    return ip_pool_after_filter;
}

template <typename... T>
std::vector<std::vector<uint8_t>> filter(std::vector<std::vector<uint8_t>>& ip_pool, const T&... args)
{
    std::vector<std::vector<uint8_t>> ip_pool_after_filter = ip_pool;
    int block_count = 0;

    for (auto p : std::initializer_list<int>{args...}) {
        ip_pool_after_filter = filter_for_block(ip_pool_after_filter, p, block_count);
        block_count++;
    }

    return ip_pool_after_filter;
}

std::vector<uint8_t> split(const std::string &str, char d)
{
    std::vector<uint8_t> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos) {
        r.push_back(std::stoi(str.substr(start, stop - start)));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(std::stoi(str.substr(start)));
    
    return r;
}

int main(int argc, char const *argv[])
{
    try
    {
        std::vector<std::vector<uint8_t>> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            ip_pool.push_back(split(line, '.'));
        }

        std::sort(begin(ip_pool), end(ip_pool),
            [](std::vector<uint8_t> left, std::vector<uint8_t> right) {
                std::vector<uint8_t>::const_iterator ip_part_left;
                std::vector<uint8_t>::const_iterator ip_part_rigth;
                for(ip_part_left = left.cbegin(), ip_part_rigth = right.cbegin(); ip_part_left != left.cend(); ++ip_part_left, ++ip_part_rigth) {
                    if (*ip_part_left > *ip_part_rigth)
                        return true;
                
                    if (*ip_part_left != *ip_part_rigth)
                         break;
                }
                return false;
            }
        );

        show_ip_pool(ip_pool);

        std::vector<std::vector<uint8_t>> ip;

        ip = filter(ip_pool, 1);
        show_ip_pool(ip);

        ip = filter(ip_pool, 46, 70);
        show_ip_pool(ip);

        ip = filter_any(ip_pool, 46);
        show_ip_pool(ip);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
