// They are baricentric interpolated by pixel according to the distance to every vertex
uniform mat4 u_model;
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia, u_Id, u_Is, u_lightPosition, u_Ka, u_Ks, u_eye, u_flags;
uniform float u_alfa, u_addAmbient;

// This variables comes from the vertex shader
varying vec3 v_world_position, v_world_normal;
varying vec2 v_uv;


void main()
{
    // Get normal vector from texture
    vec3 N = texture2D(u_normalTexture, v_uv).xyz;
    // Apply necessary transformations
    N = (u_model*vec4(N * 2.0 - vec3(1.0), 0.0)).xyz;
    // N = (vec4(N * 2.0 - vec3(1.0), 0.0)*u_model).xyz;
    
    // Apply mix factor (optional)
    
    float mix_factor = 0.3;
    N.xyz = mix(v_world_normal, N.xyz, mix_factor);
    
    
    vec3 L = normalize(u_lightPosition - v_world_position);
    vec3 R = reflect(-L, N); // R is already normalized
    vec3 V = normalize(u_eye - v_world_position);
    
    // Use Kd with texture image instead of the Kd of the entity
    vec3 Kd = texture2D(u_colorTexture, v_uv).xyz;
    // Modify Ka and Ks
    vec3 Ka = u_Ka * texture2D(u_colorTexture, v_uv).xyz;
    vec3 Ks = vec3(texture2D(u_colorTexture, v_uv).a);
    
    vec3 Ip = (clamp(dot(L, N), 0.0, 1.0))*Kd*u_Id + u_Ks*(clamp(pow(dot(R,V), u_alfa),0.0, 1.0))*u_Is;
    
    if (u_addAmbient==1.0) {Ip += Ka*u_Ia;}
   
    gl_FragColor = vec4(Ip, 1.0);
    
}
