:: https://github.com/2022-KNU-Microprocessor-Team1/Automatic_Wheelchair_with_Gimbal_Stabilizer_for_visually_impared_and_paraplegic
:: �ܺ� ���̺귯�� ��ġ �� ���� ���̺귯�� �浹 ���� ��ũ��Ʈ
:: https://docs.microsoft.com/ko-kr/windows-server/administration/windows-commands/
@echo off
title �ܺ� ���̺귯�� ��ġ �� ���� ���̺귯�� �浹 ���� ��ũ��Ʈ
setlocal
PUSHD %~DP0

:: ������ ���� Ȯ��
net session >nul 2>&1
if not %ERRORLEVEL% == 0 (
	goto PERMISSION_DENIED_EX
)

:: Core ���̺귯�� ��� ���丮��
set BACKUP_DIR_NAME=backup
:: ���� ���丮��
set SELF_DIR_NAME=extern_lib
:: �ڽ��� ���ϸ� �� Ȯ����
set SELF_NAME_WITH_EXT=%~nx0
:: Core ���̺귯�� ���
:: https://stackoverflow.com/questions/56162405/passing-a-directory-with-spaces-robocopy
set ARDUINO_CORE_LIB_PATH="%ProgramFiles(x86)%\Arduino\libraries"
:: Core ���̺귯�� ��� ���
set ARDUINO_CORE_LIB_BACKUP_PATH=.\%BACKUP_DIR_NAME%\
:: ����� ���� ���̺귯�� ���
set ARDUINO_USER_LIB_PATH=%HOMEDRIVE%%HOMEPATH%\Documents\Arduino\libraries\

if exist %ARDUINO_CORE_LIB_PATH%  (
	if exist %ARDUINO_USER_LIB_PATH% (
		goto LOOP
	)
	goto NOT_INSTALLED_EX
)

:: ����� �Է� ó��
:LOOP
echo 0 : Arduino Core ���̺귯�� ��� (%ARDUINO_CORE_LIB_PATH% "=>" %ARDUINO_CORE_LIB_BACKUP_PATH%)
echo 1 : Arduino Core ���̺귯�� ���� (%ARDUINO_CORE_LIB_BACKUP_PATH% "=>" %ARDUINO_CORE_LIB_PATH%)
echo 2 : Arduino Core �浹 �߻� ���̺귯�� ���� (%ARDUINO_CORE_LIB_PATH%)
echo 3 : �ܺ� ���̺귯�� ��ġ (%SELF_DIR_NAME% "=>" %ARDUINO_USER_LIB_PATH%)
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

::: ���� ó�� ��ƾ ���� ���� :::
:BACKUP_ARDUINO_CORE_LIB_PROC
if not exist %ARDUINO_CORE_LIB_BACKUP_PATH% (
	mkdir %BACKUP_DIR_NAME%
)

robocopy %ARDUINO_CORE_LIB_PATH% %ARDUINO_CORE_LIB_BACKUP_PATH% /E
goto LOOP

:RESTORE_ARDUINO_CORE_LIB_PROC
if not exist %ARDUINO_CORE_LIB_BACKUP_PATH% (
	echo ����� ������� �ʾҽ��ϴ�.
	goto LOOP
)

robocopy %ARDUINO_CORE_LIB_BACKUP_PATH% %ARDUINO_CORE_LIB_PATH% /MIR
goto LOOP

:REMOVE_CRASH_CORE_LIB_PROC
cd %ARDUINO_CORE_LIB_PATH%
:: ���� ���̺귯�� ����
rmdir /s /q Servo
PUSHD %~DP0
goto LOOP

:INSTALL_EXTERN_LIB_PROC
:: �ڽ� �� ��� ���� ���� ��� ����
robocopy ../%SELF_DIR_NAME%/ %ARDUINO_USER_LIB_PATH% /E /XD %BACKUP_DIR_NAME% /XF %SELF_NAME_WITH_EXT%
goto LOOP
::: ���� ó�� ��ƾ ���� �� :::

::: ���� ���� ���� :::
:NOT_INSTALLED_EX
echo Arduino IDE�� ��ġ���� �ʾҽ��ϴ�.
pause
goto EXIT

:PERMISSION_DENIED_EX
echo ������ �������� �����ؾ��մϴ�.
pause
goto EXIT
::: ���� ���� �� :::

:EXIT
exit