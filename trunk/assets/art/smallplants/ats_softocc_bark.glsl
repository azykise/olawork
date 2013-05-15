
#program vert

attribute highp 	vec4 a_position;
attribute highp 	vec3 a_normal;
attribute mediump 	vec2 a_uv0;
attribute highp 	vec3 a_tangent;
attribute highp 	vec3 a_binormal;

uniform highp mat4 u_mvp;
uniform highp mat4 u_model_invtrans;
uniform highp mat4 u_model;

uniform highp vec3 u_eyepos;
uniform highp vec3 u_eyedir;

uniform highp vec3 u_lightpos0;
uniform highp vec3 u_lightdir0;
uniform highp vec3 u_lightcol0;

varying mediump vec2 v_uv;

varying lowp vec3  v_lightdir;
varying lowp vec3  v_viewdir;

void main() {	
	
	highp vec3 ws_normal = vec3(normalize(u_model_invtrans * vec4(a_normal,0.0)));
	highp vec3 ws_tangent = vec3(normalize(u_model_invtrans * vec4(a_tangent,0.0)));
	highp vec3 ws_binormal = vec3(normalize(u_model_invtrans * vec4(a_binormal,0.0)));	
					
	highp vec3 ws_position = (u_model * a_position).xyz;
	
	highp mat3 TBN = mat3(ws_tangent, ws_binormal, ws_normal);				 
	
	v_uv = a_uv0;
	
	v_lightdir = normalize(u_lightpos0 - ws_position) * TBN;
	v_viewdir = normalize(u_eyepos - ws_position) * TBN;	
	
	gl_Position = u_mvp * a_position;
}

#end



#program frag

uniform sampler2D s_diffuse; 
uniform sampler2D s_normal;
uniform sampler2D s_specular;

varying mediump vec2 v_uv;

varying lowp vec3  v_lightdir;
varying lowp vec3  v_viewdir;

void main() {
		
	lowp vec4 tc = texture2D(s_diffuse,v_uv);
	lowp vec4 nc = texture2D(s_normal,v_uv);
	
	mediump vec3 normal = nc.rgb * 2.0 - 1.0;
	normal.y *= -1.0;
	
	lowp float dnl = max(dot(normal,v_lightdir),0.0);
	
	lowp vec3 vh = normalize( (v_lightdir + v_viewdir) * 0.5 );
	lowp float sp = pow(max(dot(normal,vh),0.0), 2.3) * 1.0;
	
	lowp vec3 fc = tc.rgb * dnl + vec3(1,1,1) * sp;	

	gl_FragColor = vec4(fc.rgb,tc.a);
}

#end