#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;

    return output;
}

struct TessFactor
{
    float arrEdge[3] : SV_TessFactor;
    float Insinde : SV_InsideTessFactor;
};

TessFactor PathConstantFunc(InputPatch<VS_OUT, 3> _in, uint _PatchIdx : SV_PrimitiveID)
{
    TessFactor output = (TessFactor) 0.f;
    
    output.arrEdge[0] = 1;
    output.arrEdge[1] = 1;
    output.arrEdge[2] = 1;
    output.Insinde = 1;
    
    return output;

}


struct HS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
[partitioning("integer")]
[outputtoppology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunc")]
[maxtessfactor(64)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _in,
                    uint _PatchIdx : SV_PrimitiveID,
                    uint _ControlPointID : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vLocalPos = _in[_ControlPointID].vLocalPos;
    output.vUV = _in[_ControlPointID].vUV;
    
    return output;

}

// Domain Shader

struct DS_OUT
{
    float4 Position : SV_Position;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
DS_OUT DS_LandScape(OutputPatch<HS_OUT, 3> _in, float3 _Weight : SV_DomainLocation
                   , TessFactor _PatchTessFactor)
{
    HS_OUT input = (HS_OUT) 0.f;
    for (int i = 0; i < 3; ++i)
    {
        input.vLocalPos += _in[i].vLocalPos * _Weight[i];
        input.vUV += _in[i].vUV * _Weight[i];
    }
    
    DS_OUT output = (DS_OUT) 0.f;
    
    output.Position = mul(float4(input.vLocalPos, 1.f), matWVP);
    output.vUV = input.vUV;
    
    return output;

}

struct PS_OUT
{
    float4 vAlbedo : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vEmissive : SV_Target3;
    float4 vData : SV_Target4;
};


PS_OUT PS_LandScape(DS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    output.vAlbedo = float4(1.f, 0.f, 0.f, 1.f);
    
    return output;

}

#endif 