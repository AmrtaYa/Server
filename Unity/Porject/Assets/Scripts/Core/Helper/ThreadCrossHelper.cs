using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
namespace GJC.Helper
{
    /// <summary>
    /// 线程交叉访问助手类，使用注意事项：需要继承单例方法，并且需在类中调用init方法后方可使用
    /// </summary>
    public class ThreadCrossHelper : SingleTon<ThreadCrossHelper>
    {
        class Item
        {
            public DateTime time;
            public Action action;
        }
        private List<Item> items;
        public override void init()
        {
            base.init();
            items = new List<Item>();
        }
        private void Update()
        {
            lock (items)
            {
                for (int i = items.Count - 1; i >= 0; i--)
                {
                    if (DateTime.Now >= items[i].time)
                    {
                        items[i].action();
                        items.RemoveAt(i);
                    }
                }
            }
        }
        /// <summary>
        /// 将线程中的东西添加到主线程中（线程无法调用api就用这个吧）
        /// </summary>
        /// <param name="action">方法</param>
        /// <param name="delay">延迟调用这个方法的时间</param>
        public void AddMainThread(Action action, float delay = 0)
        {
            lock (items)
            {
                items.Add(new Item
                {
                    action = action,
                    time = DateTime.Now.AddSeconds(delay)
                });
            }
        }
    }
}
