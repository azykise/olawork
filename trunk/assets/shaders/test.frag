uniform sampler2D s_diffuse; 

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
	
	lowp vec4 tc = texture2D(s_diffuse,v_uv);
	lowp vec3 tc3 = vec3(tc);	
	lowp vec4 out_c = vec4(tc3,1.0);
	
//--vertex_light--	
	out_c = vec4(tc3 * v_diffuselight ,1.0); //+ v_specularlight
//--vertex_light--
	
//--pixel_light--
/*
	lowp float ndotl = max(dot(v_normal,-1.0*u_lightdir0),0.0);
	lowp vec3 dc = ndotl * vec3(1.0,1.0,1.0);	
	lowp vec3 lc = ( tc * dc) ;
	
	lowp vec3 ptl = normalize(u_lightpos0 - v_position);
	lowp vec3 pte = normalize(u_eyepos - v_position);
	
	lowp vec3 vh = normalize((ptl + pte) * 0.5 );
	lowp float delta = max(0.0,dot(v_normal,vh));
	lowp float factor = pow(delta,16.0);
	lowp vec3 sc = factor * vec3(1.0,1.0,1.0);
	out_c = vec4(lc + sc,1.0);
*/
//--pixel_light--

//--no_light_test--	
//	mediump float r = tc.x;
//	mediump float g = tc.y;	
//	mediump float b = out_c.x;
//	b = b == 0.0 ? 1.0 : b / out_c.x;	
//	out_c = vec4(r,g,b*tc.z,1.0);
//--no_light_test--		
	 
	if(tc.w < 0.2)
	{
		discard;
	}
	else
	{
		gl_FragColor = out_c;
	}
}
