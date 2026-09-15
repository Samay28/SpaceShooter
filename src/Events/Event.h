#pragma once
// An event is simply data describing something that happened.

//making struct instead of class because we want to use it as a data structure and not have any member functions or private members.
struct Event
{   
    // A virtual destructor is necessary for polymorphic base classes to ensure that derived class destructors are called correctly when deleting through a base class pointer.
    virtual ~Event() = default;
};