#include "Delegate.h"
#include "Event.h"
#include <iostream>
#include <thread>
#include <stack>

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
using namespace SE::Utilz;

void foo(int i)
{
	int a = i;
	std::cout << "foo" << std::endl;
}
class A
{
private:
	void fooP(int i)
	{
		std::cout << "fooP" << std::endl;
	}
public:
	bool R(const Delegate<void(int)>& del)
	{
		std::cout << "priv ";
		Delegate<void(int)> priv(this, &A::fooP);

		return (del == priv);
	}
	void foo(int i)
	{
			std::cout << i * 2 << std::endl;
	}
	void foo1(int a)
	{
		std::cout << a << std::endl;
	}
	void foovoid()
	{
		int i = 0;
	}
	virtual void vir()
	{
		int i = 0;
	}
};
class B : public A
{
public:
	void foo(int i)
	{
		if (i == 1)
			std::cout << i * 2 << std::endl;
	}
	void vir()
	{
		int i = 1;
	}
};
//
//
//std::stack<Delegate<void(int)>> s;
//
//void Run()
//{
//	for (int i = 0; i < 100; i++)
//	{
//		auto job = s.top();
//		s.pop();
//		job(i);
//		s.push(job);
//	}
//}
//
//void Reg(const Delegate<void(int)>&& del)
//{
//	s.push(del);
//}
//
//void Reg2()
//{
//	int i = 1;
//	Reg([i](int a) {
//		std::cout << i << std::endl;
//	});
//}
#include <chrono>
#include <vector>


void foo2(int i)
{
	int b = 3;
	int c = b*i - b;
	std::cout << "foo2" << std::endl;
}
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	A a;
	B b;
	std::cout << "del1 ";
	Delegate<void(int)> del1(&a, &A::foo);
	std::cout << "del2 ";
	Delegate<void(int)> del2(&a, &A::foo);
	std::cout << "del3 ";
	Delegate<void(int)> del3(&a, &A::foo1);
	std::cout << "del4 ";
	Delegate<void()> del4(&a, &A::foovoid);
	std::cout << "del5 ";
	Delegate<void(int)> del5(&b, &B::foo);
	std::cout << "del6 ";
	Delegate<void()> del6(&a, &A::vir);
	std::cout << "del7 ";
	Delegate<void()> del7(&b, &B::vir);
	std::cout << "del8 ";
	Delegate<void(int)> del8(&foo);
	std::cout << "del9 ";
	Delegate<void(int)> del9(&foo2);
	std::cout << "del10 ";
	auto lamd = [&a](int i) -> void {std::cout << "lam" << std::endl;};	
	Delegate<void(int)> del10(lamd);
	std::cout << "del11 ";	
	Delegate<void(int)> del11(lamd);
	std::cout << "del12 ";
	auto lamd2 = [&a](int i) -> void {std::cout << "lam" << std::endl;};
	Delegate<void(int)> del12(lamd2);

	std::cout << "del1 == del2 yes ";
	std::cout << (del1 == del2 ? "yes" : "no") << std::endl;


	std::cout << "del1 == del3 no ";
	std::cout << (del1 == del3 ? "yes" : "no") << std::endl;

	std::cout << "del3 == del3 yes ";
	std::cout << (del3 == del3 ? "yes" : "no") << std::endl;


	std::cout << "del4 == del4 yes ";
	std::cout << (del4 == del4 ? "yes" : "no") << std::endl;


	std::cout << "del1 == del5 no ";
	std::cout << (del1 == del5 ? "yes" : "no") << std::endl;


	std::cout << "del1 == priv no ";
	std::cout << (a.R(del1) ? "yes" : "no") << std::endl;


	std::cout << "del5 == priv no ";
	std::cout << (a.R(del5) ? "yes" : "no") << std::endl;


	std::cout << "del6 == del6 yes ";
	std::cout << (del6 == del6 ? "yes" : "no") << std::endl;

	std::cout << "del6 == del7 no ";
	std::cout << (del6 == del7 ? "yes" : "no") << std::endl;


	std::cout << "del7 == del7 yes ";
	std::cout << (del7 == del7 ? "yes" : "no") << std::endl;


	std::cout << "del8 == del8 yes ";
	std::cout << (del8 == del8 ? "yes" : "no") << std::endl;

	std::cout << "del8 == del9 no ";
	std::cout << (del8 == del9 ? "yes" : "no") << std::endl;


	std::cout << "del9 == del9 yes ";
	std::cout << (del9 == del9 ? "yes" : "no") << std::endl;
	
	std::cout << "del1 == del8 no ";
	std::cout << (del1== del8 ? "yes" : "no") << std::endl;

	std::cout << "del8 == del10 no ";
	std::cout << (del8 == del10 ? "yes" : "no") << std::endl;

	std::cout << "del10 == del10 yes ";
	std::cout << (del10 == del10 ? "yes" : "no") << std::endl;

	std::cout << "del10 == del11 yes ";
	std::cout << (del10 == del11 ? "yes" : "no") << std::endl;

	std::cout << "del10 == del12 no ";
	std::cout << (del10 == del12 ? "yes" : "no") << std::endl;

	//del8(1);
//	del9(1);


	Event<int(int)> ev;

	auto lam = [](auto i, auto v) {
		std::cout << i << " : " << *v << std::endl;
	};
	auto bl = [](int i) { return i * 3;};
	
	for (int i = 0; i < 100; i++)
		ev += bl;

	ev -= bl;

	ev(1);
/*
	auto l = [](int i) {return i;};
	{
		std::vector < std::function<int(int)> > la;
		for (int i = 0; i < 100000000; i++)
		{
			la.push_back(l);
		}
		auto start = std::chrono::system_clock::now();
		for (auto& i : la)
			i(1);
		auto end = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "LVec " << time.count() << "ms" << std::endl;
	}
	{
		auto start = std::chrono::system_clock::now();
		for (int i = 0; i < 100000000; i++)
		{
			int ret = l(i);
		}
		auto end = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Lambda " << time.count() << "ms" << std::endl;
	}
	{
		Event<int(int)> nev;

		for (int i = 0; i < 100000000; i++)
		{
			nev += l;
		}

		auto start = std::chrono::system_clock::now();
		nev(1);
		auto end = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Event " << time.count() << "ms" << std::endl;
	}
	
	{

		Delegate<int(int)> de = [](int i) {return i;};

		auto start = std::chrono::system_clock::now();
		for (int i = 0; i < 100000000; i++)
		{
		int ret =	de(i);
		}
		auto end = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Delegate " << time.count() << "ms" << std::endl;
	}*/
	getchar();
} /* main */
