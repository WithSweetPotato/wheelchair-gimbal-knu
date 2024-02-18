#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include "../Wheelchair_Core.h"
#include <IRremote-custom.h> // https://github.com/Arduino-IRremote/Arduino-IRremote

static uint8_t MAX_WHEEL_ROTATE_SPEED = (MAX_PWM_VALUE / 3); //최대 바퀴 회전 속도

/// <summary>
/// 적외선 통신 Car MP3 SE-020401 리모콘 명령어<para>
/// https://gist.github.com/steakknife/e419241095f1272ee60f5174f7759867
/// </para></summary>
enum class IR_REMOTE_SE_020401_COMMAND : const uint16_t
{
	N0 = 22, //0
	N1 = 12, //1
	N2 = 24, //2
	N3 = 94, //3
	N4 = 8, //4
	N5 = 28, //5
	N6 = 90, //6
	N7 = 66, //7
	N8 = 82, //8
	N9 = 74, //9
	N100P = 25, //100+
	N200P = 13, //200+
	MINUS = 7, //-
	PLUS = 21, //+
	EQ = 9, //EQ
	PREV = 68, //|<<
	NEXT = 64, //>>|
	PLAY = 67, //>||
	CH_PREV = 69, //CH-
	CH = 70, //CH
	CH_NEXT = 71 //CH+
};

/// <summary>
/// 이동 방향
/// </summary>
enum class MOVE_DIRECTION : const int
{
	DO_NOTHING = 0,
	FORWARD, //전진
	BACKWARD, //후진
	LEFT, //제자리 좌회전
	RIGHT, //제자리 우회전
	LEFT_AND_FORWARD, //좌회전 및 전진
	RIGHT_AND_FORWARD, //우회전 및 전진
	BRAKE //정지
};

/// <summary>
/// 차체 이동
/// </summary>
class MOVE
{
public:
	/// <summary>
	/// MOVE 생성자
	/// </summary>
	MOVE()
	{
	}

	/// <summary>
	/// MOVE 소멸자
	/// </summary>
	~MOVE()
	{
	}

	/// <summary>
	/// 초기화 작업 수행
	/// </summary>
	void Init()
	{
		pinMode(wheel_pin::H_BRIDGE_LEFT_INPUT1, INPUT);
		pinMode(wheel_pin::H_BRIDGE_LEFT_INPUT2, INPUT);
		pinMode(wheel_pin::H_BRIDGE_RIGHT_INPUT1, INPUT);
		pinMode(wheel_pin::H_BRIDGE_RIGHT_INPUT2, INPUT);

		this->_irrecv.setReceivePin(ir_remote_pin::IR_RECV_INPUT);
		this->_irrecv.enableIRIn(); //적외선 신호 수신을 위한 타이머 및 State Machine 구성
	}

	/// <summary>
	/// 작업 수행
	/// </summary>
	void RunTask()
	{
		this->MoveTo(this->GetMoveDirectionFromIrRemote());
	}

private:
	/// <summary>
	/// 적외선 통신으로부터 이동 방향 반환
	/// </summary>
	/// <returns>이동 방향</returns>
	MOVE_DIRECTION GetMoveDirectionFromIrRemote()
	{
		MOVE_DIRECTION retVal = MOVE_DIRECTION::DO_NOTHING;
		Serial.println("remote 1");

		if (this->_irrecv.decode()) //적외선 신호로부터 복호화 된 데이터가 존재하면
		{
			VAR_DUMP(IrReceiver.decodedIRData.command);

			switch (IrReceiver.decodedIRData.command) //복호화 된 명령어에 따라
			{
			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N2): //전진
				retVal = MOVE_DIRECTION::FORWARD;
				break;

			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N8): //후진
				retVal = MOVE_DIRECTION::BACKWARD;
				break;

			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N4): //제자리 좌회전
				retVal = MOVE_DIRECTION::LEFT;
				break;

			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N6): //제자리 우회전
				retVal = MOVE_DIRECTION::RIGHT;
				break;

			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N1): //좌회전 및 전진
				retVal = MOVE_DIRECTION::LEFT_AND_FORWARD;
				break;

			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N3): //우회전 및 전진
				retVal = MOVE_DIRECTION::RIGHT_AND_FORWARD;
				break;

			case static_cast<uint16_t>(IR_REMOTE_SE_020401_COMMAND::N5): //정지
				retVal = MOVE_DIRECTION::BRAKE;
				break;

			default:
				//do nothing
				break;
			}

			IrReceiver.resume();
		}
		Serial.println("remote 2");
		return retVal;
	}

	/// <summary>
	/// 이동 방향에 따라 차체 이동
	/// </summary>
	/// <param name="moveDirection">이동 방향</param>
	void MoveTo(const MOVE_DIRECTION& moveDirection)
	{
		if (moveDirection == MOVE_DIRECTION::DO_NOTHING)
			return;

		COMMON_SHIFT_REG_PWM& commonShiftRegPwmInstance = COMMON_SHIFT_REG_PWM::GetInstance();
		Serial.println("move 1");
		//스위치의 상태 변경 전 비활성화 (단락 방지)
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_wheel_pin::H_BRIDGE_LEFT_EN_OUTPUT, LOW);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_wheel_pin::H_BRIDGE_RIGHT_EN_OUTPUT, LOW);

		uint8_t leftWheelRotateSpeed = 0; //왼쪽 바퀴 회전 속도
		uint8_t rightWheelRotateSpeed = 0; //오른쪽 바퀴 회전 속도

		switch (moveDirection) //이동 방향에 따라 각 바퀴의 이동 속도 할당
		{
		case MOVE_DIRECTION::FORWARD:
		case MOVE_DIRECTION::BACKWARD:
			leftWheelRotateSpeed = rightWheelRotateSpeed = MAX_WHEEL_ROTATE_SPEED;
			break;

		case MOVE_DIRECTION::LEFT:
			rightWheelRotateSpeed = MAX_WHEEL_ROTATE_SPEED;
			break;

		case MOVE_DIRECTION::RIGHT:
			leftWheelRotateSpeed = MAX_WHEEL_ROTATE_SPEED;
			break;

		case MOVE_DIRECTION::LEFT_AND_FORWARD:
			leftWheelRotateSpeed = (MAX_WHEEL_ROTATE_SPEED / 2);
			rightWheelRotateSpeed = MAX_WHEEL_ROTATE_SPEED;
			break;

		case MOVE_DIRECTION::RIGHT_AND_FORWARD:
			leftWheelRotateSpeed = MAX_WHEEL_ROTATE_SPEED;
			rightWheelRotateSpeed = (MAX_WHEEL_ROTATE_SPEED / 2);
			break;

		case MOVE_DIRECTION::BRAKE:
			break;

		default:
			break;
		}

		switch (moveDirection) //이동 방향에 따라 H-브릿지 출력 방향 설정
		{
		case MOVE_DIRECTION::FORWARD:
		case MOVE_DIRECTION::LEFT:
		case MOVE_DIRECTION::RIGHT:
		case MOVE_DIRECTION::LEFT_AND_FORWARD:
		case MOVE_DIRECTION::RIGHT_AND_FORWARD:
			//전진 위한 출력 1 활성화, 출력 2 비활성화
			digitalWrite(wheel_pin::H_BRIDGE_LEFT_INPUT1, HIGH);
			digitalWrite(wheel_pin::H_BRIDGE_LEFT_INPUT2, LOW);
			digitalWrite(wheel_pin::H_BRIDGE_RIGHT_INPUT1, HIGH);
			digitalWrite(wheel_pin::H_BRIDGE_RIGHT_INPUT2, LOW);
			break;

		case MOVE_DIRECTION::BACKWARD:
			//후진 위한 출력 1 비활성화, 출력 2 활성화
			digitalWrite(wheel_pin::H_BRIDGE_LEFT_INPUT1, LOW);
			digitalWrite(wheel_pin::H_BRIDGE_LEFT_INPUT2, HIGH);
			digitalWrite(wheel_pin::H_BRIDGE_RIGHT_INPUT1, LOW);
			digitalWrite(wheel_pin::H_BRIDGE_RIGHT_INPUT2, HIGH);
			break;

		case MOVE_DIRECTION::BRAKE:
			break;

		default:
			break;
		}

		//각 바퀴의 회전 속도에 따라 제어
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_wheel_pin::H_BRIDGE_LEFT_EN_OUTPUT, leftWheelRotateSpeed);
		commonShiftRegPwmInstance.SetPwmData(shift_reg_pin::inner_wheel_pin::H_BRIDGE_RIGHT_EN_OUTPUT, rightWheelRotateSpeed);
	}

private:
	IRrecv _irrecv; //IR 신호 수신 개체
};
#endif