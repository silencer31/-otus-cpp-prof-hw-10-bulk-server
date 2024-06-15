#include "async.h"

#include <iostream>
#include <future>

void test_context_1()
{
    std::size_t bulkSize = 2;

    auto handle_1 = async::connect(bulkSize);

    async::receive(handle_1, "1", 1);
    async::receive(handle_1, "2", 1);
    async::receive(handle_1, "3", 1);
    async::receive(handle_1, "\n", 1);
    async::receive(handle_1, "4", 1);
    async::receive(handle_1, "5", 1);
    async::receive(handle_1, "6", 1);
    async::receive(handle_1, "7", 1);
    async::receive(handle_1, "\n", 1);
    async::receive(handle_1, "8", 1);
    async::receive(handle_1, "9", 1);
    async::receive(handle_1, "10", 2);
    async::receive(handle_1, "11", 2);
    async::receive(handle_1, "\n", 1);
    async::receive(handle_1, "12", 2);
    async::receive(handle_1, "13", 2);
    async::receive(handle_1, "14", 2);

    async::disconnect(handle_1);

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void test_context_2()
{
    std::size_t bulkSize = 1;

    auto handle_2 = async::connect(bulkSize);

    async::receive(handle_2, "14", 2);
    async::receive(handle_2, "15", 2);
    async::receive(handle_2, "16", 2);
    async::receive(handle_2, "\n", 1);
    async::receive(handle_2, "17", 2);
    async::receive(handle_2, "18", 2);
    async::receive(handle_2, "19", 2);
    async::receive(handle_2, "20", 2);
    async::receive(handle_2, "\n", 1);
    async::receive(handle_2, "21", 2);

    async::disconnect(handle_2);

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void test_context_3()
{
    std::size_t bulkSize = 3;

    auto handle_3 = async::connect(bulkSize);

    async::receive(handle_3, "22", 2);
    async::receive(handle_3, "23", 2);
    async::receive(handle_3, "24", 2);
    async::receive(handle_3, "\n", 1);
    async::receive(handle_3, "25", 2);
    async::receive(handle_3, "26", 2);
    async::receive(handle_3, "27", 2);
    async::receive(handle_3, "28", 2);
    async::receive(handle_3, "\n", 1);
    async::receive(handle_3, "29", 2);
    async::receive(handle_3, "30", 2);
    async::receive(handle_3, "31", 2);
    async::receive(handle_3, "32", 2);
    async::receive(handle_3, "\n", 1);
    async::receive(handle_3, "33", 2);

    async::disconnect(handle_3);

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main()
{
    auto res_1 = std::async(std::launch::async, test_context_1);
    auto res_2 = std::async(std::launch::async, test_context_2);
    auto res_3 = std::async(std::launch::async, test_context_3);

    const std::size_t bulk = 5;

    auto handle_4 = async::connect(bulk);
    auto handle_5 = async::connect(bulk);
    
    async::receive(handle_4, "a", 1);
    async::receive(handle_4, "b", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "c", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "d", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_5, "e", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "f", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "g", 1);
    async::receive(handle_5, "\n", 1);
    async::receive(handle_4, "{", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "h", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "i", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "j", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "k", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_5, "}", 1);
    async::receive(handle_4, "\n", 1);
    async::receive(handle_4, "l", 1);
    async::receive(handle_5, "m", 1);
    async::receive(handle_4, "\n", 1);

    async::disconnect(handle_4);
    async::disconnect(handle_5);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    res_1.wait();
    res_2.wait();
    res_3.wait();

    std::cout << "\nEND of Main\n" << std::endl;

    return 0;
}