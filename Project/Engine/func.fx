#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

int IsBinding(Texture2D _tex)
{
    int width = 0, height = 0;
    _tex.GetDimensions(width, height);
    
    if (0 == width || height == 0)
        return false;
    
    return true;
}

void CalculateLight2D(int _LightIdx, float3 _WorldPos, inout tLight _Light)
{          
    tLightInfo Info = g_Light2D[_LightIdx];
    
    // DirectionalLight 인 경우
    if (0 == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;        
    }
    
    // PointLight 인 경우
    else if (1 == Info.Type)
    {
         // 점광원과 픽셀까지의 거리
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);
        
        // 거리값을 각도로 치환해서 거리에 따른 빛의 세기를 코사인 그래프 형태로 사용한다.
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
        
        // 광원으로부터 떨어진 거리에 따른 빛의 세기
        //float fPow2 = saturate(1.f - fDist / Info.Radius);
        
        // 최종 색상 계산 = 빛의 색 * 거리에따른 세기
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    
    // SpotLight 인 경우
    else
    {
        
    }    
}

float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_EngineTime * 0.5f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_1, vUV, 0).xyz;
    
    return vRandom;
}

#endif