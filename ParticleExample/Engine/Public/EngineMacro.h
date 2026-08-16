
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINEMACRO_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINEMACRO_H

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL   _declspec(dllexport)
#else
#define ENGINE_DLL  _declspec(dllimport)
#endif


#define DEBUG_MACRO_TO_METHOD \
__FILE__, __LINE__, __FUNCTION__ 

#define DEBUG_MSG(Message) \
Debugging_Message(DEBUG_MACRO_TO_METHOD, Message);

#define BEGIN(NAME) namespace NAME{
#define END }

#define RETURN_CHECK(SITUATION, RETURN) if(SITUATION) return RETURN;
#define RETURN_CHECK_FAILED(SITUATION, RETURN) if(FAILED(SITUATION)) return RETURN;
#define RETURN_CHECK_DXOBJECT(SITUATION, RETURN)  \
{ HRESULT hr = SITUATION; if (FAILED(hr)) { Check_Error(hr, DEBUG_MACRO_TO_METHOD);  return RETURN; } }

#define Is_Nullptr(T) \
Is_Nullptr_Debug(T, DEBUG_MACRO_TO_METHOD)


#define NO_COPY(ClassName)							\
private: \
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete; 


#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static  SHPTR<ClassName> GetInstance();					\
private:												\
	static SHPTR<ClassName> m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
SHPTR<ClassName> ClassName::m_pInstance = nullptr;			\
SHPTR<ClassName> ClassName::GetInstance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = Create<ClassName>();		\
	return m_pInstance;									\
}														

#define GET_INSTANCE(CLASSNAME) [](){ \
  SHPTR<CLASSNAME> pInstance = CLASSNAME::GetInstance();	\
    return pInstance; \
}();

#pragma region 바꿔야하는구간이다. 

#define CLONE_MACRO(CLASS, DEBUG) \
public: \
 virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) override {   \
SHPTR<CLASS> pInstance { std::make_shared<CLASS>(*this)}; \
if(FAILED(pInstance->NativeConstructClone(_tDatas))) \
{ \
DEBUG_MSG(DEBUG) \
pInstance.reset(); \
} \
return pInstance; \
}


#define CLONE_THIS(CLASS)	\
virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) override { \
return ThisShared<UCloneObject>(); \
} 



#define DESTRUCTOR(CLASS) \
public: \
virtual ~CLASS() { Free(); }



#pragma endregion 바꿔야하는구간


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINEMACRO_H