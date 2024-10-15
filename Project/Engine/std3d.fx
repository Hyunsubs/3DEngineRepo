#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
   
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};


VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
   
    // 정점의 ViewSpace 좌표
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    
    // 뷰 Space 에서 정점에서의 표면의 수직방향
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);  
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), matWV).xyz);  
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), matWV).xyz);  
    
    return output;
}

// 노멀 맵핑 특징
// 추출한 값의 범위 0 ~ 1 을 -1 ~ 1로 변환하여 사용해야 한다.
// 2. Normal 텍스처 안에 들어있는 노말값의 좌표 축이 다르다.
// xyz != xzy(rgb)

// 3. 좌표계
// 알록달록한 색상의 노멀 텍스처 -> Mesh에 특정하여 만든 텍스처 -> 연산이 빠르지만 사용처가 제한적임
// 블루 계열의 노멀 텍스처 -> Tangent Space 기준으로 노말값이 저장돼 있음 -> 무조건 표면에서 뻗어나오는 구조기 때문에 값이 파란색이(b = y) 값이 많이 나옴
// 범용적으로 사용 가능하다는 장점이 있음 계산이 좀 더 많이 필요하지만 현대 GPU에선 의미 없을 정도의 연산량임

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    float3 vViewNormal = _in.vViewNormal;
    
    
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    if (g_btex_1)
    {
        vViewNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        
        // 추출한 값의 범위를 -1~1로 확장한다.
        vViewNormal = vViewNormal * 2.f - 1.f;
        
        // 표면 좌표계(Tangent Space) 기준 방향을 View Space로 적용시키기 위해서
        // 적용시킬 곳의 표면 정보 (T,B,N)이 필요하다.
        // 표면 정보로 회전 행렬을 구성한다.
        
        float3x3 matRot =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal
        };

        // TangentSpace 방향 정보를 적용시킬 표면의 좌표계로 가져온다.
        vViewNormal = mul(vViewNormal, matRot);
    }
    
    
    tLight light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light3DCount; ++i)
    {
        CalculateLight3D(i, vViewNormal, _in.vViewPos, light);
    }
    
    // 계산한 빛을 물체의 색상에 적용
    vOutColor.xyz =   vOutColor.xyz * light.Color.rgb
                    + vOutColor.xyz * light.Ambient.rgb
                    + g_Light3D[0].light.Color.rgb * light.SpecCoef;
        
    return vOutColor;
}

#endif