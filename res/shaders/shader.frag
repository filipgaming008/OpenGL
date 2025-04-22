#version 460 core
out vec4 FragColor;

uniform vec2 resolution;
uniform float time;
uniform vec3 rayOrigin;
uniform vec3 rayDirection;

float sdSphere( vec3 p, float s )
    {
    return length(p)-s;
}

float sdPlane( vec3 p, vec3 n, float h )
{
  // n must be normalized
  return dot(p,n) + h;
}

float sdBox( vec3 p, vec3 b )
{
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float opSmoothUnion(float d1, float d2, float k) {
  float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
  return mix(d2, d1, h) - k * h * (1.0 - h);
}

float map(vec3 p)
{
    // vec3 q = p;
    // q = fract(q) - 0.5;

    float sphere1 = sdSphere(p + vec3(0.0, sin(time * 0.5), 0.0), 1.0);
    float box1 = sdBox(p + vec3(sin(time * 0.75) * 1.5, 0.0, 0.0), vec3(0.6, 0.6, 0.6));

    float light = sdSphere(p - vec3(sin(time * 0.75) * 5.0, 3.0, cos(time * 0.75) * 5.0), 0.5);

    float h = 1.0;
    vec3 n = vec3(0.0, 1.0, 0.0);
    float ground = sdPlane(p, n, h);

    return min(light, opSmoothUnion(ground, opSmoothUnion(sphere1, box1, 0.5), 0.5));
}

vec3 getNormal(vec3 p) {
    vec2 d = vec2(0.001, 0.0);
    float gx = map(p + d.xyy) - map(p - d.xyy);
    float gy = map(p + d.yxy) - map(p - d.yxy);
    float gz = map(p + d.yyx) - map(p - d.yyx);
    vec3 n = vec3(gx, gy, gz);
    return normalize(n);
}

vec3 calcRD(vec2 uv, float fov) {

    vec3 lookat = rayDirection + rayOrigin;
    vec3 f = normalize(lookat - rayOrigin);
    vec3 r = normalize(cross(vec3(0.0, 1.0, 0.0), f));
    vec3 u = normalize(cross(f, r));
    vec3 c = rayOrigin + f * fov;

    vec3 i = c + uv.x * r + uv.y * u;

    vec3 rd = i - rayOrigin;

    return normalize(rd);
}

float rayMarch(vec3 ro, vec3 rd, float maxdist, float steps) {
    float d = 0.0;
    for(float i = 0.0; i < steps; i++){
        vec3 currPos = ro + rd * d;

        float distToSDF = map(currPos);

        if (distToSDF < 0.001){
            break;
        }

        d += distToSDF;
        
        if (distToSDF > maxdist){
            break;
        }

    }

    return d;
}

void main(){
    // init
    vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / resolution.y;
    float fov = 1.0;
    vec3 rd = calcRD(uv, fov);
    float rayMarchIterations = 200.0;
    float rayMarchDistance = 1000.0;
    vec3 col = vec3(0.0);
    vec3 lightColor = vec3(1.0);
    vec3 lightSource = vec3(sin(time * 0.75) * 5.0, 3.0, cos(time * 0.75) * 5.0);

    float dist = rayMarch(rayOrigin, rd, rayMarchDistance, rayMarchIterations);

    if (dist < rayMarchDistance){
        col = vec3(1.0);

        vec3 p = rayOrigin + rd * dist;
        vec3 n = getNormal(p);

        float lightsdf = sdSphere(p - lightSource, 0.5);
        if(lightsdf < 0.5){
            col = lightColor;
        }else{
            // diffuse lighting;
            float diffuseStrenght = max(0.0, dot(normalize(lightSource), n));
            vec3 diffuse = diffuseStrenght * lightColor / dist;

            // col = diffuse;

            // specular lighting
            vec3 viewS = normalize(rayOrigin);
            vec3 reflectS = normalize(reflect(-lightSource, n));
            float specularStrenght = pow(max(0.0, dot(viewS, reflectS)), 32.0);
            vec3 specular = specularStrenght * lightColor / dist;

            vec3 lighting = diffuse * 0.75 + specular * 0.25;

            col = lighting;

            // shadow
            vec3 lightDirection = normalize(lightSource);
            float distanceToLight = length(lightSource - p);
            vec3 ro = p + n * 0.1;
            vec3 rd = lightDirection;

            float dist = rayMarch(ro, rd, distanceToLight, rayMarchIterations);
            if (dist < distanceToLight || lightsdf > 0.5){
                col *= vec3(0.25);
            }
        }
    }else{
        col = vec3(1.0) / dist;
    }

    // gamma correction
    col = pow(col, vec3(1.0 / 2.2));

    FragColor = vec4(col,1.0);
}