attribute highp 	vec4 a_position;

uniform highp mat4 u_mvp;

varying highp vec2 v_depth;

void main() {	
	gl_Position = u_mvp * a_position;
	v_depth.xy = gl_Position.zw;
}