varying vec2 v_uv;
varying vec3 worldPos;
varying vec3 localPos;

// 3.1.f
vec3 board(vec2 uv, float rows, float cols){
    float t = (floor(v_uv.x*cols)+floor(v_uv.y*rows));
    return vec3(mod(t,2.0)<1.0 ?1.0:0.0);
}
void main()
{
    // 3.1.a
    gl_FragColor = vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x));
    // 3.1.b
    gl_FragColor = distance(vec2(0.5, 0.5), v_uv.xy)*vec4(1,1,1,1);
    // 3.1.f
    gl_FragColor = vec4(board(v_uv, 20.0, 20.0),1);
    
    // float tempX = v_uv.x;
    // gl_FragColor = (mod(tempX, 0.2))*vec4(1,0,0,1);
    /*
    
    float tempX = v_uv.x;
    float tempY = v_uv.y;
    gl_FragColor = (mod(tempX, 0.1)+0.9)*vec4(1,0,0,1)+(mod(tempY, 0.1)+0.9)*vec4(0,1,0,1);
    gl_FragColor = (mod(tempX, 0.1)+0.9)*vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x))+(mod(tempY, 0.1)+0.9)*vec4(0,1,0,1);*/
}
