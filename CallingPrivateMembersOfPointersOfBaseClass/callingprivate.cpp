#include <iostream>
using namespace std;

class Base {
protected:
	virtual void foo() = 0;
};

class A : public Base {
public:
	virtual void foo() {
		cout << "A::foo()" << endl;
	}

	void bar(Base *ptr) {
		//ptr->foo(); // 'Base::foo': cannot access protected member declared in class 'Base'
	}
};

int main() {
	A a;
	a.bar(&a);
}