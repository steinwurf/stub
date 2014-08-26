stub
====

stub is a simple C++ library that is meant to provide simple helper
functions to ease sometimes tedious testing tasks.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=stub
    :target: http://buildbot.steinwurf.dk/stats?projects=stub

Usage
-----
The ``stub::call`` object act like a "sink" for function calls
i.e. we can define a call object to accept any type of function
call and it will simply store the arguments for later inspection.

The typical use-case for the call object is when testing that
some code invokes a specific set of functions with a specific
set of arguments.

Header-only
...........

The library itself is header-only so essentially to use it you just
have to clone the repository and setup the right include paths in the
project where you would like to use it.

The library uses c++11 features such as variadic templates, so you
need a relatively recent compiler to use it.

Function Calls
--------------

One of the useful features of the call stub is the possibility to
check the parameters of the "simulated" function calls:

Check a set of function calls
.............................

Example:

::

   #include<stub/call.hpp>

   stub::call<void(uint32_t)> some_function;

The above call takes an ``uint32_t`` and returns nothing, lets see how to
invoke it:

::

    some_function(3);
    some_function(4);

Now we may check how the function was invoked. Using the
``expect_calls()`` function we can create an expectation with will be
checked against the actual call and convert to either ``true`` if the
expectation matches or ``false`` if not.

::

    // Expectation matches how we called the function
    bool works = some_function.expect_calls()
        .with(3U)
        .with(4U);

    assert(works == true);

    // Not the right call order
    works = some_function.expect_calls()
        .with(4U)
        .with(3U);

    assert(works == false);


Functions taking no arguments
............................

The ``with(...)`` function takes exactly the same number and type of
arguments as the ``stub::call`` function.

::

    stub::call<void()> function;
    function();
    function();

    // Is matched by:
    bool works = function.expect_calls()
        .with()
        .with();

    assert(works);


Check a number of similar function calls
........................................

If we have a bunch of similar function calls it can be tedious to
setup an expectation to match it. To make this easier we can call
``repeat(...)`` on the expectation object. This will copy the
arguments of the last ``with(...)`` a number of times.

::

    stub::call<void()> function;

    function();
    function();
    function();
    function();
    function();
    function();

    bool works = function.expect_calls()
        .with().repeat(5);

    assert(works == true);

Is the same as:

::

    stub::call<void()> function;
    function();
    function();
    function();
    function();
    function();
    function();

    bool works = function.expect_calls()
        .with()
        .with()
        .with()
        .with()
        .with()
        .with();

    assert(works == true);

Check the number of function calls
..................................

It is also possible to directly check the number of function calls
made.

::

    stub::call<void(uint32_t)> some_function;

    some_function(3);
    some_function(4);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    // Return true if no calls were made
    assert(some_function.no_calls() == false);

Check only some function calls
..............................

Sometimes we might not care about the arguments to all function
calls. If that is the case we can use the ``ignore(...)`` function to
ignore some of the calls.

::

    stub::call<void(uint32_t,uint32_t)> function;
    function(3,1);
    function(4,2);
    function(5,0);

    assert(function.expect_calls()
        .ignore(2)
        .with(5,0));

Here we ignore the first two calls and only check the last one. The
ignore function can be used in between ``with(...)`` calls if wanted.

::

     stub::call<void(uint32_t,uint32_t)> function;
     function(3,1);
     function(4,2);
     function(5,0);

     assert(function.expect_calls()
         .with(3,1)
         .ignore(1)
         .with(5,0));

Here we ignore the arguments to the second call and check only the
first and last calls.


Check the most recent function call
...................................

We can also check the arguments of the most recent function call.

::

    stub::call<void(uint32_t,uint32_t)> function;

    function(3,4);
    function(4,3);
    function(2,6);

    assert(function.expect_calls()
        .ignore(function.calls() - 1)
        .with(2,6));

Get the arguments of a specific function call
.............................................

If you are interested in manually inspecting the arguments passed to a
function call this can be done using the ``call_arguements(uint32_t)``
function.

::

    stub::call<void(uint32_t,uint32_t)> function;

    function(3,4);
    function(4,3);
    function(2,6);

Internally the arguments are stored in a ``std::tuple`` so in this
case it would be ``std::tuple<uint32_t,uint32_t>``. Lets compare the
arguments of the second call:

::

   auto a = function.call_arguments(2);
   auto b = std::make_tuple(4,3);

   assert(a == b);

**Note:** You should use the "unqualified types" of the function
arguments. This means that if you have a function
``stub::call<void(const uint32_t&>`` then the stub library will store
the argument passed in an ``uint32_t`` instead of a ``const
uint32_t&``. So our comparison should use ``std::tuple<uint32_t>``

You can find more information about unqualified types `here
<http://stackoverflow.com/questions/17295169>`_ and `here
<http://bit.ly/1Markab>`_.

Comparing custom arguments
..........................

The default behavior for the ``expect_calls(...)`` function is to
compare arguments passed though the ``with(...)`` function to the
actual arguments using the ``operator==(...)`` function. However,
sometimes we want to make custom comparisons or to compare objects
that do not provide ``operator==(...)``. In those cases we can provide
a custom comparison function.

Lets say we have a custom object:

::

    struct cup
    {
        double m_volume;
    };

And a function with takes those objects as arguments:

::

    stub::call<void(const cup&)> function;

    function(cup{2.3});
    function(cup{4.5});

    auto p = [](const std::tuple<cup>& a,
                const std::tuple<cup>& b) -> bool
        { return std::get<0>(a).m_volume == std::get<0>(b).m_volume; };

    assert(function.expect_calls(p)
        .with(cup{2.3})
        .with(cup{4.5}));

In this case we are using a c++11 lambda function as comparison
function. Notice that we get the arguments wrapped in ``std::tuple``
objects (as unqualified types see above if you don't know what that
means).

As another example use a custom comparison for objects that do have
``operator==(...)`` but where we have custom equality criteria.

In this case we consider two ``std::pair<uint32_t,uint32_t>`` objects
equal if their second element is equal. To do this with the stub
library we need to provide a custom comparison function.

::

    using element = std::pair<uint32_t, uint32_t>;

    auto p = [](const std::tuple<element>& a,
                const std::tuple<element>& b) -> bool
        { return std::get<0>(a).second == std::get<0>(b).second; };

    stub::call<void(const element&)> function;
    function(element(2,3));
    function(element(20,3));

    // We have called the function more than once
    assert(false == function.expect_calls(p)
        .with(element(10,3)));

    // Works since we only match the second value of the pair
    assert(function.expect_calls(p)
        .with(element(1,3))
        .with(element(2,3)));

    // Without the custom comparison it fails
    assert(false == function.expect_calls()
        .with(element(1,3))
        .with(element(2,3)));

Function return values
----------------------

We can also define a ``stub::call`` which returns a value:

::

    stub::call<bool(uint32_t)> some_function;

Here we have to specify what return value we expect:

::

    some_function.set_return(true);

    bool a = some_function(23);
    bool b = some_function(13);

    assert(a == true);
    assert(b == true);

Or alternatively we can set multiple return values:

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

Unit testing
------------

The unit tests for the stub library are located in the ``test/src`` folder.

We use the Google Unit Testing Framework (gtest) to drive the unit
tests. To build the tests run:

::

    python waf configure
    python waf

Depending on the platform you should see a test binary called
``stub_tests`` in (extension also depends on operating system
e.g. ``.exe`` for windows):

::

    build/platform/test/

Where ``platform`` is typically is either linux, win32 or darwin
depending on your operating system.


License
-------
The stub library is released under the BSD license see the LICENSE.rst file
