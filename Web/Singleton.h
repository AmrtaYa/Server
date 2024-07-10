#pragma once
#include "mutex"
#include "memory"
template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	Singleton(const Singleton<T>&) = delete;
	Singleton& operator=(const Singleton<T>&) = delete;
	static std::shared_ptr<T> instance;
public:
	static std::shared_ptr<T> GetInstance()
	{
		static std::once_flag flag;
		std::call_once(flag, [&]()
			{
				instance = std::shared_ptr<T>(new T());
			});
		return instance;
	}

};

template<typename T>
std::shared_ptr<T> Singleton<T>::instance = nullptr;