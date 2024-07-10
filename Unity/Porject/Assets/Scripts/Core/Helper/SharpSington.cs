using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Web;

namespace GJC.Helper
{
    /// <summary>
    /// C#单例(与unity稍有差异)
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class SharpSington<T> where T: ICanInit,new()
    {
        private static T instance;
        private static object LOCK_OBJ = new object();
        public static T Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (LOCK_OBJ)
                    {
                        if (instance == null)
                        {
                            instance = new T();
                            instance.Init();
                        }
                    }
                }
                return instance;
            }
        }

    }

    public interface ICanInit
    {
         void Init();
    }
}