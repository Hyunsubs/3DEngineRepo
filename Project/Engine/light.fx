#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET;
    float4 vSpecular : SV_TARGET1;
};


// ================================
// Directional Light Shader 
// MRT  : LIGHT (Diffuse, Specular)
// Mesh : RectMesh
// Rasterizer   : CULL_BACK
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : ONE_ONE
// Parameter
#define LIGHT_IDX       g_int_0
#define POS_TARGET      g_tex_0
#define NORMAL_TARGET   g_tex_1
// ================================

VS_OUT VS_DirLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}


PS_OUT PS_DirLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // �ȼ����̴��� ������ �������� Position ���� Ȯ���Ѵ�.
    float4 vViewPos = POS_TARGET.Sample(g_sam_0, _in.vUV);
    
    // ������ �ε��� ��ü�� �������� �ʴ´�.
    if (0.f == vViewPos.w)
        discard;
    
    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, _in.vUV).xyz;
    
    // �ش� ������ ���� ���� ���⸦ ����Ѵ�.
    tLight light = (tLight) 0.f;
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
        
    output.vDiffuse = light.Color + light.Ambient;
    output.vSpecular = light.SpecCoef;
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}

// ================================
// Point Light Shader 
// MRT  : LIGHT (Diffuse, Specular)
// Mesh : RectMesh
// Rasterizer   : CULL_BACK
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : ONE_ONE
// Parameter
#define LIGHT_IDX       g_int_0
#define POS_TARGET      g_tex_0
#define NORMAL_TARGET   g_tex_1
// ================================


VS_OUT VS_PointLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f; 
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

PS_OUT PS_PointLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // ��ũ�� ��ǥ / ��ü �ػ� = ��ü ȭ�� �������� ��ǥ�� �ش��ϴ� UV
    float2 vRenderUV = _in.vPosition.xy / g_Resolution;
    
    // SV_Position���� ��ũ�� ��ǥ ȹ���Ͽ� Ȯ��
    float4 vViewPos = POS_TARGET.Sample(g_sam_0, vRenderUV);
    
    // ������ �ε��� ��ü�� �������� �ʰų� Stencil ���� 0�̴�
    if (0.f == vViewPos.w)
        discard;
    
    // Decal
    // �ش� ��ü�� ���� �޽� �������� ����
    // ������ ��ü�� ViewPos�� WorldPos �� �����Ѵ�.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // World �� �ִ� ��ü�� ��ǥ��, Volume Mesh �� ���� ������� ���ؼ� Local �������� ��������.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    // ��ü�� ���� �޽� ���� ���̶�� ���� ��� �ߴ�
    if (0.5f < length(vLocalPos))
    {
        discard;
    }  
    
    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, vRenderUV).xyz;

    // �ش� ������ ���� ���� ���⸦ ����Ѵ�.
    tLight light = (tLight) 0.f;
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
        
    output.vDiffuse = light.Color + light.Ambient;
    output.vSpecular = light.SpecCoef;
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    
    return output;
}







#endif