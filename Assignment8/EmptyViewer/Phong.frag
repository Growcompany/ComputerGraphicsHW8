#version 330 core

// Vertex���� �Ѿ���� ����
in vec3 wPosition;
in vec3 wNormal;
in vec3 wColor;

// ���� �����ӹ��ۿ� ����� ��
out vec4 FragColor;

uniform vec3 u_lightPos;   // ���� ��ġ
uniform vec3 u_eyePos;     // ī�޶�(eye) ��ġ
uniform vec3 u_lightColor; // ���� ����
uniform vec3 u_ka;         // Ambient
uniform vec3 u_kd;         // Diffuse
uniform vec3 u_ks;         // Specular
uniform float u_p;         // Blinn-Phong ���� (shininess)
uniform float u_Ia;        // Ambient
uniform float u_gamma;     

void main()
{
    // 1) ����ȭ�� ���� ����
    vec3 N = normalize(wNormal);

    // 2) ���� ����� �ü� ���� (View Space ����)
    vec3 L = normalize(u_lightPos - wPosition);
    vec3 V = normalize(u_eyePos   - wPosition);

    // 3) Blinn-Phong: �������� ���� ���
    vec3 H = normalize(L + V);

    // 4) Ambient ��
    vec3 ambient = u_Ia * u_ka;

    // 5) Diffuse �� (Lambert)
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = u_kd * NdotL;

    // 6) Specular �� (Blinn-Phong)
    float NdotH = max(dot(N, H), 0.0);
    vec3 specular = u_ks * pow(NdotH, u_p);

    // 7) �ջ� + ���ؽ� �÷� ���ϱ�
    vec3 colorLinear = (ambient + diffuse + specular) * wColor * u_lightColor;

    // 8) ���� ����
    vec3 colorGamma = pow(colorLinear, vec3(1.0 / u_gamma));

    FragColor = vec4(colorGamma, 1.0);
}