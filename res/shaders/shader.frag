#version 460 core
out vec4 FragColor;

uniform vec2 resolution;
uniform float time;
uniform vec3 movement;
//uniform vec2 mouse;

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float dfunc(vec3 p)
{
    vec3 box1pos = vec3(sin(time), 0.0, 0.0);
    float box1 = sdBox(p + box1pos, vec3(0.5,0.5,1.5));
    float sphere1 = sdSphere(p, 1.0);
    float ground = p.y + 1.0;
    
    return min(ground , max(sphere1, -box1));
}

mat2 rotate(float a){
    
    float q = sin(a);
    float w = cos(a);

    return mat2(q, -w, w, q);
}



void main(){
    vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / resolution.y;
    // vec2 mouse = (mouse.xy * 2.0 - resolution.xy) / resolution.y;
    
    // init
    vec3 rayOrigin = vec3(0.0, 0.0, -3.0);
    vec3 rayDirection = normalize(vec3(uv, 1.0));
    vec3 col = vec3(0.0);
    
    float rayMarchIterations = 200.0;
    
    float d = 0.0;
    
    rayOrigin += movement;
    rayDirection += movement;

    // rayOrigin.xz *= rotate(-mouse.x);
    // rayDirection.xz *= rotate(-mouse.x);
    
    
    for(float i = 0.0; i < rayMarchIterations; i++){

        vec3 p = rayOrigin + rayDirection * d;

        float dist_to_obj = dfunc(p);

        d += dist_to_obj;
        
        // col = vec3(i) / rayMarchIterations;
        
        if (d < 0.001 || d > 100.0) break;
    }
    
    col = vec3(d * 0.1);
    
    FragColor = vec4(col,1.0);
}