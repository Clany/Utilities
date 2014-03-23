#include "timer.hpp"
#include "file_operation.hpp"
#include "factory.hpp"

using namespace std;
using namespace clany;


class Base
{
public:
    using Ptr = shared_ptr<Base>;

    virtual void foo() { cout << "calling base foo()" << endl; }
};

class Derived :public Base
{
public:
    Derived() = default;
    Derived(int, bool) {};

    void foo() override { cout << "calling derived foo()" << endl; }
};


int main(int argc, char* argv[])
{
    CPUTimer timer;

    Base::Ptr base;
    Factory<Derived> factory;
    base = factory();
    base = factory(1, true);

    using ProdFactory = ObjFactory<Base, string, Base::Ptr(int, bool)>;
    ProdFactory::addType("Derived Type", Factory<Derived>());

    base = ProdFactory::create("Derived Type", 1, false);
    base->foo();

    ProdFactory::removeType("Derived Type");

    timer.elapsed();
	
	return 0;
}