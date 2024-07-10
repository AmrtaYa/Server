using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace GJC.Helper
{
    /*
   Json解析器
  {
     "ID":"1000"
  }
  对象 -->string
  string-->对象
   */
    class JsonHelper
    {
        public static string ObjectToString(object obj)
        {
            StringBuilder sb = new StringBuilder();
            PropertyInfo[] propInfo = obj.GetType().GetProperties();
            sb.Append('{');
            foreach (var item in propInfo)
            {
                sb.Append("\"" + item.Name + "\"" + ":" + "\"" + item.GetValue(obj).ToString() + "\"" + ',');
            }
            sb.Remove(sb.Length-1,1);
            sb.Append('}');
            return sb.ToString();
        }
     public   static T StringToObject<T>(string json) where T : new()//约束条件
        {
            //typeof(T)可以这样获取
            T instance = new T();
            Type type = instance.GetType();
            string[] keyValue = json.Replace("{","").Replace("}", string.Empty)
                .Replace("\"","").Split(',',':');
            for (int i = 0; i < keyValue.Length; i+=2)
            {
                PropertyInfo propInfo = type.GetProperty(keyValue[i]);//属性 比如ID Login
                object value = Convert.ChangeType(keyValue[i + 1], propInfo.PropertyType);////keyValue[i + 1]属性里的值
                propInfo.SetValue(instance,value);
            }
            //   var instance = Activator.CreateInstance();
            return instance;
        }
    }
}

