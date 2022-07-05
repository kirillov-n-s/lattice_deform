#version 330 core

in vec4 Position;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 uCameraDir;
uniform vec4 uColor;

void main()
{
    if (Normal == vec3(0.))
    {
        FragColor = uColor;
    }
    else
    {
        vec3 neglightdir = -normalize(vec3(0., 0., -1.));
        vec3 normal = normalize(Normal);

        vec3 k_ambient = vec3(0.2);
        vec3 k_diffuse = vec3(0.5);
        vec3 k_specular = vec3(0.7);
        float shininess = 32.;

        float dotdiffuse = max(dot(normal, neglightdir), 0.0);
        vec3 reflected = normalize(reflect(neglightdir, normal));
        float dotspecular = max(dot(uCameraDir, reflected), 0.0);
        float powspecular = pow(dotspecular, shininess);

        FragColor = vec4(k_ambient + dotdiffuse * k_diffuse + powspecular * k_specular, 1.);
    }
}
