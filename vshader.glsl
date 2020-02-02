#version 150

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;


uniform vec3 theta;
uniform vec3 translateOriginValue;
uniform vec3 translateMousePosValue;



void main() 
{
	
	// Compute the sines and cosines of theta for each of
    //   the three axes in one computation.
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    // Remember: these matrices are column-major
    mat4 rX = mat4( 1.0,  0.0,  0.0, 0.0,
		    0.0,  c.x,  s.x, 0.0,
		    0.0, -s.x,  c.x, 0.0,
		    0.0,  0.0,  0.0, 1.0 );

    mat4 rY = mat4( c.y, 0.0, -s.y, 0.0,
		    0.0, 1.0,  0.0, 0.0,
		    s.y, 0.0,  c.y, 0.0,
		    0.0, 0.0,  0.0, 1.0 );
			
	mat4 rZ = mat4( c.z, s.z, 0.0, 0.0,
		-s.z,  c.z, 0.0, 0.0,
		0.0,  0.0, 1.0, 0.0,
		0.0,  0.0, 0.0, 1.0 );
		
	mat4 tXYZ = mat4(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		translateOriginValue.x, translateOriginValue.y, translateOriginValue.z, 1.0);

			mat4 tXYZ2 = mat4(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		translateMousePosValue.x, translateMousePosValue.y, translateMousePosValue.z, 1.0);

	
    color = vColor;
    gl_Position = tXYZ2 * rZ * rY * rX * tXYZ * vPosition;
} 
