#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <thread>
#include <mutex>
#include <numeric>

#ifdef _WIN32
#include <Windows.h>
#endif // !1
std::mutex res_mtx;
int all_sum{};

template<class Iter>
using range = std::pair<Iter, Iter>;
template < class Iter >
using ranges = std::vector<range<Iter>>;
template<class Iter>
ranges<Iter> split_ranges(Iter begin, Iter end, size_t count) {
    assert(count);

    size_t const d = std::distance(begin, end);
    if (!d)
        return {};

    size_t const stride = std::distance(begin, end) / count;

    ranges<Iter> result;

    while (--count) {
        auto const first = begin;
        std::advance(begin, stride);
        result.emplace_back(first, begin);
    }

    if (std::distance(begin, end))
        result.emplace_back(begin, end);

    return result;
}
template<class Iter, class T = typename std::iterator_traits<Iter>::value_type>
void tread_sum(range<Iter> rng, T& result) {
    auto res = std::accumulate(rng.first, rng.second, T{});
    std::lock_guard<std::mutex>lck(res_mtx);
    result += res;
}

int main()
{/*
#ifdef _WIN32
    setlocale(LC_ALL, "");
    SetConsoleCP(GetACP()/*1251*///);
    /*SetConsoleOutputCP(GetACP()/*1251*//*);
#else
    std::ios_base::sync_with_studio(false);
    std::wcout.imbue(std::locale("en_US.UTF-8"));
    std::wcin.imbue(std::locale("en_US.UTF-8"));
#endif // _WIN32
    */
    /*------------------------------------------------*/
    std::vector<int>all_sum_mass(1000000);
    auto res = split_ranges(all_sum_mass.cbegin(), all_sum_mass.cend(), 5);
    std::vector<std::thread>threads;
    for (unsigned i = 0; i < all_sum_mass.size(); ++i) all_sum_mass[i] = i;
    for (auto&& rng : res) {
        threads.emplace_back(tread_sum<std::vector<int>::const_iterator>, rng, std::ref(all_sum));
    }
    for (auto&& thr : threads) {
        thr.join();
    }
    std::wcout << L"Общая сумма массива = " << all_sum<<std::endl;
    return 0;
}
