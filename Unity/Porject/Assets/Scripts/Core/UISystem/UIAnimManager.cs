using System;
using System.Collections.Generic;
using System.Threading;
using Cysharp.Threading.Tasks;
using DG.Tweening;
using GJC.Helper;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// 添加新动画，请继承IUIAnim下的2个不同分区接口，然后在枚举中添加
/// 下载Unitask与Mathematics
/// </summary>
public class UIAnimManager : SharpSington<UIAnimManager>, ICanInit
{
    private Dictionary<AnimType, IUIAnim> animDirs;

    private void InitAnim()
    {
        var animTypes = Enum.GetNames(typeof(AnimType));
        animDirs = new Dictionary<AnimType, IUIAnim>(animTypes.Length);
        foreach (var anim in animTypes)
        {
            if (anim == AnimType.Null.ToString()) continue; //跳过Null选项
            Type type = Type.GetType(anim);
            IUIAnim uianimInstance = Activator.CreateInstance(type) as IUIAnim;
            if (uianimInstance == null)
                Debug.LogError(anim + "实例不存在");
            animDirs.Add(Enum.Parse<AnimType>(anim), uianimInstance);
        }
    }

    /// <summary>
    /// 需要在AnimType中添加相对于类型
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <returns></returns>
    public T GetAnim<T>() where T : class, IUIAnim
    {
        AnimType animType = Enum.Parse<AnimType>(typeof(T).ToString());
        var index = animDirs[animType] as T;
        return index;
    }

    public void ManagerInit()
    {
    }

    public void Init()
    {
        InitAnim();
    }
}

public enum AnimType
{
    Null = -1,
    Cross,
    CrossIn,
    CrossOut,
    MoveTo,
    Bigger
}

/// <summary>
/// 动画播放方式
/// </summary>
public enum UIAnimWay
{
    ForWards,//正放
    BackWards//倒放
}

public interface IUIAnim
{
    UniTask StartAnim(UIComponent uiComponent, CancellationTokenSource cancellationToken = default);
}

public interface IUIAnimValue<T>
{
    T SetValue();
}

/// <summary>
/// 淡入并且淡出
/// </summary>
public class Cross : IUIAnim
{
    private float duaTime = 0.5f;

    public async UniTask StartAnim(UIComponent uiComponent, CancellationTokenSource cancellationToken = default)
    {
        IUIAnimValue<List<MaskableGraphic>> imageValues = uiComponent as IUIAnimValue<List<MaskableGraphic>>;
        if (imageValues == null) Debug.LogError(uiComponent.name + "未实现IUIAnimValue<List<MaskableGraphic>>接口");
        List<MaskableGraphic> images = imageValues.SetValue();
        IUIAnimValue<float> dur = uiComponent as IUIAnimValue<float>;
        float indexDur = duaTime;
        if (dur != null) indexDur = dur.SetValue();

        if (cancellationToken == default) cancellationToken = new CancellationTokenSource();
        List<Color> color = new List<Color>(images.Count);
        for (int i = 0; i < images.Count; i++) //先全部隐藏掉
        {
            Color c = images[i].color;
            c.a = 0;
            images[i].color = c;
            color.Add(c);
        }

        float aplha = 0.0f;
        while (aplha <= 1.0f) //亮起来
        {
            for (int i = 0; i < color.Count; i++)
            {
                Color c = color[i];
                c.a = aplha;
                color[i] = c;
                images[i].color = color[i];
            }

            if (cancellationToken.Token.IsCancellationRequested)
            {
                //如果取消了，那么恢复颜色
                for (int i = 0; i < color.Count; i++)
                {
                    Color c = color[i];
                    c.a = 0.0f;
                    color[i] = c;
                    images[i].color = color[i];
                }

                return;
            }

            await UniTask.WaitForEndOfFrame();
            aplha += Time.deltaTime * uiComponent.animSpeed / indexDur;
        }

        while (aplha > 0F) //暗下去起来
        {
            for (int i = 0; i < color.Count; i++)
            {
                Color c = color[i];
                c.a = aplha;
                color[i] = c;
                images[i].color = color[i];
            }

            if (cancellationToken.Token.IsCancellationRequested)
            {
                //如果取消了，那么恢复颜色
                for (int i = 0; i < color.Count; i++)
                {
                    Color c = color[i];
                    c.a = 0.0f;
                    color[i] = c;
                    images[i].color = color[i];
                }

                return;
            }

            await UniTask.WaitForEndOfFrame();
            aplha -= Time.deltaTime * uiComponent.animSpeed / indexDur;
        }
    }
}

/// <summary>
/// 淡入  无值传递(利用animSpeed)
/// </summary>
public class CrossIn : IUIAnim
{
    private float duaTime = 0.5f;

    public async UniTask StartAnim(UIComponent uiComponent, CancellationTokenSource cancellationToken = default)
    {
        IUIAnimValue<List<MaskableGraphic>> imageValues = uiComponent as IUIAnimValue<List<MaskableGraphic>>;
        if (imageValues == null) Debug.LogError(uiComponent.name + "未实现IUIAnimValue<List<MaskableGraphic>>接口");
        List<MaskableGraphic> images = imageValues.SetValue();
        IUIAnimValue<float> dur = uiComponent as IUIAnimValue<float>;
        float indexDur = duaTime;
        if (dur != null) indexDur = dur.SetValue();
        for (int i = 0; i < images.Count; i++) //先全部隐藏掉
        {
            Color c = images[i].color;
            c.a = 0;
            images[i].color = c;
        }

        int finishNum = 0;
        for (int i = 0; i < images.Count; i++)
        {
            images[i].DOFade(1, indexDur * uiComponent.animSpeed).onComplete = () => { finishNum++; };
        }

        await UniTask.WaitUntil(() => { return finishNum >= images.Count; })
            .AttachExternalCancellation(cancellationToken.Token).SuppressCancellationThrow();
    }
}

/// <summary>
/// 淡出  无值传递(利用animSpeed)
/// </summary>
public class CrossOut : IUIAnim
{
    private float duaTime = 0.5f;

    public async UniTask StartAnim(UIComponent uiComponent, CancellationTokenSource cancellationToken = default)
    {
        IUIAnimValue<List<MaskableGraphic>> imageValues = uiComponent as IUIAnimValue<List<MaskableGraphic>>;
        if (imageValues == null) Debug.LogError(uiComponent.name + "未实现IUIAnimValue<List<MaskableGraphic>>接口");
        List<MaskableGraphic> images = imageValues.SetValue();
        IUIAnimValue<float> dur = uiComponent as IUIAnimValue<float>;
        float indexDur = duaTime;
        if (dur != null) indexDur = dur.SetValue();
        for (int i = 0; i < images.Count; i++) //先全部隐藏掉
        {
            Color c = images[i].color;
            c.a = 1;
            images[i].color = c;
        }

        int finishNum = 0;
        for (int i = 0; i < images.Count; i++)
        {
            images[i].DOFade(0, indexDur * uiComponent.animSpeed).onComplete = () => { finishNum++; };
        }

        await UniTask.WaitUntil(() => { return finishNum >= images.Count; })
            .AttachExternalCancellation(cancellationToken.Token).SuppressCancellationThrow();
        
    }
}

/// <summary>
/// 从X点到Y点        需继承IUIAnimValue<float4[]>  XYZW 分别对应Rect  数组长度为2，起点，终点
/// </summary>
public class MoveTo : IUIAnim
{
    public async UniTask StartAnim(UIComponent uiComponent, CancellationTokenSource cancellationToken = default)
    {
        IUIAnimValue<float4[]> animValue = uiComponent as IUIAnimValue<float4[]>;
        if (animValue == null) Debug.LogError(uiComponent.gameObject.name + "需要继承IUIAnimValue<float4[]>");
        var value = animValue.SetValue();
        float4 startPos = value[0];
        float4 endPos = value[1];

        await AnimMove(uiComponent, startPos, endPos);
    }

    private static async UniTask AnimMove(UIComponent uiComponent, float4 startPos, float4 endPos)
    {
        RectTransform transform = uiComponent.GetComponent<RectTransform>();
        float offSetIndex = 0;
        offSetIndex = Vector4.Distance(startPos, endPos);
        transform.offsetMin = new Vector2(startPos.x, startPos.y);
        transform.offsetMax = new Vector2(startPos.z, startPos.w);
        while (offSetIndex > 0.1f)
        {
            await UniTask.DelayFrame(1);
            if (transform == null) return;
            transform.offsetMin = Vector2.Lerp(transform.offsetMin, new Vector2(endPos.x, endPos.y),
                0.4f * uiComponent.animSpeed);
            transform.offsetMax = Vector2.Lerp(transform.offsetMax, new Vector2(endPos.z, endPos.w),
                0.4f * uiComponent.animSpeed);
            offSetIndex =
                Vector4.Distance(
                    new Vector4(transform.offsetMin.x, transform.offsetMin.y, transform.offsetMax.x,
                        transform.offsetMax.y), endPos);
        }
    }
}