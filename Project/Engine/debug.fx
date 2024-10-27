#ifndef _DEBUG_SHADER
#define _DEBUG_SHADER

#include "value.fx"


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vNormal : NORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
};

VS_OUT VS_DebugShape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    // 동차 좌표가 1이 됐을때 이동을 제대로 적용 받음 
    // 이동을 적용받지 않는 상태로 WV 적용되면 의미없는 값이 된다. 
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    
    return output;
}

float4 PS_DebugShape(VS_OUT _in) : SV_Target
{        
    float4 vColor = g_vec4_0;
    
    if (4 == g_int_0 || 5 == g_int_0)
    {
        float3 vEye = normalize(_in.vViewPos);
        float Alpha = pow(1.f - saturate(dot(vEye, _in.vViewNormal)), 10);
        vColor.a = Alpha;
    }
    
    return vColor;
}

struct VS_LINE_IN
{
    float3 vPos : POSITION;
};

struct VS_LINE_OUT
{
    float3 vPos : POSITION;
};

struct GS_LINE_OUT
{
    float4 vPosition : SV_Position;
};


VS_LINE_OUT VS_DebugLine(VS_LINE_IN _in)
{
    VS_LINE_OUT output = (VS_LINE_OUT) 0.f;
        
    output.vPos = _in.vPos;
    
    return output;
}

[maxvertexcount(12)]
void GS_DebugLine(point VS_LINE_OUT _in[1], inout LineStream<GS_LINE_OUT> _OutStream)
{
    GS_LINE_OUT output[2] = { (GS_LINE_OUT) 0.f, (GS_LINE_OUT) 0.f };
    
    output[0].vPosition = mul(mul(float4(g_vec4_1.xyz, 1.f), matView), matProj);
    output[1].vPosition = mul(mul(float4(g_vec4_2.xyz, 1.f), matView), matProj);
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.RestartStrip();
}

float4 PS_DebugLine(GS_LINE_OUT _in) : SV_Target
{
    return g_vec4_0;
}

#endif