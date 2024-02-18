#ifndef _WHEELCHAIR_PIN_H_
#define _WHEELCHAIR_PIN_H_

/// <summary>
/// 적외선 통신 핀 정의
/// </summary>
namespace ir_remote_pin
{
	const int IR_RECV_INPUT = A0; //적외선 통신 수신 입력 (ANALOG)
}

/// <summary>
/// 바퀴에 대한 핀 정의
/// </summary>
namespace wheel_pin
{
	const int RIGHT_EN_OUTPUT = 5; //오른쪽 바퀴 속도 조절을 위한 1,2 활성화 핀 (PWM)
	const int LEFT_EN_OUTPUT = 6; //왼쪽 바퀴 속도 조절을 위한 3,4 활성화 핀 (PWM)

	const int LEFT_OUPUT1 = 12; //왼쪽 바퀴 출력 핀 1
	const int LEFT_OUTPUT2 = 13; //왼쪽 바퀴 출력 핀 2

	const int RIGHT_OUTPUT1 = 8; //오른쪽 바퀴 출력 핀 1
	const int RIGHT_OUTPUT2 = 7; //오른쪽 바퀴 출력 핀 2
}
#endif