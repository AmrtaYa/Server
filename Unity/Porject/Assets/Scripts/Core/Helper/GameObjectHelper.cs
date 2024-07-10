using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class GameObjectHelper
{
  public static T AddOrGetComponent<T>(this MonoBehaviour mono) where T: Component
  {
     T component =  mono.GetComponent<T>();
     if (component == null) component = mono.AddOrGetComponent<T>();
     return component;
  }
}
