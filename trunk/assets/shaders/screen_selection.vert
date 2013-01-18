attribute highp 	vec4 a_position;

uniform highp mat4 u_mvp;

uniform highp vec4 u_selectionid;

varying highp vec4  v_selectionid;

void main() {			
	v_selectionid = u_selectionid;
	gl_Position = u_mvp * a_position;
}