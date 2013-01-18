attribute highp 	vec4 a_position;
attribute highp 	vec3 a_normal;
attribute mediump 	vec2 a_uv0;

uniform highp mat4 u_mvp;
uniform highp mat4 u_mv;
uniform highp mat4 u_model_invtrans;
uniform highp mat4 u_model;

uniform highp mat4 u_texture_proj;

uniform highp vec3 u_eyepos;
uniform highp vec3 u_eyedir;

uniform highp vec3 u_lightpos0;
uniform highp vec3 u_lightdir0;

varying highp vec4 v_uv_proj;
varying highp vec2 v_depth;

varying mediump vec2 v_uv;
varying lowp vec3  v_vertexlight;

void main() {	
	
	highp vec3 normal = vec3(normalize(u_model_invtrans * vec4(a_normal,0.0)));					
	
//--vertex_light--
	lowp float ndotl = max(dot(normal,-1.0*u_lightdir0),0.0);
	lowp vec3 lambient = 0.4 * vec3(1.0,1.0,1.0);
	lowp vec3 ldiffuse = ndotl * vec3(1.0,1.0,1.0);	
	v_vertexlight = min((lambient + ldiffuse),1.0);
//--vertex_light--

	v_uv_proj = u_texture_proj * u_model * a_position;

	v_uv = a_uv0;
	
	gl_Position = u_mvp * a_position;
	
	v_depth.xy = gl_Position.zw;
}