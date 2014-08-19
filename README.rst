stub
====

stub is a simple C++ library that is meant to provide simple helper
functions to ease sometimes tedious testing tasks. An example of this
is ``stub::call``. The call object act like a "sink" for function
calls i.e. we can define a call object to accept any type of function
call and it will simply store the arguments for later inspection.

The typical use-case for the call object is when testing that
some code invokes a specific set of functions with a specific
set of arguments.

Example:

::

   #include<stub/call.hpp>

   stub::call<void(uint32_t)> some_function;

The above call takes an uint32_t and returns nothing, lets
invoke it:

::

    some_function(3);
    some_function(4);

Now we may check how the function was called:

::

    bool called_once = some_function.called_once_with(3U);
    assert(called_once == false);

    bool called_with = some_function.called_with(4U);
    assert(called_with == true);

We can also define a call which returns a value:

::

    stub::call<bool(uint32_t)> some_function;

Here we have to specify what return value we expect:

::

    some_function.set_return(true);

    bool a = some_function(23);
    bool b = some_function(13);

    assert(a == true);
    assert(b == true);

Or alternatively set multiple return values:

::
    stub::call<uint32_t()> some_function;

    some_function.set_return({4U,3U});

    uint32_t a = some_function();
    assert(a == 4U);

    uint32_t b = some_function();
    assert(b == 3U);

    uint32_t c = some_function();
    assert(c == 4U);

    uint32_t d = some_function();
    assert(d != 4U);
    assert(d == 3U);

The default behavior is to repeat the specified return values i.e.:

::
    stub::call<uint32_t()> some_function;
    some_function.set_return(3U);

    uint32_t a = some_function();
    uint32_t b = some_function();
    uint32_t c = some_function();

    assert(a == 3U && b == 3U && c == 3U);

This behavior can be change by calling ``no_repeat()`` in which case
the return_handler can only be invoked once per return value
specified:

::
    stub::call<uint32_t()> some_function;
    some_function.set_return(1U).no_repeat();

    uint32_t a = some_function();
    uint32_t b = some_function(); // <---- Crash

    some_function.set_return({1U,2U,3U}).no_repeat();

    uint32_t a = some_function();
    uint32_t b = some_function();
    uint32_t c = some_function();
    uint32_t d = some_function(); // <---- Crash

In addition to the functionality shown in this example the
``stub::call`` object provides a couple of extra functions for
checking the current state. See the src/stub/call.hpp header for more
information.

For more information on the options for return values see the
src/stub/return_handler.hpp
