
#program vert

attribute highp 	vec4 a_position;

uniform highp mat4 u_mvp;
uniform lowp vec4 u_color0;

void main() {	
	
	gl_Position = u_mvp * a_position;
}

#end



#program frag

uniform lowp vec4 u_color0;

void main() {
	
	gl_FragColor = u_color0;
		 
}

#end