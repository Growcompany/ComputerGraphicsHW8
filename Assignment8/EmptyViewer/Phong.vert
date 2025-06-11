#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 wPosition;  // ���� �������� ��ġ
out vec3 wNormal;    // ���� �������� ����
out vec3 wColor;     // �Է����� ���� ����

uniform mat4 model;  // �� ��� (�𵨡����)
uniform mat4 view;   // �� ���   (�����ī�޶�)
uniform mat4 proj;   // ���� ��� (ī�޶��Ŭ��)

void main()
{
    // (1) �𵨡�� ��ȯ
    mat4 modelView = view * model;
    vec4 viewPos = modelView * vec4(aPos, 1.0);
    wPosition = viewPos.xyz;

    // (2) �𵨡�信���� ���� ���
    mat3 normalMatrix = mat3(transpose(inverse(modelView)));
    wNormal = normalize(normalMatrix * aNormal);

    // (3) ���ؽ� �÷� �״�� �ѱ�
    wColor = aColor;

    // (4) ���� Ŭ�� ��ǥ
    gl_Position = proj * viewPos;
}