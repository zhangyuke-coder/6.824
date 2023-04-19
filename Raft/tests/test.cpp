#include <iostream>
#include <string>

using namespace std;

class A {
friend class B;
public:
    A() :
        s("asdf")
    {

    }


private:
    std::string s;
};
class B {
public:
    B() {
        std::thr
    }
    void p() {
        cout << a.s << endl;
    }
public:
    A a;
};
int main() {
    B b;
    b.p();
}