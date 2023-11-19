#include <gtest/gtest.h>

#include "../src/math.hpp"

TEST(FooTest, negative_derived) {

    Base::initVtable();
    Derived::initVtable();

    Derived d;
    EXPECT_NE(d.call("foo", 2, 5), 11) << "Wrong result";
}

TEST(FooTest, derived) {
    Base::initVtable();
    Derived::initVtable();
    
    Derived d;
    EXPECT_EQ(d.call("foo", 2, 5), 10) << "Wrong result";
}

TEST(FooTest, base) {
    Base::initVtable();
    Derived::initVtable();
    
    Base d;
    EXPECT_EQ(d.call("foo",2, 5), 7) << "Wrong result";
}

TEST(FooTest, baseDerived) {
    Base::initVtable();
    Derived::initVtable();
    
    Derived d;
    Base* pb = &d;
    EXPECT_EQ(pb->call("foo",2, 5), 10) << "Wrong result";
}

TEST(FooTest, baseDerivedCpy) {
    Base::initVtable();
    Derived::initVtable();

    Derived d;
    Base b = d;
    EXPECT_EQ(b.call("foo",2, 5), 7) << "Wrong result";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
