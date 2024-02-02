#version 330 core

//Gets the data at Attrib Index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

//Declare a variable to hold the data
//that we're going to pass
//uniform float x;
//uniform float y;

//Creates a Transform variable
//We'll assign the transformation matrix here later
uniform mat4 transform;

//Projection Mat
uniform mat4 projection;

void main(){
	//Create a new vec3 for the new Position
	//					//Add x to aPos.x
	//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);

	//gl_Position is predefined
	//this denotes the final position of the vertex / point
	//gl_Position = vec4(newPos, 1.0); //Tuurns vex3 into a vec4

	//Multiply the transformation matrix to the
	//vec4
	gl_Position = projection * transform * vec4(aPos, 1.0); //Turns vex3 into a vec4
}