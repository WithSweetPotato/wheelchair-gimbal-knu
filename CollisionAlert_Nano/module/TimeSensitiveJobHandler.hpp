#ifndef _TIME_SENSITIVE_JOB_HANDLER_H_
#define _TIME_SENSITIVE_JOB_HANDLER_H_

#include "../Wheelchair_Core.h"

// https://www.best-microcontroller-projects.com/arduino-millis.html
// https://www.best-microcontroller-projects.com/arduino-timer-interrupt.html
// https://forum.arduino.cc/t/delay-and-interrupts/405046

using TIME_SENSITIVE_JOB_ADDR_TYPE = void(*)(void); //Time Sensitive 작업

/// <summary>
/// Time Sensitive 작업 핸들러
/// </summary>
class TIME_SENSITIVE_JOB_HANDLER
{
public:
	/// <summary>
	/// Time Sensitive 작업 수행 (인터럽트 차단)
	/// </summary>
	/// <param name="timeSensitiveJobAddrType">Time Sensitive 작업</param>
	static inline void HandleBlockingInterruptJob(TIME_SENSITIVE_JOB_ADDR_TYPE timeSensitiveJobAddrType)
	{
		cli(); //인터럽트 비활성화
		timeSensitiveJobAddrType(); //run
		sei(); //인터럽트 재활성화
	}

	/// <summary>
	/// 밀리초 단위 지연 수행 (인터럽트 비차단)
	/// </summary>
	/// <param name="msDuration">밀리초 단위 시간</param>
	static inline void HandleNonBlockingInterruptDelay(unsigned long msDuration)
	{
		unsigned long startMs = millis(); //호출 시점의 시간 (ms)

		while (millis() - startMs < msDuration) //대기해야 할 시간만큼 대기
		{
			//do nothing
		}
	}

	/// <summary>
	/// 마이크로초 단위 지연 수행 (인터럽트 비차단)
	/// </summary>
	/// <param name="usDuration">지연이 수행 될 마이크로초 단위 시간</param>
	static inline void HandleNonBlockingInterruptDelayMs(unsigned long usDuration)
	{
		unsigned long startUs = micros(); //호출 시점의 시간 (us)

		while (micros() - startUs < usDuration) //대기해야 할 시간만큼 대기
		{
			//do nothing
		}
	}

private:
	/// <summary>
	/// TIME_SENSITIVE_JOB_HANDLER 생성자
	/// </summary>
	TIME_SENSITIVE_JOB_HANDLER()
	{
	}

	/// <summary>
	/// TIME_SENSITIVE_JOB_HANDLER 소멸자
	/// </summary>
	~TIME_SENSITIVE_JOB_HANDLER()
	{
	}
};
#endif