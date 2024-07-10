#pragma once
#include "string"
/// <summary>
/// 文件读取辅助
/// </summary>
static class FileHelper
{
public:
	FileHelper();
	static std::string ReadText(std::string path);
	static std::string ReadBytes(std::string path);
private:
	static std::string obPath;//相对路径
};

