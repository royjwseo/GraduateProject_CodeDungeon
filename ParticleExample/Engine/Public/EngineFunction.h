
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINEFUNCTION_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINEFUNCTION_H

namespace Engine {

	template <typename... Args>
	constexpr char* GetArgsTypeName()
	{
		return typeid(Args...).name();
	}


#pragma region FUNCTION 
	template<typename T>
	void Safe_Delete(T& _t) {
		delete _t;
		_t = nullptr;
	}

	template<typename T>
	void Safe_Delete_Array(T& _t) {
		delete[] _t;
		_t = nullptr;
	}

	template<typename T>
	bool Is_Nullptr_Debug(T& _data, const char* FILE,
		const unsigned int& LINE, const char* FUNC)
	{
		if (nullptr == _data)
		{
			char	szMessage[MAX_PATH] = "";
			strcpy_s(szMessage, typeid(T).name());
			_string ClassName = szMessage;
			_string str = " Is Nullptr";
			str += "\nFILE: ";
			str += FILE;
			str += "\n LINE: ";
			str += std::to_string(LINE);
			str += "\n";
			str += FUNC;
			ClassName.append(str);
#ifdef _DEBUG
			MessageBoxA(0, ClassName.c_str(), nullptr, MB_OK);
#endif
			return true;
		}
		return false;
	}


	static void Debugging_Message(const char* FILE,
		const unsigned int& LINE, const char* FUNC, const _string& _pMessage)
	{
#ifdef _DEBUG
		char	szMessage[MAX_PATH] = "";
		_string ClassName = _pMessage;
		_string str = "";
		str += "\nFILE: ";
		str += FILE;
		str += "\n LINE: ";
		str += std::to_string(LINE);
		str += "\n";
		str += FUNC;
		ClassName.append(str);
		MessageBoxA(0, ClassName.c_str(), nullptr, MB_OK);
#endif
	}

	static void Check_Error(const HRESULT& _eResult, const char* FILE,
		const unsigned int& LINE, const char* FUNC) {
		_string str = std::to_string(_eResult);
		str += "\nFILE: ";
		str += FILE;
		str += "\n LINE: ";
		str += std::to_string(LINE);
		str += "\n";
		str += FUNC;
		MessageBoxA(0, str.c_str(), nullptr, MB_OK);
	}

	template<typename T>
	static void ErrorToCreateClass(T& _rhs) {
		MessageBoxA(0, typeid(T).name(), nullptr, MB_OK);
	}

	// 생성자가 동작하는지 체크하고 동작하면 리턴하는 함수 NativeConstruct 함수가 없어도 된다. 
	template<class T, class ...Args>
	requires ConstructWidthArgsCheck<T, Args...>
	static SHPTR<T> Create(Args&&... args) {
		SHPTR<T> pInstance { std::make_shared<T>(args...)};
		return pInstance;
	}
	// 함수에서 값을 받아와서 객체를 만들어 내는 것
	template<class T, class ...Args>
		requires	CheckToSameMethodArgs<T, Args...> 
	static SHPTR<T> CreateNative(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance { std::make_shared<T>()};
		if (FAILED(pInstance->NativeConstruct(args...))) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}


	// 함수에서 값을 받아와서 객체를 만들어 내는 것
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateNativeNotMsg(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance {std::make_shared<T>()};
		if (FAILED(pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return pInstance;
	}

	// 생성자로 Device, GpuCommand를 받고 나머지 값들을 NativeCosntruct 함수에서 받는 것
	template<class T, class ...Args>
		requires  CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateConstructorNative(CSHPTRREF<class UDevice> _pDevice, Args&&... args) {
		SHPTR<T> pInstance { std::make_shared<T>(_pDevice)};
		if (FAILED(pInstance->NativeConstruct(args...))) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}

	// 생성자로 Device, GpuCommand를 받고 나머지 값들을 NativeCosntruct 함수에서 받는 것
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
		static SHPTR<T> CreateConstructorNativeNotMsg(CSHPTRREF<class UDevice> _pDevice, Args&&... args) {
		SHPTR<T> pInstance { std::make_shared<T>(_pDevice)};
		if (FAILED(pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return pInstance;
	}

	// 생성자에서만 값을 받아오는 것
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateConstructorToNative(Args&&... args) {;
	static_assert(ConstructWidthArgsCheck<T, Args...>, "생성자의 인자가 잘못되었습니다.");
	SHPTR<T> pInstance { std::make_shared<T>(args...)};
		if (FAILED(pInstance->NativeConstruct())) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}

	// 생성자에서만 값을 받아오는 것
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateConstructorToNativeNotMsg(Args&&... args) {
		static_assert(ConstructWidthArgsCheck<T, Args...>, "생성자의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{  std::make_shared<T>(args...) };
		if (FAILED(pInstance->NativeConstruct())) {
			pInstance.reset();
		}
		return pInstance;
	}

	// 자기 자신을 클론하는 함수 
	template<class T>
	static SHPTR<T> CloneThis(const T& _rhs)
	{
		SHPTR<T> pInstance {std::make_shared<T>(_rhs)};
		return pInstance;
	}


#ifdef _USE_ATOMIC
	template<class T, class U>
	SHPTR<T> ATOM_STATIC_CAST(const SHPTR<U>& _ptr)
	{
		return std::static_pointer_cast<T>(_ptr.getShared());
	}

	template<class T, class U>
	SHPTR<T> ATOM_DYNAMIC_CAST(const SHPTR<U>& _ptr)
	{
		return std::dynamic_pointer_cast<T>(_ptr.getShared());
	}
#else
	template<class T, class U>
	SHPTR<T>CUS_STATIC_CAST(const SHPTR<U>& _ptr)
	{
		return SHPTR<T>(_ptr, static_cast<T*>(_ptr.get()));
	}

	template<class T, class U>
	SHPTR<T> CUS_DYNAMIC_CAST(const SHPTR<U>& _ptr)
	{
		if (auto* ptr = dynamic_cast<typename SHPTR<T>::element_type*>(_ptr.get())) {
			return SHPTR<T>(_ptr, ptr);
		}
		else {
			return SHPTR<T>();
		}
	}
#endif
#pragma endregion FUNCTION
}


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINEFUNCTION_H