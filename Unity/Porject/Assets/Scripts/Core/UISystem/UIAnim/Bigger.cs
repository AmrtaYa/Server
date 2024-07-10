using System.Collections;
using System.Collections.Generic;
using System.Threading;
using Cysharp.Threading.Tasks;
using DG.Tweening;
using UnityEngine;

/// <summary>
/// 突然变大一下,需要IUISetValue vec4   前三位是scale 后一位是持续时间
/// </summary>
public class Bigger : IUIAnim
{
    public async UniTask StartAnim(UIComponent uiComponent, CancellationTokenSource cancellationToken = default)
    {
        IUIAnimValue<Vector4> neededValueInterface = uiComponent as IUIAnimValue<Vector4>;

        if (neededValueInterface == null)
        {
            Debug.LogError(uiComponent.gameObject.name + "未带有IUiSetValue<Vector4>接口");
            return;
        }

        var neededValue = neededValueInterface.SetValue();
        var oriValue = uiComponent.transform.localScale;
        float index = 0;
        //变大
        DOTween.To(() => uiComponent.transform.localScale, value => uiComponent.transform.localScale = value,
            new Vector3(neededValue.x,neededValue.y,neededValue.z), neededValue.w
        ).onComplete = () =>
        {
            //变小
            DOTween.To(() => uiComponent.transform.localScale, value => uiComponent.transform.localScale = value,
                oriValue, neededValue.w);
        };

        //uiComponent.transform.localScale = oriValue;


    }
}