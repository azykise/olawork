uniform sampler2D s_diffuse; 
uniform sampler2D s_shadowmap;

varying highp vec4 v_uv_proj;
varying highp vec2 v_depth;

varying mediump vec2 v_uv;
varying lowp vec3  v_vertexlight;


void main() {

	highp float texlx = 0.5 * (v_uv_proj.x / v_uv_proj.w) + 0.5;
	highp float texly = 0.5 * (v_uv_proj.y / v_uv_proj.w) + 0.5;
	
	highp vec2 s_uv = vec2(texlx ,texly);	

	highp float comp = v_depth.x / v_depth.y;
	highp float depth = texture2D(s_shadowmap,s_uv).r;
	
	
	lowp float val = comp <= depth ? 0.4 : 1.0;
	
	lowp vec4 tc = texture2D(s_diffuse,v_uv);	
	
//--vertex_light--	
	lowp vec4 out_c = vec4(tc.x * v_vertexlight.x,tc.y * v_vertexlight.y,tc.z * v_vertexlight.z,1.0);
//--vertex_light--
			 
	gl_FragColor = out_c * val;	
	
}