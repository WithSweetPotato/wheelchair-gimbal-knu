#ifndef _COLLISION_ALERT_HPP_
#define _COLLISION_ALERT_HPP_

#include "../Wheelchair_Core.h"

#define MIN_HCSR04_CM_RANGE 2 //초음파 거리 센서 최소 측정 가능 범위 (cm 단위)
#define MAX_HCSR04_CM_RANGE 400 //초음파 거리 센서 최대 측정 가능 범위 (cm 단위)
static int MIN_GL5537_THRESHOLD = 60; //최소 조도 센서 측정 임계 값
static int MAX_GL5537_THRESHOLD = 970; //최대 조도 센서 측정 임계 값
static unsigned long COLLISION_ALERT_CM_THRESHOLD = (MIN_HCSR04_CM_RANGE + 20); //충돌 경고 임계 값 (cm 단위)

/// <summary>
/// 장애물과의 cm 단위 거리
/// </summary>
struct DISTANCE_CM
{
	unsigned long _backwardCm; //후방 장애물과의 cm 단위 거리
	unsigned long _forwardCm; //전방 장애물과의 cm 단위 거리
};

/// <summary>
/// 충돌 경고
/// </summary>
class COLLISION_ALERT
{
public:
	/// <summary>
	/// COLLISION_ALERT 생성자
	/// </summary>
	COLLISION_ALERT()
	{
	}

	/// <summary>
	/// COLLISION_ALERT 소멸자
	/// </summary>
	~COLLISION_ALERT()
	{
	}

	/// <summary>
	/// 초기화 작업 수행
	/// </summary>
	void Init() const
	{
		pinMode(collision_alert_pin::HCSR04_BACKWARD_ECHO_INPUT, INPUT);
		pinMode(collision_alert_pin::HCSR04_FORWARD_ECHO_INPUT, INPUT);
	}

	/// <summary>
	/// 작업 수행
	/// </summary>
	void RunTask() const
	{
		COMMON_SHIFT_REG_PWM& commonShiftRegPwmInstance = COMMON_SHIFT_REG_PWM::GetInstance();

		uint8_t brightness = this->GetBrightnessFromGl5537(); //주변 밝기
		DISTANCE_CM distanceCm = this->GetDistanceCmFromHcsr04(); //전방, 후방 장애물과의 cm단위 거리

		VAR_DUMP(brightness);
		VAR_DUMP(distanceCm._backwardCm);
		VAR_DUMP(distanceCm._forwardCm);

		if (distanceCm._backwardCm <= COLLISION_ALERT_CM_THRESHOLD ||
			distanceCm._forwardCm <= COLLISION_ALERT_CM_THRESHOLD) //전방 혹은 후방 장애물과의 cm단위 거리가 임계 값 이하일 경우
		{
			commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::LED_OUTPUT,
				map(brightness,
					MIN_PWM_VALUE, MAX_PWM_VALUE,
					MAX_PWM_VALUE, MIN_PWM_VALUE)); //주변이 밝을 경우 최소 밝기 출력, 어두울 경우 최대 밝기 출력
			commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::PIEZO_OUTPUT, MAX_PWM_VALUE);
		}
		else //인접 한 장애물이 존재하지 않을 경우
		{
			commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::LED_OUTPUT, MIN_PWM_VALUE);
			commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::PIEZO_OUTPUT, MIN_PWM_VALUE);
		}
	}

private:
	/// <summary>
	/// GL5537 조도 센서 (CdS Photoresistor)의 측정 값으로부터 변환 된 펄스 폭 변조 (PWM) 값 범위 내의 밝기 반환
	/// </summary>
	/// <returns>펄스 폭 변조 (PWM) 값 범위 내의 밝기</returns>
	uint8_t GetBrightnessFromGl5537() const
	{
		int retVal = analogRead(collision_alert_pin::Gl5537_ANALOG_INPUT); //조도 센서 측정 값

		retVal = map(retVal,
			MIN_GL5537_THRESHOLD, MAX_GL5537_THRESHOLD,
			MIN_PWM_VALUE, MAX_PWM_VALUE); //임계 값 범위 내의 측정 값을 펄스 폭 변조 (PWM) 값 범위 내의 값으로 선형 사상

		return constrain(retVal,
			MIN_PWM_VALUE, MAX_PWM_VALUE);
	}

	/// <summary>
	/// HC-SR04 초음파 센서로부터 물체와의 cm 단위 거리 반환
	/// </summary>
	/// <returns>물체와의 cm 단위 거리</returns>
	DISTANCE_CM GetDistanceCmFromHcsr04() const
	{
		COMMON_SHIFT_REG_PWM& commonShiftRegPwmInstance = COMMON_SHIFT_REG_PWM::GetInstance();

		// https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
		// https://www.arduino.cc/reference/ko/language/functions/advanced-io/pulsein/
		//10마이크로초 이상 길이 (HIGH에 대한)의 펄스를 트리거 핀에 주어 작동 시작

		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::HCSR04_BACKWARD_TRIG_OUTPUT, LOW);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::HCSR04_FORWARD_TRIG_OUTPUT, LOW);
		TIME_SENSITIVE_JOB_HANDLER::HandleNonBlockingInterruptDelayMs(5);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::HCSR04_BACKWARD_TRIG_OUTPUT, HIGH);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::HCSR04_FORWARD_TRIG_OUTPUT, HIGH);
		TIME_SENSITIVE_JOB_HANDLER::HandleNonBlockingInterruptDelayMs(10);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::HCSR04_BACKWARD_TRIG_OUTPUT, LOW);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_collision_alert_pin::HCSR04_FORWARD_TRIG_OUTPUT, LOW);

		DISTANCE_CM retVal = { 0, };

		//HIGH 신호가 ECHO에 수신 될 때까지 측정 된 마이크로초 단위 시간
		retVal._backwardCm =
			pulseIn(collision_alert_pin::HCSR04_BACKWARD_ECHO_INPUT, HIGH);
		retVal._forwardCm =
			pulseIn(collision_alert_pin::HCSR04_FORWARD_ECHO_INPUT, HIGH);

		/***
			- 20도의 공기 중 음속 : 343m/s (0.034cm/µs)
			- 거리 = 시간 * 속도
			- 물체와의 거리 (초음파가 물체에 닿아 반사되어 수신 되는데 걸린 시간 측정) = (시간 / 2) * 속도

			---

			물체와의 거리 = (시간 * 0.034) / 2
		***/

		//물체와의 cm 단위 거리 계산
		retVal._backwardCm = (retVal._backwardCm * 0.034) / 2;
		retVal._forwardCm = (retVal._forwardCm * 0.034) / 2;

		return retVal;
	}
};
#endif