#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include "../Wheelchair_Core.h"
#include <IRremote.h> // https://github.com/Arduino-IRremote/Arduino-IRremote

static uint8_t MAX_WHEEL_ROTATE_SPEED = (MAX_PWM_VALUE / 2); //최대 바퀴 회전 속도

/// <summary>
/// 적외선 통신 Car MP3 SE-020401 리모콘 버튼 값<para>
/// https://gist.github.com/steakknife/e419241095f1272ee60f5174f7759867
/// </para></summary>
enum class IR_REMOTE_SE_020401_VALUE : const uint32_t
{
	N0 = 0xFF6897, //0
	N1 = 0xFF30CF, //1
	N2 = 0xFF18E7, //2
	N3 = 0xFF7A85, //3
	N4 = 0xFF10EF, //4
	N5 = 0xFF38C7, //5
	N6 = 0xFF5AA5, //6
	N7 = 0xFF42BD, //7
	N8 = 0xFF4AB5, //8
	N9 = 0xFF52AD, //9
	N100P = 0xFF9867, //100+
	N200P = 0xFFB04F, //200+
	MINUS = 0xFFE01F, //-
	PLUS = 0xFFA857, //+
	EQ = 0xFF906F, //EQ
	PREV = 0xFF22DD, //|<<
	NEXT = 0xFF02FD, //>>|
	PLAY = 0xFFC23D, //>||
	CH_PREV = 0xFFA25D, //CH-
	CH = 0xFF629D, //CH
	CH_NEXT = 0xFFE21D //CH+
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
		pinMode(wheel_pin::LEFT_EN_OUTPUT, OUTPUT);
		pinMode(wheel_pin::RIGHT_EN_OUTPUT, OUTPUT);

		pinMode(wheel_pin::LEFT_OUPUT1, OUTPUT);
		pinMode(wheel_pin::LEFT_OUTPUT2, OUTPUT);
		pinMode(wheel_pin::RIGHT_OUTPUT1, OUTPUT);
		pinMode(wheel_pin::RIGHT_OUTPUT2, OUTPUT);

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
		static MOVE_DIRECTION retVal = MOVE_DIRECTION::DO_NOTHING;

		if (this->_irrecv.decode(&this->_signal)) //적외선 신호로부터 복호화 된 데이터가 존재하면
		{
			switch (this->_signal.value)
			{
			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N2): //전진
				retVal = MOVE_DIRECTION::FORWARD;
				break;

			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N8): //후진
				retVal = MOVE_DIRECTION::BACKWARD;
				break;

			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N4): //제자리 좌회전
				retVal = MOVE_DIRECTION::LEFT;
				break;

			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N6): //제자리 우회전
				retVal = MOVE_DIRECTION::RIGHT;
				break;

			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N1): //좌회전 및 전진
				retVal = MOVE_DIRECTION::LEFT_AND_FORWARD;
				break;

			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N3): //우회전 및 전진
				retVal = MOVE_DIRECTION::RIGHT_AND_FORWARD;
				break;

			case static_cast<uint32_t>(IR_REMOTE_SE_020401_VALUE::N5): //정지
				retVal = MOVE_DIRECTION::BRAKE;
				break;

			default:
				//do nothing
				break;
			}

			this->_irrecv.resume();
		}

		return retVal;
	}

	/// <summary>
	/// 이동 방향에 따라 차체 이동
	/// </summary>
	/// <param name="moveDirection">이동 방향</param>
	void MoveTo(const MOVE_DIRECTION& moveDirection)
	{
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

		VAR_DUMP(leftWheelRotateSpeed);
		VAR_DUMP(rightWheelRotateSpeed);

		switch (moveDirection) //이동 방향에 따라 출력 방향 설정
		{
		case MOVE_DIRECTION::FORWARD:
		case MOVE_DIRECTION::LEFT:
		case MOVE_DIRECTION::RIGHT:
		case MOVE_DIRECTION::LEFT_AND_FORWARD:
		case MOVE_DIRECTION::RIGHT_AND_FORWARD:
			//전진 위한 출력 1 활성화, 출력 2 비활성화
			digitalWrite(wheel_pin::LEFT_OUPUT1, HIGH);
			digitalWrite(wheel_pin::LEFT_OUTPUT2, LOW);
			digitalWrite(wheel_pin::RIGHT_OUTPUT1, HIGH);
			digitalWrite(wheel_pin::RIGHT_OUTPUT2, LOW);
			break;

		case MOVE_DIRECTION::BACKWARD:
			//후진 위한 출력 1 비활성화, 출력 2 활성화
			digitalWrite(wheel_pin::LEFT_OUPUT1, LOW);
			digitalWrite(wheel_pin::LEFT_OUTPUT2, HIGH);
			digitalWrite(wheel_pin::RIGHT_OUTPUT1, LOW);
			digitalWrite(wheel_pin::RIGHT_OUTPUT2, HIGH);
			break;

		case MOVE_DIRECTION::BRAKE:
			break;

		default:
			break;
		}

		//각 바퀴의 회전 속도에 따라 제어
		analogWrite(wheel_pin::LEFT_EN_OUTPUT, leftWheelRotateSpeed);
		analogWrite(wheel_pin::RIGHT_EN_OUTPUT, rightWheelRotateSpeed);
	}

private:
	IRrecv _irrecv; //IR 신호 수신 개체
	decode_results _signal; //디코딩 된 IR 신호
};
#endif