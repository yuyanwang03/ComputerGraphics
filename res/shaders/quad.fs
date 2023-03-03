varying vec2 v_uv;

// 3.1.c
// 色值大小计算
float v(in vec2 uv, float d, float o){
     return 1.0-smoothstep(0.0,1.,abs(sin((uv.x-0.1) * 15.)))-0.1;
}
float w(in vec2 uv, float d, float o){
     return 1.0-smoothstep(0.0,1.,abs(sin((uv.y-0.1) * 15.)));
}

// 3.1.f
vec3 board(vec2 uv, float rows, float cols){
    float t = (floor(v_uv.x*cols)+floor(v_uv.y*rows));
    return vec3(mod(t,2.0)<1.0);
}

void main()
{
    // 3.1.a
    gl_FragColor = vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x));
    // 3.1.b
    gl_FragColor = distance(vec2(0.5, 0.5), v_uv.xy)*vec4(1,1,1,1);
    // 3.1.c
    float d1 =  v_uv.x;
    float d2 = v_uv.y;
    gl_FragColor = vec4(v(v_uv, d1, 1.0), 0.0, 0.0, 0.1) + vec4(0.0, 0.0, w(v_uv, d2, 1.0), 0.1); // red + blue
    // 3.1.f
    gl_FragColor = vec4(board(v_uv, 20.0, 20.0),1);
    
    
    
    // gl_FragColor = vec4((0.3-distance(vec2(v_uv.x, 0.0), vec2(0.5, 0.0)))*1.0, 0, 0, 1);
    // gl_FragColor = vec4(smoothstep(0.1, 0.2, v_uv.x)*1.0, 0, 0, 1);
    
    // gl_FragColor = (mod(tempX, 0.2))*vec4(1,0,0,1);
    /*
    
    float tempX = v_uv.x;
    float tempY = v_uv.y;
    gl_FragColor = (mod(tempX, 0.1)+0.9)*vec4(1,0,0,1)+(mod(tempY, 0.1)+0.9)*vec4(0,1,0,1);
    gl_FragColor = (mod(tempX, 0.1)+0.9)*vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x))+(mod(tempY, 0.1)+0.9)*vec4(0,1,0,1);*/
}
