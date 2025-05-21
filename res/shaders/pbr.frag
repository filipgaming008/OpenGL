#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;  

struct Material {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D heightMap;
    float heightScale;
};

uniform Material material;

uniform vec3 viewPos;
uniform vec3 lightPos;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 tV){

    const float minLayers = 16.0;
    const float maxLayers = 64.0;
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), tV), 0.0));

    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = tV.xy * material.heightScale; 
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(material.heightMap, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(material.heightMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    return currentTexCoords;
}

void main(){

    vec3 tV = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec2 texCoords = ParallaxMapping(fs_in.TexCoords, tV);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 albedo = pow(texture(material.albedoMap, texCoords).rgb, vec3(2.2));
    float metallic = texture(material.metallicMap, texCoords).r;
    float roughness = texture(material.roughnessMap, texCoords).r;
    float ao = texture(material.aoMap, texCoords).r;

    vec3 N = texture(material.normalMap, texCoords).rgb;
    N = normalize(N * 2.0 - 1.0);
    N = normalize(fs_in.TBN * N);


    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    vec3 V = normalize(viewPos - fs_in.FragPos);
    vec3 L = normalize(lightPos - fs_in.FragPos);
    vec3 H = normalize(V + L);

    float distance = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = vec3(23.47, 21.31, 20.79) * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 K_s = F;
    vec3 K_d = vec3(1.0) - K_s;
    K_d *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(N, L), 0.0);
    Lo += (K_d * albedo / PI + specular) * radiance * NdotL; 

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 result = ambient + Lo;

    // Gamma correction
    result = result / (result + vec3(1.0));
    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, 1.0f);
}