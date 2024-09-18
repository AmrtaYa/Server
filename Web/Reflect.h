#pragma once
#include "map"
#include "string"
#include "memory"
#include "vector"
#include "functional"
#include "mutex"
#include "iostream"
/// <summary>
/// 2024.8.7  实现字段获取   未实现函数反射 
/// </summary>
namespace GJC_Recfelct
{
	class Field;
	class Method;
	class Type;
	typedef std::function<void* ()> CreateInstanceFunc;
	typedef  std::function<Field* ()> RegisterFieldFunc;
	typedef  std::function<Method* ()> RegisterFunctionFunc;

	class Activator
	{
	public:
		template<typename T>
		static T* CreateInstance(Type type);

	};
	class Register
	{
	public:
		Register(std::string typeName, RegisterFieldFunc func);
		Register(std::string typeName, RegisterFunctionFunc funcc);
	};
	class RegisterClass
	{
	public:
		RegisterClass(std::string typeName, CreateInstanceFunc createInstanceFunc, std::size_t typeSize);
	};
	/// <summary>
	/// 存储反射函数
	/// </summary>
	class Method
	{
	public:
		std::string GetName();
		uintptr_t GetMethod();
	private:
		std::string name;
		uintptr_t method;

	};
	class Field
	{
	public:
		friend class Register;
		typedef std::function<void* ()> CreateFieldInstanceFunc;
		std::size_t GetOffset();
		void SetOffset(size_t offset);
		void SetType(std::string str);
		void SetTypeLen(std::size_t size);
		std::string GetType();
		void SetFieldName(std::string str);
		void SetInstanceFunc(CreateFieldInstanceFunc func);
		std::string GetFieldName();
		void* CreateInstance();
		std::size_t len();//类型长度没设置，初始化实例没注册
	private:
		CreateFieldInstanceFunc instanceFunc;
		std::size_t offset;
		std::string type;
		std::string fieldName;
		std::size_t typeLen;
	};
	class  Type
	{
	public:
		friend class Register;
		friend class RegisterClass;
		friend class Activator;
		template<typename T>
		T GetFieldValue(char* obj, std::string fieldName);
		template<typename T>
		void SetFieldValue(char* obj, std::string fieldName, T value);

		std::size_t GetFieldCounts();

		std::size_t GetFieldOffset(std::string fieldName);
		std::vector<Field> GetFields();

		Method* GetMethod(std::string methodName);
		/*template<typename RuturnType,typename... Types>
		RuturnType CallMethod(char* obj,std::string methodName,Types... args);*/
		std::size_t Size();
	private:
		CreateInstanceFunc createInstanceFunc;
		std::size_t typeSize;
		std::string typeName;
		std::map<std::string, Field*> fieldMap;
		std::map<std::string, Method*> methodMap;
	};
	/// <summary>
	/// 反射类捏
	/// </summary>
	class RefelctManager
	{
	public:
		friend class Type;
		friend class Register;
		friend class RegisterClass;
		RefelctManager();
		//这边懒得写内存销毁了，因为 反射的表应该是从头到尾不会变过了，只有进程关闭时才会
		static RefelctManager* GetInstance();
		Type GetType(std::string typeName);
		void Init();
	private:
		static std::unique_ptr<RefelctManager> instance;

		std::map<std::string, Type*> reflectMap;
		bool IfInit;
	};
	//为了去除前面的class和struct
	std::string GetName(std::string str);
	template<typename T>
	inline T Type::GetFieldValue(char* obj, std::string fieldName)
	{
		std::size_t offset = fieldMap[fieldName]->GetOffset();
		T value = *(T*)(obj + offset);
		return value;
	}
	template<typename T>
	inline void Type::SetFieldValue(char* obj, std::string fieldName, T value)
	{
		std::size_t offset = fieldMap[fieldName]->GetOffset();
		void* dst = obj + offset;
		memcpy((void*)dst, &value, sizeof(T));
	}


	template<typename T>
	inline T* Activator::CreateInstance(Type type)
	{
		auto ad = type.createInstanceFunc();
		return (T*)ad;
	}

	/*template<typename RuturnType,typename ...Types>
	inline RuturnType Type::CallMethod(char* obj, std::string methodName, Types  ... args)
	{
		auto method = GetMethod(methodName);
		auto mAddress = method->GetMethod();
		typedef std::function<RuturnType(args...)> methodDefine;
		return *((methodDefine*)mAddress)(args...);
	}*/
}
#define REGISTER_CLASS_NS(Namespace,Type)\
void* Type##RegisterCreateInstance()\
	{\
		Namespace::##Type* temp = new Namespace::##Type();\
		return (void*)temp;\
	}\
GJC_Recfelct::RegisterClass Type##RegInstance(GJC_Recfelct::GetName( std::string(typeid(Namespace::##Type).name())), Type##RegisterCreateInstance,sizeof(Namespace::##Type));

#define REGISTER_CLASS(Type)\
void* Type##RegisterCreateInstance()\
	{\
		Type* temp = new Type();\
		return (void*)temp;\
	}\
GJC_Recfelct::RegisterClass Type##RegInstance(GJC_Recfelct::GetName( std::string(typeid(Type).name())), Type##RegisterCreateInstance,,sizeof(Type));

#define REGISTER_FIELD_NS(Namespace,Type,FieldName) \
void* Type##FieldName##FieldRegisterCreateInstance()\
	{\
		Namespace::##Type* temp = new Namespace::##Type();\
		return (void*)temp;\
	}\
	GJC_Recfelct::Field* Type##Register##FieldName()			\
	{														\
		Namespace::##Type temp;								    \
		GJC_Recfelct::Field* f = new GJC_Recfelct::Field();					\
		f->SetType(typeid(temp.##FieldName).name()); \
		f->SetFieldName( #FieldName);		\
		f->SetTypeLen(sizeof(temp.##FieldName));		\
		f->SetOffset( (std::size_t)(&temp.##FieldName) - (std::size_t)(&temp)); \
		f->SetInstanceFunc(Type##FieldName##FieldRegisterCreateInstance);\
		return f;						\
	}  \
	GJC_Recfelct::Register Type##Reg##FieldName(GJC_Recfelct::GetName( std::string(typeid(Namespace::##Type).name())), Type##Register##FieldName);
#define REGISTER_FIELD(Type,FieldName) \
void* Type##FieldName##FieldRegisterCreateInstance()\
	{\
		Type* temp = new Type();\
		return (void*)temp;\
	}\
	GJC_Recfelct::Field* Type##Register##FieldName()			\
	{														\
		Type temp;								    \
		GJC_Recfelct::Field* f = new GJC_Recfelct::Field();					\
		f->SetType(typeid(temp.##FieldName).name()); \
		f->SetTypeLen( sizeof(temp.##FieldName));		\
		f->SetFieldName( #FieldName);		\
		f->SetOffset( (std::size_t)(&temp.##FieldName) - (std::size_t)(&temp)); \
	f->SetInstanceFunc(Type##FieldName##FieldRegisterCreateInstance);\
		return f;						\
	}  \
	GJC_Recfelct::Register Type##Reg##FieldName(GJC_Recfelct::GetName( std::string(typeid(Type).name())), Type##Register##FieldName);




#define REFLECT_GET_TYPE(type) \
GJC_Recfelct::RefelctManager::GetInstance()->GetType(std::string(type))

#define REFLECT_GET_VALUE(obj,field,fieldType)  \
GJC_Recfelct::RefelctManager::GetInstance()->\
GetType(GJC_Recfelct::GetName(typeid(obj).name())).\
GetFieldValue<fieldType>((char*)&obj,std::string(field));

#define REFLECT_SET_VALUE(obj,field,value)  \
GJC_Recfelct::RefelctManager::GetInstance()->\
GetType(GJC_Recfelct::GetName(typeid(obj).name())).\
SetFieldValue((char*)&obj,std::string(field),value);


#define REFLECT_ALLFIELDS(TYPE)  \
GJC_Recfelct::RefelctManager::GetInstance()->GetType(TYPE).GetFields();

#define CREATE_INSTANCE(mType,Type)\
	GJC_Recfelct::Activator::CreateInstance<Type>( REFLECT_GET_TYPE(mType));
//#define REFLECT_GET_METHOD(obj,methodName,returnValue,...) \
//GJC_Recfelct::RefelctManager::GetInstance()->\
//GetType(GJC_Recfelct::GetName(typeid(obj).name())).\
//CallMethod<returnValue,std::function<int()>>(&obj,std::string(methodName)##__VA_ARGS__);


