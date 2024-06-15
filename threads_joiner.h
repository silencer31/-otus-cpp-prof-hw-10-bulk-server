#pragma once

#include <vector>
#include <thread>

class ThreadsJoiner
{
	std::vector<std::thread>& threads;

public:
	explicit ThreadsJoiner(std::vector<std::thread>& threads_)
		: threads(threads_)
	{}

	~ThreadsJoiner()
	{
		for (unsigned int i = 0; i < threads.size(); ++i) {
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
	}
};