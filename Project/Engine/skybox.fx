#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vLocalDir : TEXCOORD1;
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 vPos = mul(mul(float4(_in.vPos * 2.f, 0.f), matView), matProj);
    vPos.z = vPos.w; // 원근 나누기를 한 후에도 깊이값이 최대인 1이 되도록 미리 z에 w를 넣어줌
    
    output.vPosition = vPos;
    output.vUV = _in.vUV;
    output.vLocalDir = normalize(_in.vPos); // 로컬의 방향 그대로가 곧 물체로 향하는 벡터임
    
    
    return output;
}

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.4f, 0.4f, 0.4f, 1.f);
    
    // Spehere 일때
    if (0 == g_int_0)
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    // Cube 일때 CubeTexture 활용
    if (1 == g_int_0)
    {
        if (g_btexcube_0)
            vColor = g_texcube_0.Sample(g_sam_0, _in.vLocalDir);
    }
    
        
    return vColor;
}

#endif