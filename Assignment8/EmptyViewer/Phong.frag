#version 330 core

// Vertex에서 넘어오는 값들
in vec3 wPosition;
in vec3 wNormal;
in vec3 wColor;

// 최종 프레임버퍼에 출력할 색
out vec4 FragColor;

uniform vec3 u_lightPos;   // 광원 위치
uniform vec3 u_eyePos;     // 카메라(eye) 위치
uniform vec3 u_lightColor; // 광원 색상
uniform vec3 u_ka;         // Ambient
uniform vec3 u_kd;         // Diffuse
uniform vec3 u_ks;         // Specular
uniform float u_p;         // Blinn-Phong 지수 (shininess)
uniform float u_Ia;        // Ambient
uniform float u_gamma;     

void main()
{
    // 1) 정규화된 법선 벡터
    vec3 N = normalize(wNormal);

    // 2) 광원 방향과 시선 방향 (View Space 기준)
    vec3 L = normalize(u_lightPos - wPosition);
    vec3 V = normalize(u_eyePos   - wPosition);

    // 3) Blinn-Phong: 하프웨이 벡터 계산
    vec3 H = normalize(L + V);

    // 4) Ambient 항
    vec3 ambient = u_Ia * u_ka;

    // 5) Diffuse 항 (Lambert)
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = u_kd * NdotL;

    // 6) Specular 항 (Blinn-Phong)
    float NdotH = max(dot(N, H), 0.0);
    vec3 specular = u_ks * pow(NdotH, u_p);

    // 7) 합산 + 버텍스 컬러 곱하기
    vec3 colorLinear = (ambient + diffuse + specular) * wColor * u_lightColor;

    // 8) 감마 보정
    vec3 colorGamma = pow(colorLinear, vec3(1.0 / u_gamma));

    FragColor = vec4(colorGamma, 1.0);
}