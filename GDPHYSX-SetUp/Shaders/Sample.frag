#version 330 core


out vec4 FragColor; //Returns a Color
//Simple shader that colors the model Red

void main(){
	
	//				 r   g	  b   a		Ranges from 0->1
	FragColor = vec4(0.7f,0.f,0.f, 1.f); //Sets the color of the Fragment

	//Apply it to the texture
	//Assign the texture color using the function
	//Divide the texture color by the attenuation factor to get the point light
	//FragColor = vec4((diffuse + ambientCol + specColor) / attenuation,1.0) * texture(tex0, texCoord);
}