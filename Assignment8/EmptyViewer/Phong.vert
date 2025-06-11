#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 wPosition;  // 월드 공간상의 위치
out vec3 wNormal;    // 월드 공간상의 법선
out vec3 wColor;     // 입력으로 받은 색상

uniform mat4 model;  // 모델 행렬 (모델→월드)
uniform mat4 view;   // 뷰 행렬   (월드→카메라)
uniform mat4 proj;   // 투영 행렬 (카메라→클립)

void main()
{
    // (1) 모델→뷰 변환
    mat4 modelView = view * model;
    vec4 viewPos = modelView * vec4(aPos, 1.0);
    wPosition = viewPos.xyz;

    // (2) 모델→뷰에서의 법선 계산
    mat3 normalMatrix = mat3(transpose(inverse(modelView)));
    wNormal = normalize(normalMatrix * aNormal);

    // (3) 버텍스 컬러 그대로 넘김
    wColor = aColor;

    // (4) 최종 클립 좌표
    gl_Position = proj * viewPos;
}