#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>
using namespace std;
class A {

public:
    void f1() {
        std::thread t(&A::threadfun,this);
        t.join();
    }
    void threadfun() {
        cout << "qwer" << endl;
    }
};
// void f1() {
//     cout << "qwerqwer" << endl;
// }

int main() {
    A a;
    a.f1();
}