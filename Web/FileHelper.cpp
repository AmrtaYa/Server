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
		LogManager::Log(path + "  ��ȡ�ļ�ʧ��");
		return std::string();//��ʱ�򷵻ش������ҳ��Ŷ�
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
		LogManager::Log(path + "  ��ȡ�ļ�ʧ��");
		return 0;//��ʱ�򷵻ش������ҳ��Ŷ�
	}
	auto index = std::string(std::istreambuf_iterator<char>(img),std::istreambuf_iterator<char>());
	img.close();
	return index;

}
