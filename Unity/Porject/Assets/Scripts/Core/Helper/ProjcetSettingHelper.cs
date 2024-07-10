using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.Rendering;

namespace GJC.Helper
{
    public static class ProjcetSettingHelper
    {
        public static void ChangeRender_Intensity_Radius(string urpPath)
        {
            var pipLine = Resources.Load<RenderPipelineAsset>(urpPath);
            QualitySettings.renderPipeline = pipLine;
        }
    }
}