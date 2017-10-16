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
//#include "Delegate.h"
//#include <list>
//#include <functional>
//
//namespace SA {
//
//	template<typename RET, typename ...PARAMS>
//	class Event<RET(PARAMS...)> final : private Delegate_Base<RET(PARAMS...)> {
//	public:
//
//		Event() = default;
//		~Event() {
//			for (auto& element : invocationList) delete element;
//			invocationList.clear();
//		} //~Event
//
//		bool isNull() const { return invocationList.size() < 1; }
//		bool operator ==(void* ptr) const {
//			return (ptr == nullptr) && this->isNull();
//		} //operator ==
//		bool operator !=(void* ptr) const {
//			return (ptr != nullptr) || (!this->isNull());
//		} //operator !=
//
//		size_t size() const { return invocationList.size(); }
//
//		Event& operator =(const Event&) = delete;
//		Event(const Event&) = delete;
//
//		bool operator ==(const Event& another) const {
//			if (invocationList.size() != another.invocationList.size()) return false;
//			auto anotherIt = another.invocationList.begin();
//			for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
//				if (**it != **anotherIt) return false;
//			return true;
//		} //==
//		bool operator !=(const Event& another) const { return !(*this == another); }
//
//		bool operator ==(const Delegate<RET(PARAMS...)>& another) const {
//			if (isNull() && another.isNull()) return true;
//			if (another.isNull() || (size() != 1)) return false;
//			return (another.invocation == **invocationList.begin());
//		} //==
//		bool operator !=(const Delegate<RET(PARAMS...)>& another) const { return !(*this == another); }
//
//		Event& operator +=(const Event& another) {
//			for (auto& item : another.invocationList) // clone, not copy; flattens hierarchy:
//				this->invocationList.push_back(new typename Delegate_Base<RET(PARAMS...)>::InvocationElement(item->object, item->stub));
//			return *this;
//		} //operator +=
//
//		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
//		Event& operator +=(const LAMBDA & lambda) {
//			Delegate<RET(PARAMS...)> d = Delegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
//			return *this += d;
//		} //operator +=
//
//		Event& operator +=(const Delegate<RET(PARAMS...)>& another) {
//			if (another.isNull()) return *this;
//			this->invocationList.push_back(new typename Delegate_Base<RET(PARAMS...)>::InvocationElement(another.invocation.object, another.invocation.stub));
//			return *this;
//		} //operator +=
//
//		// will work even if RET is void, return values are ignored:
//		// (for handling return values, see operator(..., handler))
//		void operator()(PARAMS... arg) const {
//			for (auto& item : invocationList)
//				(*(item->stub))(item->object, arg...);
//		} //operator()
//
//		template<typename HANDLER>
//		void operator()(PARAMS... arg, HANDLER handler) const {
//			size_t index = 0;
//			for (auto& item : invocationList) {
//				RET value = (*(item->stub))(item->object, arg...);
//				handler(index, &value);
//				++index;
//			} //loop
//		} //operator()
//
//		void operator()(PARAMS... arg, Delegate<void(size_t, RET*)> handler) const {
//			operator()<decltype(handler)>(arg..., handler);
//		} //operator()
//		void operator()(PARAMS... arg, std::function<void(size_t, RET*)> handler) const {
//			operator()<decltype(handler)>(arg..., handler);
//		} //operator()
//
//
//		void Add(const Delegate<RET(PARAMS...)>& another)
//		{
//			this->invocationList.push_back(new typename Delegate_Base<RET(PARAMS...)>::InvocationElement(another.invocation.object, another.invocation.stub));
//		}
//	private:
//
//		std::list<typename Delegate_Base<RET(PARAMS...)>::InvocationElement *> invocationList;
//
//	}; //class Event
//
//} /* namespace SA */
//
#ifndef SE_UTILZ_EVENT_H_
#define SE_UTILZ_EVENT_H_
#include "Delegate.h"
#include <vector>
#include <functional>

namespace SE
{
	namespace Utilz
	{
		template <typename T> class Event;

		/**
		*@brief A collection of delegates.
		*/
		template<typename RET, typename... PARAMS>
		class Event<RET(PARAMS...)>
		{
			std::vector<Delegate<RET(PARAMS...)>> invokerList;
		public:

			/**
			*@brief Register a delegate to the event.
			* Example code:
			* @code
			* void foo(){cout << "Hello World" << endl;}
			* void foo2(){cout << "Hello World" << endl;}
			*class A
			*{
			* public:
			* void foo(){cout << "Hello World" << endl};
			*}
			* Delegate<void()> del(&foo);
			* ev += del;
			* ev += &foo2;
			* ev += [](){ cout << "Hello World" << endl;};
			* ev += {&a, &A::foo};
			* ev(); // Prints "Hello World" four times
			* @endcode
			*/
			Event& operator+=(const Delegate<RET(PARAMS...)>& other)
			{
				invokerList.push_back(other);
				return *this;
			}

			/**
			*@brief No unregistering(for now).
			*/
			Event& operator-=(const Delegate<RET(PARAMS...)>& other) 
			{
				size_t i = 0;
				while(i < invokerList.size())
				{
					if (invokerList[i] != other)
						i++;
					else;
					invokerList.erase(invokerList.begin() + i);
				}
				return *this;
			}

			/**
			*@brief No eqaulity(for now).
			*/
			bool operator==(const Event<RET(PARAMS...)>& other) = delete;
			/**
			*@brief No copying(for now).
			*/
			Event& operator=(const Event<RET(PARAMS...)>& other) = delete;

			/**
			*@brief Invoke all the delegates. No return handling.
			*/
			inline void operator()(PARAMS... args)const
			{
				for (auto& i : invokerList)
					i(std::forward<PARAMS>(args)...);
			}


			/**
			*@brief Invoke all the delegates. With return handling.
			* Example code:
			* @code
			* Event<int(int)> eve;
			* eve += [](int i) {return i * 2; };
			* int i = 5;
			* eve += [i](int a) {return i + a; };
			*
			* eve(1, [](size_t index, int* ret) {
			* 	std::cout << *ret << std::endl;
			* }); // Prints 2, 6
			* @endcode
			*/
			template<typename HANDLER>
			void operator()(PARAMS... param, HANDLER handler) const {
				size_t index = 0;
				for (auto& item : invokerList) {
					RET value = item(std::forward<PARAMS>(param)...);
					handler(index, &value);
					++index;
				}
			}


			/**
			*@brief Invoke all the delegates. With return handling.
			* Example code:
			* @code
			* Event<int(int)> eve;
			* eve += [](int i) {return i * 2; };
			* int i = 5;
			* eve += [i](int a) {return i + a; };
			*
			* eve(1, [](size_t index, int* ret) {
			* 	std::cout << *ret << std::endl;
			* }); // Prints 2, 6
			* @endcode
			*/
			inline void operator()(PARAMS... param, Delegate<void(size_t, RET*)> handler) const {
				operator()<decltype(handler)>(param..., handler);
			}

			/**
			*@brief Invoke all the delegates. With return handling.
			* Example code:
			* @code
			* Event<int(int)> eve;
			* eve += [](int i) {return i * 2; };
			* int i = 5;
			* eve += [i](int a) {return i + a; };
			*
			* eve(1, [](size_t index, int* ret) {
			* 	std::cout << *ret << std::endl;
			* }); // Prints 2, 6
			* @endcode
			*/
			inline void operator()(PARAMS... param, std::function<void(size_t, RET*)> handler) const {
				operator()<decltype(handler)>(param..., handler);
			}


			inline void clear()
			{
				invokerList.clear();
			}
		};

	}
}
#endif
