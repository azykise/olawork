attribute highp 	vec4 a_position;
attribute highp 	vec3 a_normal;
attribute mediump 	vec2 a_uv0;

uniform highp mat4 u_mvp;
uniform highp mat4 u_model_invtrans;
uniform highp mat4 u_model;

uniform highp vec3 u_eyepos;
uniform highp vec3 u_eyedir;

uniform highp vec3 u_lightpos0;
uniform highp vec3 u_lightdir0;
uniform highp vec3 u_lightcol0;

varying highp 	vec3 v_position;
varying highp 	vec3 v_normal;
varying mediump vec2 v_uv;

varying lowp vec3  v_diffuselight;
varying lowp vec3  v_specularlight;

void main() {	
	
	highp vec3 normal = vec3(normalize(u_model_invtrans * vec4(a_normal,0.0)));		
	highp vec3 vert_pos = vec3(u_model * a_position);			
	
//--vertex_light--

	v_diffuselight  = vec3(1.0,1.0,1.0);
	v_specularlight = vec3(1.0,1.0,1.0);	
/*	
	mediump vec3 ptl = normalize(u_lightpos0 - vert_pos);
	mediump vec3 pte = normalize(u_eyepos - vert_pos);
	
	mediump vec3 vh = normalize((ptl + pte) * 0.5 );
	mediump float delta = max(0.0,dot(normal,vh));
	mediump float factor = pow(delta,16.0);
	
	v_specularlight = factor * vec3(1.0,1.0,1.0);	
*/
	
	lowp float ndotl = max(dot(normal,-1.0*u_lightdir0),0.0);
	v_diffuselight = ndotl * vec3(1.0,1.0,1.0);	

//--vertex_light--

	v_position = vert_pos;	
	v_normal = normal;
	v_uv = a_uv0;
	
	gl_Position = u_mvp * a_position;
}

