#version 460

out vec4 out_color;

in vec2 uv0;
in vec3 position_normal;
in vec3 fragment_position;

layout(location = 3) uniform vec3 light_dir;
layout(location = 4) uniform vec3 ambient;
layout(location = 5) uniform float Ks;
layout(location = 6) uniform float Kd;
layout(location = 7) uniform float shininess;
layout(location = 8) uniform vec3 light_color;
layout(location = 9) uniform vec3 view_position; 

layout(binding = 0) uniform sampler2D mytexture;

void main()
{
    vec3 n_light_dir = normalize(light_dir);
    vec3 n_position_normal = normalize(position_normal);
    float NdotL = dot(n_position_normal, -n_light_dir);

    vec4 diffuse_color = texture2D(mytexture, uv0);

    if (NdotL > 0.0f)
	{
		vec3 diffuse = diffuse_color.xyz * light_color * NdotL * Kd;
		vec3 R = reflect(-n_light_dir, n_position_normal);
		vec3 V = normalize(view_position - fragment_position);
		
		float RdotV = max(dot(R, V), 0.0f);
		vec3 specular = light_color * pow(RdotV, shininess) * Ks;
		
		vec3 color = ambient * diffuse_color.xyz + diffuse + specular;
	}
	else
	{
		vec3 color = diffuse_color * vec4(ambient, 1.0f);
	}

    out_color = vec4(color, 1.0f);
}
