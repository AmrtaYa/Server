using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
namespace GJC.Helper
{
    /*
     使用方式：
    1.Get(设置对象池名称，生成的预制件，位置，旋转) 如果需要在获取的时候添加其他行为，请实现此命名空间中的Ireset接口
    2、Release() 放回对象池，自动setactive为false，如果想在释放时添加其他的行为，请实现此命名空间中的Irelease接口
    3、PoolDestroy() 销毁  有多个重载，可以根据单个物体，单个对象池名称的全部，又或者是全部
     */

    /// <summary>
    /// 游戏对象池  unity自带，但是这个是自己写的
    /// 需要附带GJC.Helper中的SingleTon单例脚本
    /// </summary>
    public class GameObjectPool : SingleTon<GameObjectPool>
    {
        public Dictionary<string, List<GameObject>> pool;
        public override void init()
        {
            base.init();
            pool = new Dictionary<string, List<GameObject>>();
        }
        public GameObject Get(string poolDir, GameObject go, Vector3 pos, Quaternion rot)
        {
            GameObject index = CreateGameObject(poolDir, go);
            index.SetActive(true);
            index.transform.position = pos;
            index.transform.rotation = rot;
            var inter = index.GetComponents<IRsetable>();
            if (inter != null)
                foreach (var item in inter)
                {
                    item.OnRest();
                }
            return index;
        }
        private GameObject CreateGameObject(string poolDir, GameObject go)
        {
            if (!pool.ContainsKey(poolDir))
            {
                pool.Add(poolDir, new List<GameObject>());
            }
            GameObject index = pool[poolDir].Find(G =>
                {
                    if (G == null)
                    {
                        pool[poolDir].Remove(G);
                        return false;
                    }

                    return !G.activeInHierarchy;
                }

               );
            if (index == null)
            {
                index = Instantiate<GameObject>(go);
                pool[poolDir].Add(index);
            }

            return index;
        }
        private IEnumerator ReleaseIE(GameObject go, float delay = 0)
        {
            yield return new WaitForSeconds(delay);
            if (go != null)
            {
                var inter = go.GetComponents<IRelease>();
                if (inter != null)
                    foreach (var item in inter)
                    {
                        item.OnRelease();
                    }
                go.SetActive(false);
            }


        }
        public void Release(GameObject go, float delay = 0)
        {
            StartCoroutine(ReleaseIE(go, delay));
        }


        private IEnumerator DestroyIE(GameObject go, float delay = 0)
        {
            yield return new WaitForSeconds(delay);
            foreach (var list in pool.Values)
            {
               var needGo =  list.Where((g) => g == go).ToList();
               if (needGo.Count==1)
               {
                   list.Remove(needGo[0]);
                   Destroy(needGo[0]);
               }
            }
               
        
          
          
        }
        /// <summary>
        /// 销毁该key中的所有元素
        /// </summary>
        /// <param name="delay"></param>
        /// <returns></returns>
        private IEnumerator DestroyIE(string key, float delay = 0)
        {
            yield return new WaitForSeconds(delay);
            foreach (var item in pool[key])
            {
                GameObject.Destroy(item);
            }
            pool.Remove(key);
        }
        /// <summary>
        /// 销毁所有元素
        /// </summary>
        /// <param name="delay"></param>
        /// <returns></returns>
        private IEnumerator DestroyIE(float delay = 0)
        {
            yield return new WaitForSeconds(delay);
            List<string> index = new List<string>(pool.Keys);
            foreach (var item in index)
            {
                StartCoroutine(DestroyIE(item));
            }

        }


        /// <summary>
        /// 销毁字典中特定的物体
        /// </summary>
        /// <param name="go"></param>
        /// <param name="delay"></param>
        public void PoolDestroy(GameObject go, float delay = 0)
        {
            StartCoroutine(DestroyIE(go, delay));
        }
        /// <summary>
        /// 销毁一个key里的所有元素
        /// </summary>
        /// <param name="key"></param>
        /// <param name="delay"></param>
        public void PoolDestroy(string key, float delay = 0)
        {
            StartCoroutine(DestroyIE(key, delay));
        }
        /// <summary>
        /// 销毁全部
        /// </summary>
        /// <param name="delay"></param>
        public void PoolDestroy(float delay = 0)
        {
            StartCoroutine(DestroyIE(delay));
        }
    }
    public interface IRelease
    { void OnRelease(); }
    public interface IRsetable
    { void OnRest(); }

}

