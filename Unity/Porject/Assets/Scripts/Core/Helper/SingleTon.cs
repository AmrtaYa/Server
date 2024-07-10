using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GJC.Helper
{
    /// <summary>
    /// 脚本单例类   用法：唯一，常用
    /// 适用性：场景中存在唯一的对象，即可让该对象继承这个类   只限于unity中的工具类
    /// 如何适用：必须传递自己的类型
    ///                任意脚本生命周期中，通过子类instance访问
    /// </summary>
    public class   SingleTon<T> : MonoBehaviour where T: SingleTon<T>
    {
        private static T instance;
        public static T Instance 
        {
            get
            {
                if (instance == null)
                {
                    instance = FindObjectOfType<T>();
                    if (instance == null)
                    {
                        new GameObject(typeof(T).ToString()).AddComponent<T>();
                        instance.init();
                    }
                    else
                    {
                        instance.init();
                    }
        
                }
               
                return instance;
            }
        }
        private void Awake()
        {
            if (instance == null)
            {
                instance = this as T;
                init();
            }
        }
        public virtual void init()
        { 
        
        }
    }
}
