using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class OverrideImage : Image
{
    public override bool IsRaycastLocationValid(Vector2 screenPoint, Camera eventCamera)
    {
        Vector2 localPoint;
        RectTransformUtility.ScreenPointToLocalPointInRectangle(rectTransform, screenPoint, eventCamera, out localPoint);
 
        Vector2 pivot = rectTransform.pivot;
        Vector2 normalizedLocal = new Vector2(pivot.x + localPoint.x / rectTransform.rect.width, pivot.y + localPoint.y /rectTransform.rect.height);
        Vector2 uv = new Vector2(
            sprite.rect.x + normalizedLocal.x * sprite.rect.width, 
            sprite.rect.y + normalizedLocal.y * sprite.rect.height );
 
        uv.x /= sprite.texture.width;
        uv.y /= sprite.texture.height;
 
        //uv are inversed, as 0,0 or the rect transform seem to be upper right, then going negativ toward lower left...
        Color c = sprite.texture.GetPixelBilinear(uv.x, uv.y);
 
        return c.a> 0.1f;

    }
}