cbuffer ConstantBufferData : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}
// D�finition d'une structure pour la lumi�re
struct Light
{
    float3 position;
    float3 intensity;
    float3 direction;
};

// Ajout d'un buffer constant pour la lumi�re
cbuffer LightBuffer : register(b1)
{
    Light lights[1]; // Pour cet exemple, nous utilisons une seule lumi�re
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL; // Ajouter le champ normal ici
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR, float3 normal : NORAMAL)
{
    PSInput result;
   
     // Assurez-vous de transformer �galement la normale si n�cessaire
    // Cela d�pend de votre pipeline, si vous transformez les normales dans le vertex shader ou pas
    float4 worldPosition = mul(position, World);
    float3 worldNormal = mul(normal, (float3x3) World); // Transformer la normale en espace du monde
    
    
    result.position = mul(mul(mul(position, World), View), Projection);
    result.color = color;
    result.normal = worldNormal; // Assigner la normale transform�e

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(lights[0].position - input.position.xyz);
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diff * lights[0].intensity;
    float3 ambient = float3(0.1, 0.1, 0.1); // Lumi�re ambiante simple
    float3 color = input.color.rgb * (diffuse + ambient); // Moduler la couleur par l'�clairage
    return float4(color, input.color.a); // Utiliser l'alpha de la couleur d'entr�e
}
