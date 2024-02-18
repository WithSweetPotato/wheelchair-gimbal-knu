#include "./Wheelchair_Core.h"

// CollisionAlert.hpp
extern int MIN_GL5537_THRESHOLD; //최소 조도 센서 측정 임계 값
extern int MAX_GL5537_THRESHOLD; //최대 조도 센서 측정 임계 값
extern unsigned long COLLISION_ALERT_CM_THRESHOLD; //충돌 경고 임계 값 (cm 단위)

// GimbalStabilizer.hpp

WHEELCHAIR wheelchair;

/// <summary>
/// 한 번 실행
/// </summary>
void setup()
{
#ifdef DEBUG_MODE
  Serial.begin(BITS_PER_SECONDS);
#endif

  wheelchair.Init();

  //충돌 경고 위한 임계 값 설정
  MIN_GL5537_THRESHOLD = 60; //최소 조도 센서 측정 임계 값
  MAX_GL5537_THRESHOLD = 970; //최대 조도 센서 측정 임계 값
  COLLISION_ALERT_CM_THRESHOLD = (MIN_HCSR04_CM_RANGE + 20); //충돌 경고 임계 값 (cm 단위)
}

/// <summary>
/// 반복 실행
/// </summary>
void loop()
{
	wheelchair.RunTask();
}
