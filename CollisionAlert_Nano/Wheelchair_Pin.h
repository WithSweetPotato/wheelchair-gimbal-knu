#ifndef _WHEELCHAIR_PIN_H_
#define _WHEELCHAIR_PIN_H_

/// <summary>
/// 충돌 경고 위한 핀 정의
/// </summary>
namespace collision_alert_pin
{
	const int HCSR04_BACKWARD_TRIG_OUTPUT = 5; //후방 장애물 감지 초음파 센서 트리거 핀 (PWM)
	const int HCSR04_FORWARD_TRIG_OUTPUT = 3; //전방 장애물 감지 초음파 센서 트리거 핀 (PWM)
	const int HCSR04_BACKWARD_ECHO_INPUT = 11; //후방 장애물 감지 초음파 센서 에코 핀
	const int HCSR04_FORWARD_ECHO_INPUT = 10; //전방 장애물 감지 초음파 센서 에코 핀

	const int GL5537_ANALOG_INPUT = A0; //조도 센서 (ANALOG)
	const int PIEZO_OUTPUT = 6; //충돌 경고 피에조 부저 (PWM)
	const int LED_OUTPUT = 9; //충돌 경고 LED (PWM)
}
#endif