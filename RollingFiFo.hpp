
#pragma once
#include <stack>
#include <array>
#include <vector>
#include <utility>

// ring buffer in c++14
// scaels up when full using resource pool
// notice that it never scales down
// todo: maybe to cache the pointer of *working_pool.rbegin() and of *working_pool.begin()

template <typename T, size_t resourse_size>
class RollingFiFo
{
public:
	using resourse_type = std::array<T, resourse_size>;

	// pushes to the HEAD of the stack
	void push_back(T value)
	{
		manage_resouse_push();
		(*working_pool.rbegin())[tail++] = std::move(value);
	}
	// pops from the TAIL of the stack
	T pop_front()
	{
		const auto& ret_val = (*working_pool.begin())[head++];
		manage_resourse_pop();
		return std::move(ret_val);
	}
private:
	std::stack<resourse_type> resourse_pool;
	std::vector<resourse_type> working_pool;
	size_t head = 0;
	size_t tail = resourse_size;

	void manage_resouse_push()
	{
		if (tail == resourse_size) {
			if (resourse_pool.empty()) {
				working_pool.push_back(std::move(resourse_type()));
			}
			else {
				working_pool.push_back(std::move(resourse_pool.top()));
				resourse_pool.pop();
			}
			tail = 0;
		}
	}

	void manage_resourse_pop()
	{
		if (head == resourse_size) {
			resourse_pool.push(std::move(working_pool[0]));
			working_pool.erase(working_pool.begin());
			head = 0;
		}
	}
};
