#include "Reflect.h"
#include <mutex>
std::unique_ptr<GJC_Recfelct::RefelctManager> GJC_Recfelct::RefelctManager::instance = nullptr;

GJC_Recfelct::Register::Register(std::string typeName, RegisterFieldFunc func)
{
	if (RefelctManager::GetInstance()->reflectMap.find(typeName) == RefelctManager::GetInstance()->reflectMap.end())
	{
		Type* type = new Type();
		type->typeName = typeName;
		RefelctManager::GetInstance()->reflectMap[typeName] = type;
	}
	Field* f = func();
	RefelctManager::GetInstance()->reflectMap[typeName]->fieldMap[f->fieldName] = f;
}
GJC_Recfelct::Register::Register(std::string typeName, RegisterFunctionFunc funcc)
{
}
static std::once_flag flag;
GJC_Recfelct::RefelctManager* GJC_Recfelct::RefelctManager::GetInstance()
{
	std::call_once(flag, []() {
			if (instance == nullptr)
			{
				instance = std::make_unique<GJC_Recfelct::RefelctManager>();
				instance->Init();
			}
		});
	return instance.get();
}

GJC_Recfelct::Type GJC_Recfelct::RefelctManager::GetType(std::string typeName)
{
	if (reflectMap.find(typeName) == reflectMap.end())
		throw "类型不存在";

	return *reflectMap[typeName];
}

void GJC_Recfelct::RefelctManager::Init()
{
	if (IfInit)return;
	IfInit = true;


}

GJC_Recfelct::RefelctManager::RefelctManager() : IfInit(false)
{
}

std::size_t GJC_Recfelct::Type::GetFieldCounts()
{
	return std::size_t();
}

std::size_t GJC_Recfelct::Type::GetFieldOffset(std::string fieldName)
{
	auto  iter = fieldMap.find(fieldName);
	if (iter == fieldMap.end())
		throw "未找到字段" + fieldName;
	return iter->second->GetOffset();
}

std::vector<GJC_Recfelct::Field> GJC_Recfelct::Type::GetFields()
{
	std::vector<GJC_Recfelct::Field> fs;
	for (auto iter =fieldMap.begin(); iter != fieldMap.end(); ++iter)
	{
		fs.push_back(*iter->second);
	}
	return fs;
}

GJC_Recfelct::Method* GJC_Recfelct::Type::GetMethod(std::string methodName)
{
	auto iter = methodMap.find(methodName);
	if (iter == methodMap.end()) throw "未找到函数反射:" + methodName;
	return iter->second;
}

std::size_t GJC_Recfelct::Type::Size()
{
	return typeSize;
}

std::size_t GJC_Recfelct::Field::GetOffset()
{
	return offset;
}

void GJC_Recfelct::Field::SetOffset(size_t offset)
{
	this->offset = offset;
}

void GJC_Recfelct::Field::SetType(std::string str)
{
	this->type = str;
}

std::string GJC_Recfelct::Field::GetType()
{
	return type;
}

void GJC_Recfelct::Field::SetFieldName(std::string str)
{
	this->fieldName = str;
}

void GJC_Recfelct::Field::SetInstanceFunc(CreateFieldInstanceFunc func)
{
	this->instanceFunc = func;
}

std::string GJC_Recfelct::Field::GetFieldName()
{
	return this->fieldName;
}

void* GJC_Recfelct::Field::CreateInstance()
{
	auto address = instanceFunc();
	return address;
}

std::size_t GJC_Recfelct::Field::len()
{
	return typeLen;
}

std::string GJC_Recfelct::GetName(std::string str)
{
	auto spacePos = str.find_first_of(' ');
	std::string result = str.substr(spacePos+1);
	return result;
}

GJC_Recfelct::RegisterClass::RegisterClass(std::string typeName, CreateInstanceFunc createInstanceFunc, std::size_t typeSize)
{
	if (RefelctManager::GetInstance()->reflectMap.find(typeName) == RefelctManager::GetInstance()->reflectMap.end())
	{
		Type* type = new Type();
		type->typeName = typeName;
		type->typeSize = typeSize;
		RefelctManager::GetInstance()->reflectMap[typeName] = type;
	}
	RefelctManager::GetInstance()->reflectMap[typeName]->createInstanceFunc = std::move(createInstanceFunc);

}
