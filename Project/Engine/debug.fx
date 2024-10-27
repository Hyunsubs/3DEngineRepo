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
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    
    return output;
}

float4 PS_DebugShape(VS_OUT _in) : SV_Target
{        
    float4 vColor = g_vec4_0;
    
    if (4 == g_int_0)
    {
        float3 vEye = normalize(_in.vViewPos);        
        float Alpha = pow(1.f - saturate(dot(vEye, _in.vViewNormal)), 3);
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
    GS_LINE_OUT output[3] = { (GS_LINE_OUT) 0.f, (GS_LINE_OUT) 0.f, (GS_LINE_OUT) 0.f };
    
    output[0].vPosition = mul(mul(g_vec4_1, matView), matProj);
    output[1].vPosition = mul(mul(g_vec4_2, matView), matProj);
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
   
    _OutStream.RestartStrip();
}

float4 PS_DebugLine(GS_LINE_OUT _in) : SV_Target
{
    return g_vec4_0;
}

#endif