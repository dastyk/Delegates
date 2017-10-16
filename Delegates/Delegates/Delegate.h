///*
//
//	Copyright (C) 2017 by Sergey A Kryukov: derived work
//	http://www.SAKryukov.org
//	http://www.codeproject.com/Members/SAKryukov
//
//	Based on original work by Sergey Ryazanov:
//	"The Impossibly Fast C++ Delegates", 18 Jul 2005
//	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-Delegates
//
//	MIT license:
//	http://en.wikipedia.org/wiki/MIT_License
//
//	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed
//
//*/
//
//#pragma once
//#include "DelegateBase.h"
//
//namespace SA {
//
//	template <typename T> class Delegate;
//	template <typename T> class Event;
//
//	template<typename RET, typename ...PARAMS>
//	class Delegate<RET(PARAMS...)> final : private Delegate_Base<RET(PARAMS...)> {
//	public:
//
//		Delegate() = default;
//
//		bool isNull() const { return invocation.stub == nullptr; }
//		bool operator ==(void* ptr) const {
//			return (ptr == nullptr) && this->isNull();
//		} //operator ==
//		bool operator !=(void* ptr) const {
//			return (ptr != nullptr) || (!this->isNull());
//		} //operator !=
//
//		Delegate(const Delegate& another) { another.invocation.Clone(invocation); }
//
//		template <typename LAMBDA>
//		Delegate(const LAMBDA& lambda) {
//			assign((void*)(&lambda), lambda_stub<LAMBDA>);
//		} //Delegate
//
//		Delegate& operator =(const Delegate& another) {
//			another.invocation.Clone(invocation);
//			return *this;
//		} //operator =
//
//		template <typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
//		Delegate& operator =(const LAMBDA& instance) {
//			assign((void*)(&instance), lambda_stub<LAMBDA>);
//			return *this;
//		} //operator =
//
//		bool operator == (const Delegate& another) const { return invocation == another.invocation; }
//		bool operator != (const Delegate& another) const { return invocation != another.invocation; }
//
//		bool operator ==(const Event<RET(PARAMS...)>& another) const { return another == (*this); }
//		bool operator !=(const Event<RET(PARAMS...)>& another) const { return another != (*this); }
//
//		template <class T, RET(T::*TMethod)(PARAMS...)>
//		static Delegate create(T* instance) {
//			return Delegate(instance, method_stub<T, TMethod>);
//		} //create
//
//		template <class T, RET(T::*TMethod)(PARAMS...) const>
//		static Delegate create(T const* instance) {
//			return Delegate(const_cast<T*>(instance), const_method_stub<T, TMethod>);
//		} //create
//
//		template <RET(*TMethod)(PARAMS...)>
//		static Delegate create() {
//			return Delegate(nullptr, function_stub<TMethod>);
//		} //create
//
//		template <typename LAMBDA>
//		static Delegate create(const LAMBDA & instance) {
//			return Delegate((void*)(&instance), lambda_stub<LAMBDA>);
//		} //create
//
//		RET operator()(PARAMS... arg) const {
//			return (*invocation.stub)(invocation.object, arg...);
//		} //operator()
//
//	private:
//
//		Delegate(void* anObject, typename Delegate_Base<RET(PARAMS...)>::stub_type aStub) {
//			invocation.object = anObject;
//			invocation.stub = aStub;
//		} //Delegate
//
//		void assign(void* anObject, typename Delegate_Base<RET(PARAMS...)>::stub_type aStub) {
//			this->invocation.object = anObject;
//			this->invocation.stub = aStub;
//		} //assign
//
//		template <class T, RET(T::*TMethod)(PARAMS...)>
//		static RET method_stub(void* this_ptr, PARAMS... params) {
//			T* p = static_cast<T*>(this_ptr);
//			return (p->*TMethod)(params...);
//		} //method_stub
//
//		template <class T, RET(T::*TMethod)(PARAMS...) const>
//		static RET const_method_stub(void* this_ptr, PARAMS... params) {
//			T* const p = static_cast<T*>(this_ptr);
//			return (p->*TMethod)(params...);
//		} //const_method_stub
//
//		template <RET(*TMethod)(PARAMS...)>
//		static RET function_stub(void* this_ptr, PARAMS... params) {
//			return (TMethod)(params...);
//		} //function_stub
//
//		template <typename LAMBDA>
//		static RET lambda_stub(void* this_ptr, PARAMS... arg) {
//			LAMBDA* p = static_cast<LAMBDA*>(this_ptr);
//			return (p->operator())(arg...);
//		} //lambda_stub
//
//		friend class Event<RET(PARAMS...)>;
//		typename Delegate_Base<RET(PARAMS...)>::InvocationElement invocation;
//
//	}; //class Delegate
//
//} /* namespace SA */
#ifndef SE_UTILZ_DELEGATE_H_
#define SE_UTILZ_DELEGATE_H_
#include <functional>
namespace SE
{
	namespace Utilz
	{
		template <typename T> class Delegate;

		template<typename RET, typename... PARAMS>
		class Delegate<RET(PARAMS...)>
		{
			std::function<RET(PARAMS...)> invoker;
			size_t uniqueIdentifier;
		public:

			Delegate() {}

			operator bool() { return invoker.operator bool(); }

			/**
			*@brief Copy constructor.
			*/
			Delegate(const Delegate& other)
			{
				this->invoker = other.invoker;
				this->uniqueIdentifier = other.uniqueIdentifier;
			}
			/**
			*@brief Copy constructor with rvalue.
			*/
			Delegate(const Delegate&& other)
			{
				this->invoker = std::move(other.invoker);
				this->uniqueIdentifier = other.uniqueIdentifier;
			}

			/**
			*@brief Create delegate from function pointer.
			*@param ptr The function pointer. (&foo)
			* Example code:
			* @code
			* void Call(const Delegate<void()>& del) { del();}
			* void foo(){cout << "Hello World" << endl;}
			*
			* Delegate<void()> del(&foo);
			* del(); // Prints "Hello World"
			* Call(&foo); // Prints "Hello World"
			* @endcode
			*/
			Delegate(RET(ptr)(PARAMS...)) 
			{
				invoker = ptr;
				uniqueIdentifier = (size_t)ptr;
			}

			/**
			*@brief Create delegate from lambda.
			* Example code:
			* @code
			* void Call(const Delegate<void()>& del) { del();}
			* void foo(){cout << "Hello World" << endl;}
			*
			* Delegate<void()> del([](){ cout << "Hello World" << endl;});
			* del(); // Prints "Hello World"
			* Call([&del](){foo(); del();}); // Prints "Hello World" twice
			* @endcode
			*/

			template <typename T>
			Delegate(const T& lambda)
			{
				invoker = lambda;
				union Converter
				{
					size_t conv;
					const T* ptr;
					Converter(const T& t) : ptr(&t) {}
				};
				Converter ptrToSize_t(lambda);
				uniqueIdentifier = ptrToSize_t.conv;
			}


			/**
			*@brief Create delegate from class method.
			*@param [in] instance The pointer to the class object. (Both this, and &myClass) works.
			*@param [in] TMethod The class method pointer. (&Class::Method)
			* Example code:
			* @code
			* void Call(const Delegate<void()>& del) { del();}
			* class A
			*{
			* public:
			* void foo(){cout << "Hello World" << endl;}
			*}
			*
			* A a;
			* Delegate<void()> del = {&a, &A::foo};
			* del(); // Prints "Hello World"
			* Call(del); // Prints "Hello World"
			* Call({&a, &A::foo}); // Prints "Hello World"
			* @endcode
			*/
			template <class T>
			Delegate(T* instance, RET(T::*TMethod)(PARAMS...))
			{
				invoker = [instance, TMethod](PARAMS... params) -> RET {
					T* p = static_cast<T*>(instance);
					return (instance->*TMethod)(std::forward<PARAMS>(params)...);
				};
				union Converter
				{
					size_t conv;
					RET(T::*ptr)(PARAMS...);
					Converter(RET(T::*ptr)(PARAMS...)) : ptr(ptr){}
				};
				Converter ptrToSize_t(TMethod);
				uniqueIdentifier = (size_t)((((size_t)(instance) +ptrToSize_t.conv)* ((size_t)(instance)+ptrToSize_t.conv + 1))/2) + ptrToSize_t.conv;
			}


			/**
			*@brief Create delegate from const class method.
			*@param [in] instance The pointer to the class object. (Both this, and &myClass) works.
			*@param [in] TMethod The class method pointer. (&Class::Method)
			* Example code:
			* @code
			* void Call(const Delegate<void()>& del) { del();}
			* class A
			*{
			* public:
			* void foo()const{cout << "Hello World" << endl;}
			*}
			*
			* A a;
			* Delegate<void()> del = {&a, &A::foo};
			* del(); // Prints "Hello World"
			* Call(del); // Prints "Hello World"
			* Call({&a, &A::foo}); // Prints "Hello World"
			* @endcode
			*/
			template <class T>
			Delegate(const T* instance, RET(T::*TMethod)(PARAMS...) const)
			{
				invoker = [instance, TMethod](PARAMS... params) -> RET {
					T* const p = const_cast<T*>(instance);
					return (instance->*TMethod)(std::forward<PARAMS>(params)...);
				};
			}

			/**
			*@brief No equal operator for now.
			*/
			inline bool operator==(const Delegate& other)const
			{
				return this->uniqueIdentifier == other.uniqueIdentifier;
			};
			/**
			*@brief No equal operator for now.
			*/
			inline bool operator!=(const Delegate& other)const
			{
				return this->uniqueIdentifier != other.uniqueIdentifier;
			};
			bool operator+=(const Delegate& other)const = delete;
			bool operator-=(const Delegate& other)const = delete;
			bool operator+(const Delegate& other)const = delete;
			bool operator-(const Delegate& other)const = delete;

			
			/**
			*@brief Assignment from Delegate to Delegate.
			*/
			Delegate& operator=(const Delegate& other)
			{
				this->invoker = other.invoker;
				this->uniqueIdentifier = other.uniqueIdentifier;
				return *this;
			}

			/**
			*@brief Assignment from Delegate to Delegate, with rvalue.
			*/
			Delegate& operator=(const Delegate&& other)
			{
				this->invoker = std::move(other.invoker);
				return *this;
			}

			/**
			*@brief Invoke the delegate.
			*/
			inline RET operator()(PARAMS... args)const
			{
				return invoker(std::forward<PARAMS>(args)...);
			}
		};
	}
}
#endif // SE_UTILZ_DELEGATE_H_
