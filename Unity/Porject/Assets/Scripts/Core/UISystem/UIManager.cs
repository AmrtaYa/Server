using System;
using System.Collections.Generic;
using System.Threading;
using Cysharp.Threading.Tasks;
using GJC.Helper;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UIManager : UIManagerBase<UIManager>
{
    protected override void init()
    {
        base.init();
        canvasScaler = GetComponent<CanvasScaler>();
        canvasScaler.referenceResolution = new Vector2(Screen.width, Screen.height);
    }
}


public class UIManagerBase<T> : ResSingleTon<T> where T : ResSingleTon<T>
{
    private EventSystem _eventSystem;
    protected CanvasScaler canvasScaler;
    private Dictionary<UILayer, Transform> layers; //UImanager底下的层级

    public void Init()
    {
    }

    protected override void init()
    {
        base.init();
        UIAnimManager.Instance.ManagerInit();
        CheckUIManagerCompleted();
        CreateLayersIndex();
        DontDestroyOnLoad(gameObject);
        DontDestroyOnLoad(_eventSystem);
    }

    /// <summary>
    /// 初始化UI层
    /// </summary>
    private void CreateLayersIndex()
    {
        layers = new Dictionary<UILayer, Transform>(transform.childCount);
        for (int i = 0; i < transform.childCount; i++)
        {
            var layer = transform.GetChild(i);
            layers.Add(Enum.Parse<UILayer>(layer.name), layer);
        }
    }

    /// <summary>
    /// 检测组件是否齐全
    /// </summary>
    private void CheckUIManagerCompleted()
    {
        _eventSystem = GameObject.FindObjectOfType<EventSystem>();
        if (_eventSystem == null)
        {
            GameObject eventSys = new GameObject("EventSystem");
            _eventSystem = eventSys.AddComponent<EventSystem>();
            eventSys.AddComponent<StandaloneInputModule>();
        }
        //检查子对象是否存在

        var layersName = Enum.GetNames(typeof(UILayer));

        if (transform.childCount < layersName.Length - 1)
        {
            foreach (var value in layersName)
            {
                if (value == "All") continue;
                new GameObject(value).transform.SetParent(transform);
            }
        }
    }

    public T FindUIComponent<T>(string uiName = "") where T : UIComponent
    {
        UIComponent ui;
        if (string.IsNullOrEmpty(uiName))
        {
            ui = transform.GetComponentInChildren<T>();
        }
        else
        {
            ui = transform.FindTheTfByName<T>(uiName);
        }

        return ui as T;
    }

    /// <summary>
    /// 添加UI   UI需要继承UIComponet
    /// </summary>
    /// <param name="UIName">UI的路径名字</param>
    /// <param name="uiLayer">UI添加的层级 默认Mid层</param>
    /// <param name="pos">UI的位置  默认0,0</param>
    /// <param name="beforeInit">在UI组件初始化之间，所需要执行的代码</param>
    /// <typeparam name="T"></typeparam>
    /// <returns></returns>
    public T AddUI<T>(string UIName, UILayer uiLayer = UILayer.Mid, Vector2 pos = default,
        Action<T> beforeInit = null) where T : UIComponent
    {
        if (beforeInit != null)
        {
            Action<UIComponent> action = (t) => { beforeInit((T)t); };
            return AddUIOri(UIName, uiLayer, pos, action, false) as T;
        }

        return AddUIOri(UIName, uiLayer, pos, null, false) as T;
    }

    public async UniTask<T> AddUI<T>(UIAnimPackage package, string UIName, UILayer uiLayer = UILayer.Mid,
        Vector2 pos = default,
        Action<T> beforeInit = null) where T : UIComponent
    {
        T ui = null;
        if (beforeInit != null)
        {
            Action<UIComponent> action = (t) => { beforeInit((T)t); };
            ui = AddUIOri(UIName, uiLayer, pos, action, false) as T;
            ui.animWay = package.UIAnimWay;
            await package.anim.StartAnim(ui, package.cancelToken);

            return ui;
        }

        ui =  AddUIOri(UIName, uiLayer, pos, null, false) as T;

        ui.animWay = package.UIAnimWay;
        await package.anim.StartAnim(ui, package.cancelToken);
        return ui;
    }

    public T AddUIWithPool<T>(string UIName, UILayer uiLayer = UILayer.Mid, Vector2 pos = default,
        Action<T> beforeInit = null) where T : UIComponent
    {
        if (beforeInit != null)
        {
            Action<UIComponent> action = (t) => { beforeInit((T)t); };
            return AddUIOri(UIName, uiLayer, pos, action, true) as T;
        }

        return AddUIOri(UIName, uiLayer, pos, null, true) as T;
    }

    /// <summary>
    /// 添加UI   UI需要继承UIComponet
    /// </summary>
    /// <param name="UIName">UI的路径名字</param>
    /// <param name="uiLayer">UI添加的层级 默认Mid层</param>
    /// <param name="pos">UI的位置  默认0,0</param>
    /// <param name="beforeInit">在UI组件初始化之间，所需要执行的代码</param>
    /// <typeparam name="T"></typeparam>
    /// <returns></returns>
    public UIComponent AddUI(string UIName, UILayer uiLayer = UILayer.Mid, Vector2 pos = default,
        Action<UIComponent> beforeInit = null)
    {
        return AddUI<UIComponent>(UIName, uiLayer, pos, beforeInit);
    }

    private UIComponent AddUIOri(string UIName, UILayer uiLayer = UILayer.Mid, Vector2 pos = default,
        Action<UIComponent> beforeInit = null, bool GameObjectPool = false)
    {
        // GameObject go = JsonConfig.Instance.jsonTables.UIResConfig.GetInstance(UIName);
        GameObject go = null;
        if (GameObjectPool)
        {
            go = GJC.Helper.GameObjectPool.Instance.Get(UIName, LoadWay.ResLoad<GameObject>(UIName), pos,
                Quaternion.identity);
        }
        else
        {
            go = LoadWay.NormalLoad(UIName);
        }

        go.transform.SetParent(layers[uiLayer]);
        go.name = UIName;
        if (pos == default)
        {
            go.transform.position = new Vector3(Screen.width / 2, Screen.height / 2, 0);
            var rectTr = go.GetComponent<RectTransform>();
            if (rectTr.anchorMin == Vector2.zero && rectTr.anchorMax == Vector2.one)
            {
                rectTr.offsetMin = Vector2.zero;
                rectTr.offsetMax = Vector2.zero;
            }
        }
        else
            go.transform.position = new Vector3(pos.x, pos.y, 0);

        var uiComponent = go.GetComponent<UIComponent>();
        // uiComponent.uiResConfigName = UIName;
        if (beforeInit != null)
            beforeInit(uiComponent);
        if (uiComponent == null)
            Debug.LogError(UIName + "未添加UIComponent组件");
        uiComponent.Init();
      

        return uiComponent;
    }

    /// <summary>
    /// 清空UI，默认全部清空
    /// </summary>
    /// <param name="layerIndex">设置层，清空相应的层</param>
    public void ClearUI(UILayer layerIndex = UILayer.All)
    {
        List<Transform> trIndex = new List<Transform>();
        if (layerIndex == UILayer.All)
        {
            foreach (var layer in layers.Values)
            {
                for (int i = 0; i < layer.childCount; i++)
                {
                    trIndex.Add(layer.GetChild(i));
                }
            }
        }
        else
        {
            for (int i = 0; i < layers[layerIndex].childCount; i++)
            {
                trIndex.Add(layers[layerIndex].GetChild(i));
            }
        }

        foreach (var layer in trIndex)
        {
            Destroy(layer.gameObject);
        }
    }

    public void ClearUI(UIComponent ui)
    {
        if (ui == null) return;
        IUIClear clearInterface = ui as IUIClear;
        if(clearInterface!=null) clearInterface.Clear();
        Destroy(ui.gameObject);
    }
    public void ClearUIWithPool(UIComponent ui)
    {
        if (ui == null) return;
        IUIClear clearInterface = ui as IUIClear;
        if(clearInterface!=null) clearInterface.Clear();
        GameObjectPool.Instance.PoolDestroy(ui.gameObject);
    }
    public void ReleaseUIWithPool(UIComponent ui)
    {
        if (ui == null) return;
        IUIClear clearInterface = ui as IUIClear;
        if(clearInterface!=null) clearInterface.Clear();
        GameObjectPool.Instance.Release(ui.gameObject);
    }
    public async UniTask ClearUI(UIComponent ui, UIAnimPackage thisAnimType = default)
    {
        if (ui == null) return;
        if (thisAnimType.anim != null)
        {
            ui.animWay = thisAnimType.UIAnimWay;
            await thisAnimType.anim.StartAnim(ui, thisAnimType.cancelToken);
        }

        IUIClear clearInterface = ui as IUIClear;
        if(clearInterface!=null) clearInterface.Clear();
        Destroy(ui.gameObject);
    }

    /// <summary>
    /// 获取层内的UI数量
    /// </summary>
    /// <param name="uiLayer"></param>
    /// <typeparam name="T"></typeparam>
    /// <returns></returns>
    public int GetUINumbs<T>(UILayer uiLayer = UILayer.Mid)
    {
        Debug.LogError("目前不可用  GetUINumbs");
        return 0;
    }

    public Transform GetLayer(UILayer layer)
    {
        return layers[layer];
    }
}

public struct UIAnimPackage
{
    /// <summary>
    /// 动画类型
    /// </summary>
    public IUIAnim anim;

    /// <summary>
    /// 可以取消的动画
    /// </summary>
    public CancellationTokenSource cancelToken;

    /// <summary>
    /// 动画播放顺序
    /// </summary>
    public UIAnimWay UIAnimWay;
}

public static class UIHelper
{
    public static async UniTask SetUIAnim<T>(this UIComponent ui,float waitTime = 0 , CancellationTokenSource token = default)
        where T : class, IUIAnim
    {
        string uiName = ui.gameObject.name;
        try
        {
            await UniTask.WaitForSeconds(waitTime);
            await UIAnimManager.Instance.GetAnim<T>().StartAnim(ui, token);
        }
        catch (Exception e)
        {
#if UNITY_EDITOR
            Debug.Log("删除了空引用:" + uiName+"\n"+e);
#endif
        }
    }

    //切换窗口
    public static async UniTask<T> ChangeWindows<T>(this UIComponent ui, string targetUIPath,
        UIAnimPackage thisAnimType = default, UIAnimPackage targetAnimType = default, UILayer layer = UILayer.Mid)
        where T : UIComponent
    {
        await UIManager.Instance.ClearUI(ui, thisAnimType);


        T uiComponent = await UIManager.Instance.AddUI<T>(targetAnimType, targetUIPath, layer);

        return uiComponent;
    }

    //获取鼠标点击的UI
    public static GameObject GetFirstPickGameObject(Vector2 position)
    {
        EventSystem eventSystem = EventSystem.current;
        PointerEventData pointerEventData = new PointerEventData(eventSystem);
        pointerEventData.position = position;
        //射线检测ui
        List<RaycastResult> uiRaycastResultCache = new List<RaycastResult>();
        eventSystem.RaycastAll(pointerEventData, uiRaycastResultCache);
        if (uiRaycastResultCache.Count > 0)
            return uiRaycastResultCache[0].gameObject;
        return null;
    }
    public static List<GameObject> GetPickGameObject(Vector2 position)
    {
        EventSystem eventSystem = EventSystem.current;
        PointerEventData pointerEventData = new PointerEventData(eventSystem);
        pointerEventData.position = position;
        //射线检测ui
        List<RaycastResult> uiRaycastResultCache = new List<RaycastResult>();
        eventSystem.RaycastAll(pointerEventData, uiRaycastResultCache);
        if (uiRaycastResultCache.Count > 0)
            return uiRaycastResultCache.ConvertAll(r => r.gameObject);
        return new List<GameObject>();
    }
}

public interface IUIClear
{
    void Clear();
}

public enum UILayer
{
    All,
    Bot,
    Bot1,
    Bot2,
    Mid,
    Mid1,
    Mid2,
    Top,
    Top1,
    Top2,
}