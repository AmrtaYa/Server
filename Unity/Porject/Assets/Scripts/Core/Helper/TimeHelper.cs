using System.Collections;
using System.Collections.Generic;
using Cysharp.Threading.Tasks;
using UnityEngine;

public static class TimeHelper
{
    public  static string ChangeTimeFormat(float playTime)
    {
        int t = (int)playTime;
        int hour = (t / 3600);
        int min = (t - hour * 3600) / 60;
        int sec = t - hour * 3600 - min * 60;

        string hourStr = hour < 10 ? $"0{hour}" : hour.ToString();
        string minStr = min < 10 ? $"0{min}" : min.ToString();
        string secStr = sec < 10 ? $"0{sec}" : sec.ToString();

        return $"{hourStr}:{minStr}:{secStr}";
    }
}
