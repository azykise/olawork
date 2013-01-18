uniform sampler2D s_diffuse; 

varying mediump vec2 v_uv;
varying lowp vec3  v_vertexlight;

void main() {
	
	lowp vec4 tc = texture2D(s_diffuse,v_uv);
	
//--vertex_light--	
	lowp vec4 out_c = vec4(tc.x * v_vertexlight.x,tc.y * v_vertexlight.y,tc.z * v_vertexlight.z,1.0);
//--vertex_light--
			 
	if(tc.w < 0.2)
	{
		discard;
	}
	else
	{
		gl_FragColor = out_c;
	}
	
}
