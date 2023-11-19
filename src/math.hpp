#ifndef MATH_HPP
#define MATH_HPP

#include <string>
#include <map>
#include <functional>

class Base;
class Derived;

class CallHelper
{
public:
    CallHelper() = default;
    CallHelper(int (Base::*f)(int, int)) : m_f(f)
    {
    }
    CallHelper(int (Derived::*f)(int, int)) : m_f(f)
    {
    }

    int operator () (Base& obj, int x, int y);
    int operator () (Base&& obj, int x, int y);

    int operator () (Derived& obj, int x, int y);
    int operator () (Derived&& obj, int x, int y);
private:
    using Function = std::variant<
            int (Base::*)(int, int),
            int (Derived::*)(int, int)>;

    Function m_f; 
};

class Base
{
public:
    using vtable_key = std::string;
    // typedef std::string vtable_key;

public:
    Base(): m_vptr(&s_vtable)
    {
    }
    Base(const Base&): m_vptr(&s_vtable)
    {
    }
    Base(Base &&): m_vptr(&s_vtable)
    {
    }
    Base& operator=(const Base&)
    {
        m_vptr = &s_vtable;
        return *this;
    }
    Base& operator=(const Base&&)
    {
        m_vptr = &s_vtable;
        return *this;
    }

    int foo(int x, int y)
    {
        return x + y;
    }

    int g(int x, int y)
    {
        return x + y;
    }

    int call(vtable_key func, int x, int y)
    {
        VTable::iterator f = m_vptr->find(func);
        if (f != m_vptr->end())
            return (f->second)(*this, x, y);
        throw std::logic_error("Function not found.");
    }

public:
    static void initVtable()
    {
        std::string key = "foo";
        s_vtable[key] = CallHelper(&Base::foo);
        s_vtable["g"] = CallHelper(&Base::g);
    }

protected:
    using VTable = std::map<vtable_key, CallHelper>;
    static VTable s_vtable;

    VTable* m_vptr;
};

Base::VTable Base::s_vtable = Base::VTable();

class Derived: public Base
{
public:
    using vtable_key = std::string;

public:
    Derived()
    {
        m_vptr = &s_vtable;
    }
    Derived(const Derived&)
    {
        m_vptr = &s_vtable;
    }
    Derived(Derived &&)
    {
        m_vptr = &s_vtable;
    }
    Derived& operator=(const Derived&)
    {
        m_vptr = &s_vtable;
        return *this;
    }
    Derived& operator=(const Derived&&)
    {
        m_vptr = &s_vtable;
        return *this;
    }

    int foo(int x, int y)
    {
        return x * y;
    }

    int call(vtable_key func, int x, int y)
    {
        VTable::iterator f = m_vptr->find(func);
        if (f != m_vptr->end())
            return (f->second)(*this, x, y);
        throw std::logic_error("Function not found.");
    }

public:
    static void initVtable()
    {
        s_vtable = Base::s_vtable;
        s_vtable["foo"] = CallHelper(&Derived::foo);
    }

protected:
    static VTable s_vtable;
};

Base::VTable Derived::s_vtable = Base::VTable();

int CallHelper::operator () (Base& obj, int x, int y)
{
    if (std::holds_alternative<int (Base::*)(int, int)>(m_f))
    {
        int (Base::*func)(int, int) = std::get<int (Base::*)(int, int)>(m_f);
        return std::invoke(func, obj, x, y);
    }
    else if (std::holds_alternative<int (Derived::*)(int, int)>(m_f))
    {
        int (Derived::*func)(int, int) = std::get<int (Derived::*)(int, int)>(m_f);
        return std::invoke(func, static_cast<Derived&>(obj), x, y);
    }
    throw std::logic_error("No function conversion available");
}

int CallHelper::operator () (Derived& obj, int x, int y)
{
    if (std::holds_alternative<int (Base::*)(int, int)>(m_f))
    {
        int (Base::*func)(int, int) = std::get<int (Base::*)(int, int)>(m_f);
        return std::invoke(func, static_cast<Base&>(obj), x, y);
    }
    else if (std::holds_alternative<int (Derived::*)(int, int)>(m_f))
    {
        int (Derived::*func)(int, int) = std::get<int (Derived::*)(int, int)>(m_f);
        return std::invoke(func, static_cast<Derived&>(obj), x, y);
    }
    throw std::logic_error("No function conversion available");
}

int CallHelper::operator () (Base&& obj, int x, int y)
{
    if (std::holds_alternative<int (Base::*)(int, int)>(m_f))
    {
        int (Base::*func)(int, int) = std::get<int (Base::*)(int, int)>(m_f);
        return std::invoke(func, obj, x, y);
    }
    else if (std::holds_alternative<int (Derived::*)(int, int)>(m_f))
    {
        int (Derived::*func)(int, int) = std::get<int (Derived::*)(int, int)>(m_f);
        return std::invoke(func, static_cast<Derived&&>(obj), x, y);
    }
    throw std::logic_error("No function conversion available");
}

int CallHelper::operator () (Derived&& obj, int x, int y)
{
    if (std::holds_alternative<int (Base::*)(int, int)>(m_f))
    {
        int (Base::*func)(int, int) = std::get<int (Base::*)(int, int)>(m_f);
        return std::invoke(func, static_cast<Base&&>(obj), x, y);
    }
    else if (std::holds_alternative<int (Derived::*)(int, int)>(m_f))
    {
        int (Derived::*func)(int, int) = std::get<int (Derived::*)(int, int)>(m_f);
        return std::invoke(func, static_cast<Derived&&>(obj), x, y);
    }
    throw std::logic_error("No function conversion available");
}
#endif // MATH_HPP