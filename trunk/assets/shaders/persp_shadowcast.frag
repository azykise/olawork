
varying highp vec2 v_depth;

void main() {
	highp float f = v_depth.x/v_depth.y;
	gl_FragColor = vec4(f,f,f,f);		 
}