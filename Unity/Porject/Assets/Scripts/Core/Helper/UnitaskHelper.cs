using System.Collections;
using System.Collections.Generic;
using System.Threading;
using Cysharp.Threading.Tasks;
using UnityEngine;

public static class UnitaskHelper
{
    private static CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
    
    public static void StopAllTask()
    {
        cancellationTokenSource.Cancel();
    }

    public static CancellationTokenSource GetCancel()
    {
        if (cancellationTokenSource.IsCancellationRequested)
            cancellationTokenSource = new CancellationTokenSource();
        return cancellationTokenSource;
    }
    

}