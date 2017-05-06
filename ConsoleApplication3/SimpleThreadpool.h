#pragma once

#include "boost/asio.hpp"
#include "boost/thread/thread.hpp"
#include <thread>
#include <future>

class SimpleThreadPoolOrganizer
{
public:
	SimpleThreadPoolOrganizer(long noThreads = 1) : work(service)
	{
		for (int i = 0; i < noThreads; ++i)
		{
			threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &service));
		}
	};
	virtual ~SimpleThreadPoolOrganizer()
	{
		service.stop();
		threadpool.join_all();
	};

	template<class R>
	std::future<R> addTaskExclusively(std::function<R(void)> && f)
	{
		std::lock_guard<std::mutex> guard(internalMutex);
		return addTask(std::move(f));
	}

	template<class R>
	std::future<R> addTask(std::function<R(void)>&& f)
	{
		auto task = std::make_shared<std::packaged_task<R(void)>>(std::move(f));
		auto fut = task->get_future();		
		service.post([task]() { (*task)(); });
		return fut;
	}

protected:
	boost::asio::io_service service;
	boost::asio::io_service::work work;
	boost::thread_group threadpool;
	std::mutex internalMutex;
};
