#include "timer.hpp"
#include "file_operation.hpp"
#include "factory.hpp"

using namespace std;
using namespace clany;


class Base
{
public:
    using BasePtr = shared_ptr<Base>;

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

    Base::BasePtr base;
    auto factory = Factory<Derived>();
    base = factory();
    base = factory(1, true);

    using ProdFactory = ObjFactory<Base>;
    ProdFactory::addType("Derived Type", bind(Factory<Derived>(), 1, false));

    base = ProdFactory::create("Derived Type");
    base->foo();

    ProdFactory::removeType("Derived Type");

    timer.elapsed();
	
	return 0;
}