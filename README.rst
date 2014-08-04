stub
====

stub is a simple C++ library that is meant to provide simple helper
functions to ease sometimes tedious testing tasks.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=gauge
    :target: http://buildbot.steinwurf.dk/stats?projects=gauge

Usage
-----
The ``stub::call`` object act like a "sink" for function calls
i.e. we can define a call object to accept any type of function
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

    stub::call<bool(uint32_t)> another_function;

Here we have to specify what return value we expect:

::

    another_function.set_return(true);

    bool a = another_function(23);
    bool b = another_function(13);

    assert(a == true);
    assert(b == true);

In addition to the functionality shown in this example the
``stub::call`` object provides a couple of extra functions for
checking the current state. See the src/stub/call.hpp header for more
information.

For more information on the options for return values see the
src/stub/return_handler.hpp
