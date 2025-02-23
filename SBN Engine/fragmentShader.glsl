#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;//sil istersen ��nk� ���kta pozisyon �nemli de�il! Directional Light'ta
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform samplerCube skybox;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    //vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;

	// �i�ek a�a�lar�n alphas�n� yok ediyor b�ylece s�per oluyor.
	vec4 textureColour = texture(material.diffuse, TexCoords);
	if(textureColour.a < 0.1) {
		discard;
	}

	//vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));


	FragColor = vec4(result, 1.0); //* vec4(texture(skybox, R).rgb, 1.0);

}