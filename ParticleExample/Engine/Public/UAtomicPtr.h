#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UATOMICPTRH
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UATOMICPTRH

namespace Engine {

	/*
	typename std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0
	템플릿 별칭으로 is_convertible_v는 타입 변환이 가능한지 확인하는 템플릿 유틸리이다 

	enable_if_t는 해당 타입으로 변환이 가능한 경우에만 생성자가 생성되도록 하는 것이다.
	requiers는 에러를 내는 것, 템플릿 별칭도 마찬가지이다. 두 개를 잘 조합해서 사용해야 한다.
	*/

	// Atomic을 씌운 smart 포인터를 사용하기 위한 클래스이다. 
	template <typename T>
	class  DEF_CACHE_ALGIN UAtomicSharedPtr {
	public:
		UAtomicSharedPtr() : ptr{ nullptr } {}
		// 복사 생성자
		UAtomicSharedPtr(const UAtomicSharedPtr& sharedPtr) {
			std::shared_ptr<T> shared = sharedPtr.getShared();
			ptr.store(shared);
		}

		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*> || AtomicPtrConvertibleFrom<T, U>, int> = 0>
		UAtomicSharedPtr(const UAtomicSharedPtr<U>& sharedPtr) {
			ptr.store(sharedPtr.getShared());
		}

		UAtomicSharedPtr(UAtomicSharedPtr&& sharedPtr) noexcept { ptr.store(sharedPtr.ptr); sharedPtr.ptr.store(nullptr); }
		UAtomicSharedPtr(const std::shared_ptr<T>& sharedPtr) noexcept : ptr{ sharedPtr } {}

		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*> || std::is_convertible_v<T*, U*>
		|| SharedPtrConvertibleFrom<T, U>, int> = 0>
		UAtomicSharedPtr(const std::shared_ptr<U>&sharedPtr) noexcept { ptr.store(std::static_pointer_cast<T>(sharedPtr)); }

	
		UAtomicSharedPtr(T* _ptr) noexcept { ptr.load().get() = _ptr; }
		UAtomicSharedPtr(const std::nullptr_t& sharedPtr) noexcept : ptr{ sharedPtr } {  }
		~UAtomicSharedPtr() { ptr.exchange(nullptr); }


		UAtomicSharedPtr<T>& operator=(const UAtomicSharedPtr& sharedPtr) {
			std::shared_ptr<T> shared = sharedPtr.getShared();
			ptr.store(shared);
			return *this;
		}

		// 이동 대입 연산자
		UAtomicSharedPtr<T>& operator=(UAtomicSharedPtr&& sharedPtr) noexcept {
			std::shared_ptr<T> shared = sharedPtr.getShared();
			ptr.store(shared);
			sharedPtr.ptr.store(nullptr);
			return *this;
		}

		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
		requires AtomicPtrConvertibleFrom<T, U>
		UAtomicSharedPtr<T>& operator=(const UAtomicSharedPtr<U>& sharedPtr) {
			ptr.store(sharedPtr.getShared());
			return *this;
		}
		UAtomicSharedPtr<T>& operator=(const std::nullptr_t& sharedPtr) {
			ptr.store(sharedPtr);
			return *this;
		}

		UAtomicSharedPtr<T>& operator=(const std::shared_ptr<T>& sharedPtr) {
			ptr = sharedPtr;
			return *this;
		}
		UAtomicSharedPtr<T>& operator=(std::shared_ptr<T>&& sharedPtr) noexcept {
			ptr = sharedPtr;
			sharedPtr = nullptr;
			return *this;
		}
		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
		requires SharedPtrConvertibleFrom<T, U>
		UAtomicSharedPtr<T>& operator=(const std::shared_ptr<U>& sharedPtr) {
			ptr.store(sharedPtr.getShared());
			return *this;
		}

		T* operator->() const {
			return ptr.load().get();
		}

		T* get() const {
			return ptr.load().get();
		}

		T& getRef() {
			return *get();
		}

		 std::shared_ptr<T> getShared() const  {
			return ptr.load();
		}
		const std::atomic<std::shared_ptr<T>>& getAtomicShared() const {
			return ptr;
		}

		UAtomicSharedPtr<T>* operator&() { return this; }

		std::shared_ptr<T> operator*() const {
			return ptr.load();
		}

		template<class U>
		const bool operator ==(const UAtomicSharedPtr<U>& _rhs) const {
			return get() == _rhs.get();
		}
		template<class U>
		const bool operator !=(const UAtomicSharedPtr<U>& _rhs) const {
			return get() != _rhs.get();
		}

		const bool operator ==(const nullptr_t& _rhs)
		{
			return _rhs == get();
		}

		const bool operator !=(const nullptr_t& _rhs)
		{
			return _rhs != get();
		}

		template<class U>
		const bool operator ==(const std::_Vector_iterator<std::_Vector_val<std::_Simple_types<U>>>& _rhs) const {
			return get() == _rhs->get();
		}

		template<class U>
		const bool operator !=(const std::_Vector_iterator<std::_Vector_val<std::_Simple_types<U>>>& _lhs) {
			return _lhs->get() != get();
		}

		void reset()
		{
			ptr.store(nullptr);
		}

		const unsigned int use_count()
		{
			return ptr.load().use_count();
		}

		void swap(UAtomicSharedPtr& _atomicPtr)
		{
			ptr.swap(_atomicPtr.ptr);
		}

		template<class U>
		bool owner_before(const U& _before)
		{
			return ptr.load().owner_before(_before);
		}

		std::string GetName() { return typeid(T).name(); }


		const bool is_lock_free() const
		{
			return ptr.is_lock_free();
		}

		void store(const std::shared_ptr<T>& _atomicPtr, std::memory_order order = std::memory_order_seq_cst) const
		{
			ptr.store(_atomicPtr, order);
		}

		std::shared_ptr<T> load(std::memory_order order = std::memory_order_seq_cst) const
		{
			return ptr.load(order);
		}

		std::shared_ptr<T> exchage(const std::shared_ptr<T>& _atomicPtr, std::memory_order order = std::memory_order_seq_cst) const
		{
			return ptr.exchange(_atomicPtr, order);
		}

		bool compare_exchange_strong(std::shared_ptr<T>& _expected, const std::shared_ptr<T>& _desired,
			std::memory_order success, std::memory_order failure) const noexcept
		{
			return ptr.compare_exchange_strong(_expected, _desired, success, failure);
		}

		bool compare_exchange_weak(std::shared_ptr<T>& _expected, const std::shared_ptr<T>& _desired,
			std::memory_order success, std::memory_order failure) const noexcept
		{
			return ptr.compare_exchange_weak(_expected, _desired, success, failure);
		}

		bool compare_exchange_strong(std::shared_ptr<T>& _expected, const std::shared_ptr<T>& _desired,
			std::memory_order order = std::memory_order_seq_cst) const noexcept
		{
			return ptr.compare_exchange_strong(_expected, _desired, order);
		}

		bool compare_exchange_weak(std::shared_ptr<T>& _expected, const std::shared_ptr<T>& _desired,
			std::memory_order order = std::memory_order_seq_cst) const noexcept
		{
			return ptr.compare_exchange_weak(_expected, _desired, order);
		}

		void wait(std::shared_ptr<T> old, std::memory_order order = std::memory_order_seq_cst) const noexcept
		{
			ptr.wait(old, order);
		}

	private:
		ALIGNAS  std::atomic<std::shared_ptr<T>> ptr;
	};

	template<class T, class U>
	const bool operator ==(const U& _rhs, const UAtomicSharedPtr<T>& _lhs) {
		return _rhs == _lhs.get();
	}
	template<class T, class U>
	const bool operator !=(const U& _rhs, const UAtomicSharedPtr<T>& _lhs) {
		return _rhs != _lhs.get();
	}

	template<class U>
	const bool operator ==(const std::_Vector_iterator<std::_Vector_val<std::_Simple_types<U>>>& _lhs, const UAtomicSharedPtr<U>& _rhs) {
		return _lhs->get() == _rhs.get();
	}

	template<class U>
	const bool operator !=(const std::_Vector_iterator<std::_Vector_val<std::_Simple_types<U>>>& _lhs, const UAtomicSharedPtr<U>& _rhs) {
		return _lhs->get() != _rhs.get();
	}

	// Atomic을 씌운 smart 포인터를 사용하기 위한 클래스이다. 
	template <typename T>
	class UAtomicWeakPtr {
	public:
		UAtomicWeakPtr() { ptr.load().lock() = nullptr; }
		UAtomicWeakPtr(const UAtomicWeakPtr& weakPtr) { ptr.store(weakPtr.ptr); }
		UAtomicWeakPtr(UAtomicWeakPtr&& weakPtr) noexcept { ptr.store(weakPtr.ptr); weakPtr.ptr.store({}); }
		UAtomicWeakPtr(const std::weak_ptr<T>& weakPtr) noexcept : ptr(weakPtr) {}
		UAtomicWeakPtr(const UAtomicSharedPtr<T>& sharedPtr){ ptr.store(sharedPtr.getShared()); }

		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*> || std::is_convertible_v<T*, U*>
		|| SharedPtrConvertibleFrom<T, U>, int> = 0>
		UAtomicWeakPtr(const std::shared_ptr<U>&sharedPtr) noexcept  { ptr.store(sharedPtr); }

		void operator=(const UAtomicWeakPtr& weakPtr) {
			ptr.store(weakPtr.ptr);
		}
		void operator=(const UAtomicSharedPtr<T>& sharedPtr) {
			ptr.store(sharedPtr.getShared());
		}

		void operator=(UAtomicWeakPtr&& weakPtr) {
			ptr.store(weakPtr.ptr);
			weakPtr.ptr.load().lock() = nullptr;
		}
		void operator=(const std::weak_ptr<T>& weakPtr) {
			ptr.store(weakPtr);
		}


		T* get() const {
			return ptr.load().lock().get();
		}

		std::shared_ptr<T> lock() const {
			return ptr.load().lock();
		}
		const std::atomic<std::weak_ptr<T>>& getAtomicWeak() const {
			return ptr;
		}

		 T& operator*()  { return *ptr.load().lock().get(); }
		std::atomic<std::weak_ptr<T>>* operator&() { return &ptr; }

		const bool  operator ==(const UAtomicWeakPtr& _rhs)
		{
			return _rhs.ptr.load().lock() == ptr.load().lock();
		}
		const bool  operator !=(const UAtomicWeakPtr& _rhs)
		{
			return _rhs.ptr.load().lock() != ptr.load().lock();
		}
		template<class U>
		const bool operator ==(const U& _rhs)
		{
			return _rhs == ptr.load().lock();
		}
		template<class U>
		const bool operator !=(const U& _rhs)
		{
			return _rhs != ptr.load().lock();
		}

		void reset()
		{
			ptr.load().lock().reset();
		}

		const unsigned int  use_count()
		{
			return ptr.load().lock().use_count();
		}

		void swap(const UAtomicWeakPtr& _atomicPtr)
		{
			ptr.load().lock().swap(_atomicPtr.ptr);
		}

		template<class U>
		bool owner_before(const U& _before)
		{
			return ptr.load().lock().owner_before(_before);
		}

		std::string GetName() { return typeid(T).name(); }
	private:
		std::atomic<std::weak_ptr<T>> ptr;
	};

	template<class T, class U>
	const bool operator ==(const U& _rhs, const UAtomicWeakPtr<T>& _lhs) {
		return _rhs == _lhs.get();
	}
	template<class T, class U>
	const bool operator !=(const U& _rhs, const UAtomicWeakPtr<T>& _lhs) {
		return _rhs != _lhs.get();
	}
}

namespace std
{
	// Atomic 함수 재정의
	template <typename T>
	struct hash<Engine::UAtomicSharedPtr<T>> {
		size_t operator()(const Engine::UAtomicSharedPtr<T>& ptr) const {
			return std::hash<T*>()(ptr.get());
		}
	};

	template <typename T>
	struct equal_to<Engine::UAtomicSharedPtr<T>> {
		bool operator()(const Engine::UAtomicSharedPtr<T>& lhs, const Engine::UAtomicSharedPtr<T>& rhs) const {
			return lhs.get() == rhs.get();
		}
	};
}

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UATOMICPTRH