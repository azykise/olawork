
#program vert

attribute highp 	vec4 a_position;
attribute mediump 	vec2 a_uv0;

uniform highp mat4 u_mvp;

varying mediump vec2 v_uv;
varying highp 	vec2 v_depth;

void main() {	
	gl_Position = u_mvp * a_position;
	v_depth.xy = gl_Position.zw;
	v_uv = a_uv0;
}

#end






#program frag

uniform lowp vec4 u_color0;

uniform sampler2D s_diffuse;

varying highp 	vec2 v_depth;
varying mediump vec2 v_uv;

void main() {

	lowp vec4 tc = texture2D(s_diffuse,v_uv);		

	highp float f = v_depth.x/v_depth.y;
	
	if(tc.w < 0.2)
	{
		discard;
	}
	else
	{
		gl_FragColor = vec4(f,f,f,f);	
	}				 
}

#end