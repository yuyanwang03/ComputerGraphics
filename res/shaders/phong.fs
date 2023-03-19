// They are baricentric interpolated by pixel according to the distance to every vertex
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia, u_Id, u_Is, u_lightPosition, u_Ka, u_Kd, u_Ks, u_eye;
uniform float u_alfa;

// This variables comes from the vertex shader
varying vec3 v_world_position, v_world_normal;
varying vec2 v_uv;


void main()
{
    
    vec3 N = normalize(v_world_normal);
    vec3 L = normalize(u_lightPosition - v_world_position);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(u_eye - v_world_position);
    
    // Check correctness of the formula
    vec3 Ip = u_Ka*u_Ia + (clamp(dot(L, N), 0.0, 1.0))*u_Kd*u_Id + u_Ks*(clamp(pow(dot(R,V), u_alfa),0.0, 1.0))*u_Is;
    gl_FragColor = vec4(Ip, 1.0);
}
