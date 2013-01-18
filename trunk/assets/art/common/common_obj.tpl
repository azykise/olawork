<OlaShaderRoot>
	<head name="common_obj" fxtype="glsl"/>
	<desc>normalmap;phong</desc>
	<states>
		<AlpahTest value="0"/>
		<Cull value="back"/>
	</states>
	<glsl>
		<attribute contains="pos;nor;uv0;tan;bnr"/>		
		<uniform type="mat4" name="u_mvp"/>
		<uniform type="mat4" name="u_model_invtrans"/>
		<uniform type="mat4" name="u_model"/>		
		<uniform type="vec3" name="u_eyepos"/>
		<uniform type="vec3" name="u_eyedir"/>	
		<uniform type="vec3" name="u_lightpos0"/>
		<uniform type="vec3" name="u_lightdir0"/>			
		<uniform type="sampler2D" name="s_diffuse"/>
		<uniform type="sampler2D" name="s_normal"/>
		<uniform type="sampler2D" name="s_specular"/>	
		<varying type="vec2" name="v_uv"/>	
		<varying type="vec3" name="v_lightdir"/>	
		<varying type="vec3" name="v_viewdir"/>		
		<vert>										
			highp vec3 ws_normal = vec3(normalize(u_model_invtrans * vec4(a_normal,0.0)));
			highp vec3 ws_tangent = vec3(normalize(u_model_invtrans * vec4(a_tangent,0.0)));
			highp vec3 ws_binormal = vec3(normalize(u_model_invtrans * vec4(a_binormal,0.0)));	
							
			highp vec3 ws_position = (u_model * a_position).xyz;
			
			highp mat3 TBN = mat3(ws_tangent, ws_binormal, ws_normal);				 
			
			v_uv = a_uv0;
			
			v_lightdir = normalize(u_lightpos0 - ws_position) * TBN;
			v_viewdir = normalize(u_eyepos - ws_position) * TBN;	
			
			gl_Position = u_mvp * a_position;	
		</vert>		
		<frag>							
			lowp vec4 tc = texture2D(s_diffuse,v_uv);
			lowp vec4 nc = texture2D(s_normal,v_uv);
			lowp vec4 sc = texture2D(s_specular,v_uv);
			
			mediump vec3 normal = nc.rgb * 2.0 - 1.0;
			normal.y *= -1.0;
			
			lowp float dnl = max(dot(normal,v_lightdir),0.0);
			
			lowp vec3 vh = normalize( (v_lightdir + v_viewdir) * 0.5 );
			lowp float sp = pow(max(dot(normal,vh),0.0), 2.3) * 1.0;
			
			lowp vec3 fc = tc.rgb * dnl + vec3(1,1,1) * sp + sc.rgb * 0.6;	
		
			gl_FragColor = vec4(fc.rgb,tc.a);		
		</frag>
	</glsl>
</OlaShaderRoot>