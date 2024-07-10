#include "IOServerPool.h"
#include "LogManager.h"
IOServerPool::IOServerPool() : maxThreadNum(0), currentThreadNum(0), ifInit(false),
mainIo(std::make_shared<IO_Context>()),
mainWork(std::make_shared<IO_Work>(*mainIo.get()))
{
}
IOServerPool::~IOServerPool()
{
	for (size_t i = 0; i < ioThreads.size(); i++)
	{
		ioThreads[i]->GetContext().stop();
		ioThreads[i]->GetThread().join();
	}
	ioThreads.clear();
}
IOServerPackage::IOServerPackage() :
	context(std::make_shared<IO_Context>()),
	work(std::make_shared<IO_Work>(GetContext())),
	mThread(std::make_unique<std::thread>([this]() 
		{
			ProcessFunc(GetContext());
		}))
{
}
void IOServerPackage::ProcessFunc(IO_Context& context)
{
	context.run();
}
void IOServerPool::InitPool(size_t threadNum)
{
	if (ifInit)
	{
		LogManager::Log("IOServerPool已加载，无需再次加载");
		return;
	}
	maxThreadNum = threadNum;
	//根据设定线程进行初始化，如果没有，那就是最大线程数
	for (size_t i = 0; i < threadNum; ++i)
	{
		ioThreads.emplace_back(std::make_shared<IOServerPackage>());
	}
	ifInit = true;
	LogManager::Log("并发服务启动成功");
}

IO_Context& IOServerPool::GetIoContext()
{
	std::lock_guard<std::mutex> lock(mut);
	IO_Context& index =
		ioThreads[currentThreadNum]->GetContext();
	currentThreadNum++;
	if (currentThreadNum >= maxThreadNum) currentThreadNum = 0;
	return index;
}

void IOServerPool::Run()
{
	mainIo->run();
}

void IOServerPool::Stop()
{
	mainIo->stop();
}



IO_Context& IOServerPackage::GetContext()
{
	return *context.get();
}

std::thread& IOServerPackage::GetThread()
{
	return *mThread;
}

std::shared_ptr<IO_Work> IOServerPackage::GetWork()
{
	return work;
}
