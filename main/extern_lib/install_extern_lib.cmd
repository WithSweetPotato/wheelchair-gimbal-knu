:: https://github.com/2022-KNU-Microprocessor-Team1/Automatic_Wheelchair_with_Gimbal_Stabilizer_for_visually_impared_and_paraplegic
:: 외부 라이브러리 설치 및 기존 라이브러리 충돌 방지 스크립트
:: https://docs.microsoft.com/ko-kr/windows-server/administration/windows-commands/
@echo off
title 외부 라이브러리 설치 및 기존 라이브러리 충돌 방지 스크립트
setlocal
PUSHD %~DP0

:: 관리자 권한 확인
net session >nul 2>&1
if not %ERRORLEVEL% == 0 (
	goto PERMISSION_DENIED_EX
)

:: Core 라이브러리 백업 디렉토리명
set BACKUP_DIR_NAME=backup
:: 현재 디렉토리명
set SELF_DIR_NAME=extern_lib
:: 자신의 파일명 및 확장자
set SELF_NAME_WITH_EXT=%~nx0
:: Core 라이브러리 경로
:: https://stackoverflow.com/questions/56162405/passing-a-directory-with-spaces-robocopy
set ARDUINO_CORE_LIB_PATH="%ProgramFiles(x86)%\Arduino\libraries"
:: Core 라이브러리 백업 경로
set ARDUINO_CORE_LIB_BACKUP_PATH=.\%BACKUP_DIR_NAME%\
:: 사용자 정의 라이브러리 경로
set ARDUINO_USER_LIB_PATH=%HOMEDRIVE%%HOMEPATH%\Documents\Arduino\libraries\

if exist %ARDUINO_CORE_LIB_PATH%  (
	if exist %ARDUINO_USER_LIB_PATH% (
		goto LOOP
	)
	goto NOT_INSTALLED_EX
)

:: 사용자 입력 처리
:LOOP
echo 0 : Arduino Core 라이브러리 백업 (%ARDUINO_CORE_LIB_PATH% "=>" %ARDUINO_CORE_LIB_BACKUP_PATH%)
echo 1 : Arduino Core 라이브러리 복원 (%ARDUINO_CORE_LIB_BACKUP_PATH% "=>" %ARDUINO_CORE_LIB_PATH%)
echo 2 : Arduino Core 충돌 발생 라이브러리 제거 (%ARDUINO_CORE_LIB_PATH%)
echo 3 : 외부 라이브러리 설치 (%SELF_DIR_NAME% "=>" %ARDUINO_USER_LIB_PATH%)
set /p INPUT=">>"

if "%INPUT%"=="0" (
	goto BACKUP_ARDUINO_CORE_LIB_PROC
)
if "%INPUT%"=="1" (
	goto RESTORE_ARDUINO_CORE_LIB_PROC
)
if "%INPUT%"=="2" (
	goto REMOVE_CRASH_CORE_LIB_PROC
)
if "%INPUT%"=="3" (
	goto INSTALL_EXTERN_LIB_PROC
)

cls
goto LOOP

::: 하위 처리 루틴 정의 시작 :::
:BACKUP_ARDUINO_CORE_LIB_PROC
if not exist %ARDUINO_CORE_LIB_BACKUP_PATH% (
	mkdir %BACKUP_DIR_NAME%
)

robocopy %ARDUINO_CORE_LIB_PATH% %ARDUINO_CORE_LIB_BACKUP_PATH% /E
goto LOOP

:RESTORE_ARDUINO_CORE_LIB_PROC
if not exist %ARDUINO_CORE_LIB_BACKUP_PATH% (
	echo 백업이 수행되지 않았습니다.
	goto LOOP
)

robocopy %ARDUINO_CORE_LIB_BACKUP_PATH% %ARDUINO_CORE_LIB_PATH% /MIR
goto LOOP

:REMOVE_CRASH_CORE_LIB_PROC
cd %ARDUINO_CORE_LIB_PATH%
:: 서보 라이브러리 제거
rmdir /s /q Servo
PUSHD %~DP0
goto LOOP

:INSTALL_EXTERN_LIB_PROC
:: 자신 및 백업 폴더 제외 모두 복사
robocopy ../%SELF_DIR_NAME%/ %ARDUINO_USER_LIB_PATH% /E /XD %BACKUP_DIR_NAME% /XF %SELF_NAME_WITH_EXT%
goto LOOP
::: 하위 처리 루틴 정의 끝 :::

::: 예외 정의 시작 :::
:NOT_INSTALLED_EX
echo Arduino IDE가 설치되지 않았습니다.
pause
goto EXIT

:PERMISSION_DENIED_EX
echo 관리자 권한으로 실행해야합니다.
pause
goto EXIT
::: 예외 정의 끝 :::

:EXIT
exit