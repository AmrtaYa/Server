using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class BtnImagePointerEvent : MonoBehaviour,
    IPointerEnterHandler,IPointerExitHandler,IDragHandler,IEndDragHandler,IBeginDragHandler
{
    public Action<PointerEventData> pointerEnter;
    public Action<PointerEventData> pointerExit;
    public Action<PointerEventData> dragAction;
    public Action<PointerEventData> dragEnd;
    public Action<PointerEventData> dragStart;


    public void OnPointerEnter(PointerEventData eventData)
    {
        if (pointerEnter != null) pointerEnter(eventData);
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        if (pointerExit != null) pointerExit(eventData);
    }

    public void OnDrag(PointerEventData eventData)
    {
        if (dragAction != null) dragAction(eventData);
    }

    public void OnEndDrag(PointerEventData eventData)
    {
        if (dragEnd != null) dragEnd(eventData);
    }

    public void OnBeginDrag(PointerEventData eventData)
    {
        if (dragStart != null) dragStart(eventData);
    }
}
