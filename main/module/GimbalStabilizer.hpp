#ifndef _GIMBAL_STABILIZER_HPP_
#define _GIMBAL_STABILIZER_HPP_

#include "../Wheelchair_Core.h"
#include <Servo-avr-custom.h> // https://github.com/arduino-libraries/Servo

static uint8_t INIT_SERVO_X_ANGLE = 80;
static uint8_t INIT_SERVO_Y_ANGLE = 97;
static uint8_t INIT_SERVO_ANGLE = 90; //초기 서보 회전 각도

static uint16_t INIT_STS90H1_X_RAW_VALUE = 496; //2축 기울기 센서 (수평 고정형)의 초기 x축에 대한 원시 값 (수평 기준)
static uint16_t MIN_STS90H1_X_RAW_VALUE = 45; //2축 기울기 센서 (수평 고정형)의 최소 x축에 대한 원시 값
static uint16_t MAX_STS90H1_X_RAW_VALUE = 942; //2축 기울기 센서 (수평 고정형)의 최대 y축에 대한 원시 값

static uint16_t INIT_STS90H1_Y_RAW_VALUE = 488; //2축 기울기 센서 (수평 고정형)의 초기 y축에 대한 원시 값 (수평 기준)
static uint16_t MIN_STS90H1_Y_RAW_VALUE = 37; //2축 기울기 센서 (수평 고정형)의 최소 y축에 대한 원시 값
static uint16_t MAX_STS90H1_Y_RAW_VALUE = 943; //2축 기울기 센서 (수평 고정형)의 최대 y축에 대한 원시 값

/// <summary>
/// 수평 유지 장치
/// </summary>
class GIMBAL_STABILIZER
{
public:
	/// <summary>
	/// GIMBAL_STABILIZER 생성자
	/// </summary>
	GIMBAL_STABILIZER()
	{
	}

	/// <summary>
	/// GIMBAL_STABILIZER 소멸자
	/// </summary>
	~GIMBAL_STABILIZER()
	{
	}

	/// <summary>
	/// 초기화 작업 수행
	/// </summary>
	void Init()
	{
		if (this->_xAxisServo.attach(gimbal_pin::X_AXIS_SERVO_OUTPUT) == INVALID_SERVO)
			goto THROW_INVALID_SERVO_EX;

		if (this->_yAxisServo.attach(gimbal_pin::Y_AXIS_SERVO_OUTPUT) == INVALID_SERVO)
			goto THROW_INVALID_SERVO_EX;

		this->_xAxisServo.write(INIT_SERVO_X_ANGLE);
		this->_yAxisServo.write(INIT_SERVO_Y_ANGLE);
		return;

	THROW_INVALID_SERVO_EX:
		ERROR_LOG("Failed to attach Servo");
		while (1);
	}

	/// <summary>
	/// 작업 수행
	/// </summary>
	void RunTask()
	{
	}

private:
	/// <summary>
	/// x 및 y축에 대한 경사각 반환
	/// </summary>
	void GetDipAngleXY()
	{
		//TOOD : x및 y축에 대한 현재 경사각 반환, 센서 테스트 필요
	}

private:
	Servo _xAxisServo; //x축에 대한 서보 모터 (roll)
	Servo _yAxisServo; //y축에 대한 서보 모터 (pitch)
};
#endif