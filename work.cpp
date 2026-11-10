#include "work.h"

#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <syncstream>
#include <thread>

namespace {

constexpr std::chrono::seconds quickDuration{1};
constexpr std::chrono::seconds slowDuration{7};

void calculate(std::string_view name, std::chrono::seconds duration)
{
    std::this_thread::sleep_for(duration);
    std::osyncstream(std::cout) << name << std::endl;
}

} // namespace

void work()
{
    const auto start = std::chrono::steady_clock::now();

    std::future<void> a2 = std::async(
        std::launch::async, calculate, "A2", slowDuration);

    calculate("A1", quickDuration);
    calculate("C1", quickDuration);
    calculate("C2", quickDuration);
    calculate("D2", quickDuration);

    // A1 уже завершено. Перед B чекаємо завершення A2.
    a2.get();
    calculate("B", slowDuration);
    calculate("D1", quickDuration);

    const std::chrono::duration<double> elapsed =
        std::chrono::steady_clock::now() - start;

    std::osyncstream(std::cout) << "Execution time: " << std::fixed
                               << std::setprecision(2) << elapsed.count()
                               << " s\nWork is done!" << std::endl;
}
