// They are baricentric interpolated by pixel according to the distance to every vertex
uniform mat4 u_model;
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia, u_Id, u_Is, u_lightPosition, u_Ka, u_Ks, u_Kd, u_eye, u_flags;
uniform float u_alfa, u_addAmbient;

// This variables comes from the vertex shader
varying vec3 v_world_position, v_world_normal;
varying vec2 v_uv;


void main()
{
    vec3 N, Kd, Ka, Ks;
    
    // Color texture flag
    if (u_flags.x == 0.0) {Kd = u_Kd; Ka = u_Ka;}
    else {Kd = texture2D(u_colorTexture, v_uv).xyz; Ka = u_Ka * texture2D(u_colorTexture, v_uv).xyz;}
    // Specular texture flag
    if (u_flags.y == 0.0) {Ks = u_Ks;}
    else {Ks = vec3(texture2D(u_colorTexture, v_uv).a);}
    // Normal texture flag
    if (u_flags.z == 0.0) {N = normalize(v_world_normal);}
    else{
        // Get normal vector from texture
        N = texture2D(u_normalTexture, v_uv).xyz;
        // Apply necessary transformations
        N = (u_model*vec4(N * 2.0 - vec3(1.0), 0.0)).xyz;
        // Apply mix factor (optional)
        /*
        float mix_factor = 0.3;
        N.xyz = mix(v_world_normal, N.xyz, mix_factor);
        */
    }
    
    vec3 L = normalize(u_lightPosition - v_world_position);
    vec3 R = reflect(-L, N); // R is already normalized
    vec3 V = normalize(u_eye - v_world_position);
    
    vec3 Ip = (clamp(dot(L, N), 0.0, 1.0))*Kd*u_Id + Ks*(clamp(pow(dot(R,V), u_alfa),0.0, 1.0))*u_Is;
    
    if (u_addAmbient==1.0) {Ip += Ka*u_Ia;}
    
    gl_FragColor = vec4(Ip, 1.0);
    
}
