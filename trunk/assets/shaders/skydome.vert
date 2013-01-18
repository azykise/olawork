attribute highp 	vec4 a_position;
attribute highp 	vec3 a_normal;
attribute mediump 	vec2 a_uv0;

uniform highp mat4 u_mvp;
uniform highp mat4 u_model_invtrans;

varying mediump vec2 v_uv;
varying lowp vec3  v_diffuselight;

void main() {	
	
	highp vec3 normal = vec3(normalize(u_model_invtrans * vec4(a_normal,0.0)));					
	
//--vertex_light--
	lowp float ndotl = max(dot(normal,-1.0*vec3(0.0,0.0,1.0)),1.0);
	v_diffuselight = ndotl * vec3(1.0,1.0,1.0);	
//--vertex_light--

	v_uv = a_uv0;
	
	gl_Position = u_mvp * a_position;
}

