using UnityEngine;

namespace GJC.Helper
{
    /// <summary>
    /// 脚本单例类   根据类型名生成对应预制件
    /// 适用性：场景中存在唯一的对象，即可让该对象继承这个类   只限于unity中的工具类
    /// 如何适用：必须传递自己的类型
    ///                任意脚本生命周期中，通过子类instance访问
    /// </summary>
    public class ResSingleTon<T> : MonoBehaviour where T : ResSingleTon<T>
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
                        GameObject go = LoadWay.NormalLoad("Manager/" + typeof(T));
#if UNITY_EDITOR
                        if(go==null)
                        Debug.LogError("Manager/" + typeof(T)+"不存在");        
#endif
                        go.name = typeof(T).ToString();
                        instance = FindObjectOfType<T>();
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

        protected virtual void init()
        {
        }
    }
}
public static class LoadWay
{
    public static GameObject NormalLoad(string path)
    {
        GameObject prefab = Resources.Load<GameObject>(path);
#if UNITY_EDITOR
        if (path.Contains("Manager"))
        {
            if (prefab == null) Debug.LogError("未找到单例预制件，请放在Resources的Manager文件夹下");
        }
#endif
        return GameObject.Instantiate(prefab);
    }
    public static GameObject NormalLoad(GameObject go)
    {
        return GameObject.Instantiate(go);
    }

    public static T ResLoad<T>(string path) where T : Object 
    {
        T res = Resources.Load<T>(path);
#if UNITY_EDITOR
        if (res == null) Debug.LogError("未找到资源:"+path);
#endif
        return res;
    }

}