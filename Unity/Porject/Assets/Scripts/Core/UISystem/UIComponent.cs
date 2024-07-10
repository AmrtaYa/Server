using System;
using System.Collections;
using System.Collections.Generic;
using Cysharp.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;


public abstract class UIComponent : MonoBehaviour
{
    public float animSpeed = 1.0f;

    // protected Dictionary<string, string> texts;
    /// <summary>
    /// 动画播放形式，0是正放，1是倒放，也就是返回
    /// </summary>
    [HideInInspector] public UIAnimWay animWay;

    protected bool IfInit = false;

    private void Start()
    {
        if (!IfInit)
        {
            Init();
        }
    }

    /// <summary>
    /// 初始化 
    /// </summary>
    public virtual void Init()
    {
        IfInit = true;
        Translate();
    }

    private void FixedUpdate()
    {
        _FixedUpdate();
    }

    protected virtual void _FixedUpdate()
    {
       
    }

    private void Update()
    {
        _Update();
    }

    private void OnDestroy()
    {
        _Destroy();
    }

    protected virtual void _Update()
    {
    }

    protected virtual void _Destroy()
    {
    }

    /// <summary>
    /// 请在init之前获取组件,翻译  目前用不到
    /// </summary>
    protected virtual void Translate()
    {
        // if (string.IsNullOrEmpty(uiResConfigName))
        // {
        //     return;
        // }

        // if (texts != null) return;
        // LanguageType lt = GameSetting.OtherEntity.SingleDropdown_LanguageType;
        // switch (lt)
        // {
        //     case LanguageType.Chinese:
        //         texts = JsonConfig.Instance.jsonTables.UIResConfig.Get(uiResConfigName).Chinese;
        //         break;
        //     case LanguageType.English:
        //         texts = JsonConfig.Instance.jsonTables.UIResConfig.Get(uiResConfigName).English;
        //         break;
        // }
    }
}