
#program vert

attribute highp 	vec4 a_position;
attribute mediump 	vec2 a_uv0;

uniform highp mat4 u_mvp;

varying mediump vec2 v_uv;

void main() {	
	v_uv = a_uv0;	
	gl_Position = u_mvp * a_position;
}


#end



#program frag

uniform sampler2D s_diffuse; 

varying mediump vec2 v_uv;

void main() {	
	gl_FragColor = texture2D(s_diffuse,v_uv);			 
}


#end