varying vec2 v_uv;

void main()
{
    // 3.1.a
    gl_FragColor = vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x));
    // 3.1.b
    gl_FragColor = distance(vec2(0.5, 0.5), v_uv.xy)*vec4(1,1,1,1);
    // 3.1.c
    gl_FragColor = step(0.5, v_uv.x)*vec4(1,0,0,1);
    
    // 3.1.f
    float tempX = v_uv.x;
    float tempY = v_uv.y;
    gl_FragColor = (mod(tempX, 0.1)+0.9)*vec4(1,0,0,1)+(mod(tempY, 0.1)+0.9)*vec4(0,1,0,1);
    gl_FragColor = (mod(tempX, 0.1)+0.9)*vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x))+(mod(tempY, 0.1)+0.9)*vec4(0,1,0,1);
}
