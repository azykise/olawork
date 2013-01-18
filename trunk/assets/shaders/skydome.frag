uniform sampler2D s_diffuse; 

varying highp 	vec3 v_position;
varying mediump vec2 v_uv;

varying lowp vec3  v_diffuselight;

void main() {
	
	lowp vec4 tc = texture2D(s_diffuse,v_uv);
	
//--vertex_light--	
	gl_FragColor = vec4(tc.x * v_diffuselight.x,tc.y * v_diffuselight.y,tc.z * v_diffuselight.z,1.0);
//--vertex_light--			 
}
