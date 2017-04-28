#include <stdafx.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int main()
{
	std::mutex m;
	std::condition_variable v;
	std::thread t1([&m]() {
		while (true)     
		{
			std::unique_lock<std::mutex> lock(m);
			std::cout << "T1 got the lock! " << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(3));
			std::cout << "T1 will lose the lock!" << std::endl;
		}
	});
	std::thread t2([&m]() {
		while (true)
		{
			std::unique_lock<std::mutex> lock(m);
			std::cout << "T2 got the lock! " << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			std::cout << "T2 will lose the lock!" << std::endl;
		}
	});

	t1.join();
	t2.join();
	return 0;
}