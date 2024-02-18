#ifndef _WHEELCHAIR_PIN_H_
#define _WHEELCHAIR_PIN_H_

/// <summary>
/// 쉬프트 레지스터 핀 정의
/// </summary>
namespace shift_reg_pin
{
	/***
		ShiftRegister-PWM-Library는 기본값으로 데이터, LATCH, 클록에 대해
		순차적으로 2, 4, 3번 핀 사용
	***/

	const int HC595_DATA_OUTPUT = 2; //쉬프트 레지스터 데이터 핀
	const int HC595_RCLK_OUTPUT = 4; //쉬프트 레지스터 출력 레지스터 클록 (LATCH)
	const int HC595_SRCLK_OUTPUT = 3; //쉬프트 레지스터 클록

	/// <summary>
	/// 쉬프트 레지스터의 출력에 대한 충돌 감지 알림 핀 정의
	/// </summary>
	namespace inner_collision_alert_pin
	{
		const uint8_t HCSR04_BACKWARD_TRIG_OUTPUT = 0; //후방 장애물 감지 초음파 센서 트리거 핀 (PWM)
		const uint8_t HCSR04_FORWARD_TRIG_OUTPUT = 1; //전방 장애물 감지 초음파 센서 트리거 핀 (PWM)

		const uint8_t PIEZO_OUTPUT = 4; //충돌 경고 피에조 부저 (PWM)
		const uint8_t LED_OUTPUT = 5; //충돌 경고 LED (PWM)
	}

	/// <summary>
	/// 쉬프트 레지스터의 출력에 대한 바퀴 핀 정의
	/// </summary>
	namespace inner_wheel_pin
	{
		const uint8_t H_BRIDGE_RIGHT_EN_OUTPUT = 2; //H-브릿지 오른쪽 바퀴 속도 조절을 위한 1,2 활성화 핀 (PWM)
		const uint8_t H_BRIDGE_LEFT_EN_OUTPUT = 3; //H-브릿지 왼쪽 바퀴 속도 조절을 위한 3,4 활성화 핀 (PWM)
	}
}

/// <summary>
/// 충돌 경고 위한 핀 정의
/// </summary>
namespace collision_alert_pin
{
	const int HCSR04_BACKWARD_ECHO_INPUT = 13; //후방 장애물 감지 초음파 센서 에코 핀
	const int HCSR04_FORWARD_ECHO_INPUT = 12; //전방 장애물 감지 초음파 센서 에코 핀

	const int Gl5537_ANALOG_INPUT = A0; //조도 센서 (ANALOG)
}

/// <summary>
/// 수평 유지 장치에 대한 핀 정의
/// </summary>
namespace gimbal_pin
{
	const int X_AXIS_SERVO_OUTPUT = 10; //x축에 대한 서보 모터 (PWM)
	const int Y_AXIS_SERVO_OUTPUT = 9; //y축에 대한 서보 모터 (PWM)

	//TODO : 기울기 센서 아날로그 입력 혹은 디지털 입력 테스트
	const int X_AXIS_GIMBAL_INPUT = A2; //x축에 대한 기울기 센서
	const int Y_AXIS_GIMBAL_INPUT = A3; //y축에 대한 기울기 센서
}

/// <summary>
/// 적외선 통신 핀 정의
/// </summary>
namespace ir_remote_pin
{
	const int IR_RECV_INPUT = A1; //적외선 통신 수신 입력 (ANALOG)
}

/// <summary>
/// 바퀴에 대한 핀 정의
/// </summary>
namespace wheel_pin
{
	const int H_BRIDGE_LEFT_INPUT1 = 6; //H-브릿지 왼쪽 바퀴 입력 핀 1
	const int H_BRIDGE_LEFT_INPUT2 = 5; //H-브릿지 왼쪽 바퀴 입력 핀 2

	const int H_BRIDGE_RIGHT_INPUT1 = 8; //H-브릿지 오른쪽 바퀴 입력 핀 1
	const int H_BRIDGE_RIGHT_INPUT2 = 7; //H-브릿지 오른쪽 바퀴 입력 핀 2
}
#endif