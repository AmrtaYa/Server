#pragma once
#include "string"
/// <summary>
/// �ļ���ȡ����
/// </summary>
static class FileHelper
{
public:
	FileHelper();
	static std::string ReadText(std::string path);
	static std::string ReadBytes(std::string path);
private:
	static std::string obPath;//���·��
};

