using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
namespace GJC.Helper
{
    /// <summary>
    /// 数组工具类
    /// </summary>
    public static class ArrayHelper
    {
        /// <summary>
        /// 查找数组中满足条件的元素
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="Array">数组</param>
        /// <param name="condition">满足条件</param>
        /// <returns></returns>
        public static T[] FindObjects<T>(this T[] Array,Func<T,bool> condition)
        {
            List<T> indexArray = new List<T>();
            foreach (var item in Array)
            {
                if (condition(item)==true)
                {
                    indexArray.Add(item);
                }
            }
            return indexArray.ToArray();
        }
        /// <summary>
        /// 查找符合条件的元素，但是只找一个
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="Array"></param>
        /// <param name="condition"></param>
        /// <returns></returns>
        public static T FindObject<T>(this T[] Array, Func<T, bool> condition)
        {
            foreach (var item in Array)
            {
                if (condition(item) == true)
                {
                    return item;
                }
            }
            return default(T);
        }
        /// <summary>
        /// 获得数组中 某属性最大的
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="Q"></typeparam>
        /// <param name="Array"></param>
        /// <param name=""></param>
        /// <param name="condition"></param>
        /// <returns></returns>
        public static T GetMax<T,Q>(this T[] Array,Func<T,Q> condition)where Q:IComparable
        {
            T index = Array[0];
            for (int i = 1; i < Array.Length; i++)
            {
                if (condition(index).CompareTo(condition(Array[i]))<0)
                {
                    index = Array[i];
                
                }
            }
            return index;
        }
        /// <summary>
        /// 获得数组中 某属性最小的
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="Q"></typeparam>
        /// <param name="Array"></param>
        /// <param name=""></param>
        /// <param name="condition"></param>
        /// <returns></returns>
        public static T GetMin<T, Q>(this T[] Array, Func<T, Q> condition) where Q : IComparable
        {
            T index = Array[0];
            for (int i = 1; i < Array.Length; i++)
            {
                if (condition(index).CompareTo(condition(Array[i])) > 0)
                {
                    index = Array[i];

                }
            }
            return index;
        }
        
        /// <summary>
        /// 排序
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="Q"></typeparam>
        /// <param name="Array"></param>
        /// <param name="sort">如何排序 up升序 down 降序</param>
        /// <param name="condition"></param>
        public static void Order<T,Q>(this T[] Array,Sort sort,Func<T,Q> condition)where Q:IComparable
        {
            switch (sort)
            {
                case Sort.down://降序
                    for (int j=0; j<Array.Length-1; j++)
                    {
                        for (int i = 0; i < Array.Length - 1; i++)
                        {
                            if (condition(Array[i]) .CompareTo(condition(Array[i + 1]))<0)
                            {
                                var index = Array[i];
                                Array[i] = Array[i + 1];
                                Array[i + 1] = index;
                            
                            }


                        }
                    }
                    break;
                case Sort.up://升序
                    for (int j = 0; j < Array.Length - 1; j++)
                    {
                        for (int i = 0; i < Array.Length - 1; i++)
                        {
                            if (condition(Array[i]).CompareTo(condition(Array[i + 1])) > 0)
                            {
                                var index = Array[i];
                                Array[i] = Array[i + 1];
                                Array[i + 1] = index;

                            }


                        }
                    }
                    break;
            }
        }
        /// <summary>
        /// 筛选类型  比如原本全go类型，condition写tr，就全能变成tr类型
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="Q"></typeparam>
        /// <param name="Array"></param>
        /// <param name="condition"></param>
        /// <returns></returns>
        public static Q[] Select<T, Q>(this T[] Array, Func<T, Q> condition)
        {
            List<Q> arr = new List<Q>();
            foreach (var item in Array)
            {
                arr.Add(condition(item));
            }
            return arr.ToArray();
        }
    }
  public  enum Sort
    { 
     down = 0,
     up = 1
    
    }
}
