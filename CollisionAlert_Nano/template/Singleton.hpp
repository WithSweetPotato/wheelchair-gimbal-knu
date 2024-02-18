#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

/// <summary>
/// 인스턴스 타입에 따른 싱글톤
/// </summary>
/// <typeparam name="INSTANCE_TYPE">인스턴스 타입</typeparam>
template <typename INSTANCE_TYPE>
class SINGLETON
{
public:
	/// <summary>
	/// 자신의 고유 인스턴스 참조 반환
	/// </summary>
	/// <returns>자신의 고유 인스턴스 참조</returns>
	static INSTANCE_TYPE& GetInstance()
	{
		if (_instance == NULL)
			_instance = new INSTANCE_TYPE();

		return *_instance;
	}

	/// <summary>
	/// 자신의 고유 인스턴스 메모리 할당 해제
	/// </summary>
	static void Dispose()
	{
		delete _instance;
		_instance = NULL;
	}

protected:
	static INSTANCE_TYPE* _instance; //고유 인스턴스

	/// <summary>
	/// SINGLTON 생성자
	/// </summary>
	SINGLETON()
	{
	}

private:
	// https://docs.microsoft.com/ko-kr/cpp/cpp/copy-constructors-and-copy-assignment-operators-cpp?view=msvc-170
	// https://docs.microsoft.com/ko-kr/cpp/cpp/copy-constructors-and-copy-assignment-operators-cpp?view=msvc-170
	//고유 인스턴스에 대한 복사 생성, 대입 방지
	SINGLETON<INSTANCE_TYPE>(const SINGLETON<INSTANCE_TYPE>&) = delete;
	SINGLETON<INSTANCE_TYPE>& operator=(const SINGLETON<INSTANCE_TYPE>&) = delete;

	//고유 인스턴스에 대한 이동 생성, 대입 방지
	SINGLETON<INSTANCE_TYPE>(const SINGLETON<INSTANCE_TYPE>&&) = delete;
	SINGLETON<INSTANCE_TYPE>& operator=(const SINGLETON<INSTANCE_TYPE>&&) = delete;
};

template<typename INSTANCE_TYPE>
INSTANCE_TYPE* SINGLETON<INSTANCE_TYPE>::_instance = NULL;
#endif