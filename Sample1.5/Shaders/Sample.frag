#version 330 core


out vec4 FragColor; //Returns a Color
//Simple shader that colors the model Red
void main(){
	//				 r   g	  b   a		Ranges from 0->1
	FragColor = vec4(0.7f,0.f,0.f, 1.f); //Sets the color of the Fragment
}