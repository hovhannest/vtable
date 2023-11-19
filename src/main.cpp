#include <iostream>

#include "math.hpp"

int main() {
    Base::initVtable();
    Derived::initVtable();

    Derived d;
    //d.call("foo", 2, 5);


    Base* pb = &d;
    pb->call("foo", 2, 5);
    return 0;
}
