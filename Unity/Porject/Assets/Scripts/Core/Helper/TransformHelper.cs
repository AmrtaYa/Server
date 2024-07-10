using System.Collections.Generic;
using UnityEngine;

namespace GJC.Helper
{
    /// <summary>
    /// 
    /// </summary>
    public static class TransformHelper
    {
        /// <summary>
        /// 根据名字查找自身子物体的Transform
        /// </summary>
        /// <param name="parentTF"></param>
        /// <param name="name"></param>
        /// <returns></returns>
        public static Transform FindTheTfByName(this Transform parentTF, string name)
        {
            Transform childTF;
            childTF = parentTF.Find(name);
            if (childTF != null)
            {
                return childTF;
            }
            else
            {
                for (int i = 0; i < parentTF.childCount; i++)
                {
                    childTF = FindTheTfByName(parentTF.GetChild(i), name);
                    if (childTF != null)
                    {
                        return childTF;
                    }
                }

                return null;
            }
        }

        /// <summary>
        /// 根据名字查找自身子物体的RectTransform   参数一定要get一下RectTransofrm，用transform无法获取到
        /// </summary>
        /// <param name="parentTF"></param>
        /// <param name="name"></param>
        /// <returns></returns>
        public static RectTransform FindTheTfByName(this RectTransform parentTF, string name)
        {
            RectTransform childTF;
            childTF = parentTF.Find(name) as RectTransform;
            if (childTF != null)
            {
                return childTF;
            }
            else
            {
                for (int i = 0; i < parentTF.childCount; i++)
                {
                    childTF = FindTheTfByName(parentTF.GetChild(i) as RectTransform, name);
                    if (childTF != null)
                    {
                        return childTF;
                    }
                }

                return null;
            }
        }

        /// <summary>
        /// 根据名字查找自身子物体的任何一个组件 适用于Transform       参数一定要get一下RectTransofrm，用transform无法获取到
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="parentTF"></param>
        /// <param name="name"></param>
        /// <returns></returns>
        public static T FindTheTfByName<T>(this Transform parentTF, string name)
        {
            var childTr = parentTF.Find(name);
            T child = default(T);
            if (childTr != null)
                child = childTr.GetComponent<T>();
            if (child != null) return child;
            for (int i = 0; i < parentTF.childCount; i++)
            {
                child = FindTheTfByName<T>(parentTF.GetChild(i), name);
                if (child != null) return child;
            }

            return default(T);
        }

        /// <summary>
        /// 根据名字查找自身子物体的任何一个组件  适用于RectTransoform
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="parentTF"></param>
        /// <param name="name"></param>
        /// <returns></returns>
        public static T FindTheTfByName<T>(this RectTransform parentTF, string name)
        {
            var childTr = parentTF.Find(name) as RectTransform;
            T child = default(T);
            if (childTr != null)
                child = childTr.GetComponent<T>();
            if (child != null) return child;
            for (int i = 0; i < parentTF.childCount; i++)
            {
                child = FindTheTfByName<T>(parentTF.GetChild(i) as RectTransform, name);
                if (child != null) return child;
            }

            return default(T);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="parentTF"></param>
        /// <param name="container"></param>
        /// <param name="index">0查找自身  1不查找自身</param>
        /// <typeparam name="T"></typeparam>
        public static void FindAllComponent<T>(this Transform parentTF, ref List<T> container, int index = 0)
        {
            int childCount = parentTF.childCount;
            if (childCount <= 0)
            {
                return;
            }

            if (index == 0)
            {
                T com = parentTF.GetComponent<T>();
                if (com != null) container.Add(com);
            }

            for (int i = 0; i < childCount; i++)
            {
                T com = parentTF.GetChild(i).GetComponent<T>();
                if (com != null) container.Add(com);
                FindAllComponent<T>(parentTF.GetChild(i), ref container, (index + 1));
            }
        }
    }
}