using System;
using System.Collections.Generic;
using SQLite4Unity3d;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// 数据库单个实体信息
/// </summary>
public struct DBEntity : IDataBase
{
    [PrimaryKey] public int ID { get; set; }

    public int ID2 { get; set; }
    // public int ID3 { get; set; }
    //  public int ID;

    public int GetPrimKey()
    {
        return ID;
    }

    public Type[] dependEntity()
    {
        return null;
    }

    public IDataBase GetDefaultEntity(int id)
    {
        return new DBEntity() { ID = id };
    }
}
public interface IDataBase
{
    public int GetPrimKey();

    public Type[] dependEntity();
    public IDataBase GetDefaultEntity(int id);
}