#include "FileHelper.h"
#include "filesystem"
#include "iostream"
#include <fstream>
#include "LogManager.h"
std::string FileHelper::obPath = std::filesystem::current_path().string() + "/HtmlPage";

FileHelper::FileHelper()
{

}
std::string FileHelper::ReadText(std::string path)
{
	std::string htmlText;
	std::string index;
	path = obPath + path;
	std::ifstream file;
	file.open(path, std::ios::in);
	if (!file.is_open())
	{
		LogManager::Log(path + "  读取文件失败");
		return std::string();//到时候返回错误代码页面才对
	}
	while (std::getline(file, index))
	{
		htmlText += index;
	}
	file.close();
	return htmlText;
}

std::string FileHelper::ReadBytes(std::string path)
{
	path = obPath + path;
	std::ifstream img(path,  std::ios::binary);
	if (!img.is_open())
	{
		LogManager::Log(path + "  读取文件失败");
		return 0;//到时候返回错误代码页面才对
	}
	auto index = std::string(std::istreambuf_iterator<char>(img),std::istreambuf_iterator<char>());
	img.close();
	return index;

}
