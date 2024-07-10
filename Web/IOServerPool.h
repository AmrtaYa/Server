#pragma once
#include "BoostDefine.h"
#include "Singleton.h"
#include "thread"
#include "vector"
class IOServerPackage
{
	friend class IOServerPool;
public:
	IOServerPackage();
private:
	std::unique_ptr<std::thread> mThread;
	std::shared_ptr<IO_Context> context;
	std::shared_ptr<IO_Work> work;
	void ProcessFunc(IO_Context& context);
	IO_Context& GetContext();
	std::thread& GetThread();
	std::shared_ptr<IO_Work> GetWork();

};
class IOServerPool : public Singleton<IOServerPool>
{
	friend class Singleton<IOServerPool>;
private:
	IOServerPool();
	
	bool ifInit = false;
	std::vector<std::shared_ptr<IOServerPackage>> ioThreads;
	std::mutex mut;
	short currentThreadNum = 0;
	short maxThreadNum = 0;

	//主线程的
	std::shared_ptr<IO_Context> mainIo;
	std::shared_ptr<IO_Work> mainWork;
public:
	~IOServerPool();
	void InitPool(size_t threadNum = std::thread::hardware_concurrency());
	IO_Context& GetIoContext();
	
	void Run();
	void Stop();
};

