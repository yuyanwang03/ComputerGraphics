// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia, u_Id, u_Is, u_lightPosition, u_Ka, u_Kd, u_Ks;


varying vec3 v_world_normal;
varying vec3 v_world_position;
varying vec2 v_uv;
varying vec3 u_Ip;


void main()
{    
    // vec4 texture = texture2D(u_texture, v_uv);
    gl_FragColor = vec4(u_Ip, 1.0);
}
