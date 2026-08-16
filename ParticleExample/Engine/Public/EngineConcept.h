#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_ENGINECONCEPT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_ENGINECONCEPT_H

namespace Engine
{
	template <class T, class U>
	concept SameType = std::is_same_v<T, U>();
	template <class T, class U>
	concept ParentsChildType = std::is_base_of_v<T*, U*>() || std::is_base_of_v<U*, T*>();
	template<class T, class U>
	concept Convertible = std::is_convertible_v<T*, U*>();

	template <typename T>
	concept SharedPtrConvertible = requires(T t) {
		{ std::shared_ptr<T>(std::move(t)) };
	};

	template <typename T, typename U>
	concept ConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U> || Convertible<T, U>);

	// 이 ConCept은 생성자 인자들이 제대로 있는지 확인하는 ConCept이다. 
	template<typename T, class ...Args>
	concept ConstructWidthArgsCheck = std::is_constructible_v<T, Args...>;

	// 생성자가 있는지 체크
	template <typename T, typename DevicePtr, typename GpuCommandPtr>
	concept ConstructArgsMatch = std::is_constructible_v<T, DevicePtr, GpuCommandPtr>;

	// 이 Conecept은 T::NativeConstruct의 인자가 맞는지 확인하는 Concept이다. 
	template <typename T, typename... Args>
	// 해당 함수가 있는지 확인해서 인자들을 비교한다. 그리고 HRESULT 라는 반환 값이 동일한지 체크
	concept CheckToSameMethodArgs = requires(T * t, Args&&... args) {
		{ t->NativeConstruct(args...) } -> std::same_as<HRESULT>;
	};
	
	template<class T>
	concept CheckNumber =  std::is_same_v<T, unsigned int> || std::is_same_v<T, float> || std::is_same_v<T, short> || std::is_same_v<T, long> ||
		std::is_same_v<T, unsigned short> || std::is_same_v<T, int> || std::is_same_v<T, unsigned long> || std::is_same_v<T, unsigned long long> 
		|| std::is_same_v<T, long long>	|| std::is_same_v<T, unsigned char> || std::is_same_v<T, signed char>;

	template<class T>
	concept CheckAtomicNumber = std::is_same_v<T, std::atomic<unsigned int>> || std::is_same_v<T, std::atomic<float>> 
		|| std::is_same_v<T, std::atomic<short>> || std::is_same_v<T, std::atomic<long>> ||	std::is_same_v<T, std::atomic<unsigned short>>
		|| std::is_same_v<T, std::atomic<int>> || std::is_same_v<T, std::atomic<unsigned long>> || std::is_same_v<T, std::atomic<unsigned long long>>
		|| std::is_same_v<T, std::atomic<long long>> || std::is_same_v<T, std::atomic<unsigned char>> || std::is_same_v<T, std::atomic<signed char>>;

	template <typename T, typename U>
	concept SharedPtrConvertibleFrom = SharedPtrConvertible<U> && (SameType<T, U> || ParentsChildType<T, U>);
	template<typename T, typename U>
	concept AtomicPtrConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U>);
}


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_ENGINECONCEPT_H