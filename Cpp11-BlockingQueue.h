#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <iostream>

template <typename T>
class BlockingQueue {
public:
	BlockingQueue() {}
	BlockingQueue(const BlockingQueue<T>&) = delete;
	BlockingQueue<T>& operator=(const BlockingQueue<T>&) = delete;
	T deQ();
	void enQ(const T& t);
	T& front();
	void clear();
	size_t size();

private:
	std::queue<T> q_;
	std::mutex mtx_;
	std::condition_variable cv_;

};

//remove element from front of queue

template<typename T>
T BlockingQueue<T>::deQ()
{
	std::unique_lock<std::mutex> l(mtx_);
	if (q_.size() > 0) {
		T temp = q_.front();
		q_.pop();
		return temp;
	}
	while (q_.size() == 0) cv_.wait(l, [this]() {return q_.size() > 0; });
	T temp = q_.front();
	q_.pop();
	return temp;
}

//push element onto back of queue

template<typename T>
void BlockingQueue<T>::enQ(const T& t)
{
	{
		std::unique_lock<std::mutex> l(mtx_);
		q_.push(t);
	}
	cv_.notify_one();
}

//peek at next item to be popped

template<typename T>
T& BlockingQueue<T>::front()
{
	std::lock_guard<std::mutex> l(mtx_);
	if (q_.size() > 0) return q_.front();
	throw std::exception("attempt to deQueue empty queue");
}

//remove all elements from queue

template<typename T>
void BlockingQueue<T>::clear()
{
	std::lock_guard<std::mutex> l(mtx_);
	while (q_.size() > 0) q_.pop();
}

// return number of elements in queue

template<typename T>
size_t BlockingQueue<T>::size()
{
	std::lock_guard<std::mutex> l(mtx_);
	return q_.size();
}