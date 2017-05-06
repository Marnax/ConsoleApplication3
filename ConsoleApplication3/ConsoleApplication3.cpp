#include <stdafx.h>
#include "SimpleThreadpool.h"
#include <iostream>

int main()
{
	{
		SimpleThreadPoolOrganizer pool(5);
		std::vector<std::future<int>> vec;
		for (int i = 0; i < 500; ++i)
		{
			std::cout << "Füge Task hinzu " << i << std::endl;
			vec.push_back(pool.addTaskExclusively(std::function<int(void)>([i]() {
				Sleep(1000);
				return int(i);
			})));
		}
		std::cout << "Alle Tasks hinzugefügt " << std::endl;
		for (int i = 0; i < vec.size(); ++i)
		{
			long myVal = vec[i].get();
			std::cout << "Got Future " << myVal << std::endl;
		}
	}
}