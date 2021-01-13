Usage
-----

The ``stub::function`` object act like a "sink" for function calls
i.e. we can define a function object to accept any type of function
call and it will simply store the arguments for later inspection.

The typical use-case for the function object is when testing that
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

One of the useful features of the function stub is the possibility to
check the parameters of the "simulated" function calls:

Check a set of function calls
.............................

Example:

::

   #include <stub/function.hpp>

   stub::function<void(uint32_t)> some_function;

The above function takes an ``uint32_t`` and returns nothing, lets see how to
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
.............................

The ``with(...)`` function takes exactly the same number and type of
arguments as the ``stub::function``.

::

    stub::function<void()> function;
    function();
    function();

    // Is matched by:
    bool works = function.expect_calls()
        .with()
        .with();

    assert(works);


Check the number of function calls
..................................

It is also possible to directly check the number of function calls
made.

::

    stub::function<void(uint32_t)> some_function;

    some_function(3);
    some_function(4);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    // Return true if no calls were made
    assert(some_function.calls() != 0);

Clear the state of the function object
......................................

Somethings we need to reset things to its initial state::

    stub::function<uint32_t(uint32_t)> some_function;
    some_function.set_return(5);

    uint32_t a = some_function(3);
    uint32_t b = some_function(4);

    assert(a == 5);
    assert(b == 5);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    some_function.clear();

    // Return true if no calls were made
    assert(some_function.calls() == 0);

    // Before we can use some_function again we have to set a new return value
    some_function.set_return(6);
    uint32_t c = some_function(1);

    assert(c == 6);

Clear only the function calls state
...................................

It is also possible to just clear the recorded function calls. Using ``clear()``
will also remove any specified return handler::

    stub::function<uint32_t(uint32_t)> some_function;
    some_function.set_return(5);

    uint32_t a = some_function(3);
    uint32_t b = some_function(4);

    assert(a == 5);
    assert(b == 5);

    // Return how many calls where made
    assert(some_function.calls() == 2);

    some_function.clear_calls();

    // Return true if no calls were made
    assert(some_function.calls() == 0);

    // We can continue to call the function
    uint32_t c = some_function(1);

    assert(c == 5);

Get the arguments of a specific function call
.............................................

If you are interested in manually inspecting the arguments passed to a
function call this can be done using the ``call_arguments(uint32_t)``
function.

::

    stub::function<void(uint32_t,uint32_t)> function;

    function(3,4);
    function(4,3);
    function(2,6);

Internally the arguments are stored in a ``std::tuple`` so in this
case it would be ``std::tuple<uint32_t,uint32_t>``. Lets compare the
arguments of the second call:

::

   auto a = function.call_arguments(1);
   auto b = std::make_tuple(4,3);

   assert(a == b);

**Note:** You should use the "unqualified and decayed types" of the function
arguments. This means that if you have a function
``stub::function<void(const uint32_t&>`` then the stub library will store
the argument passed in an ``uint32_t`` instead of a ``const
uint32_t&``. So our comparison should use ``std::tuple<uint32_t>``. If you use
``std::make_tuple(...)`` to build the your expectation this should happen
automatically (so you don't have to worry about it).

You can find more information about unqualified types `here
<http://stackoverflow.com/questions/17295169>`_ and `here
<http://bit.ly/1Markab>`_.

Ignore specific arguments of a function call
............................................

Sometimes it is useful to ignore specific arguments to a function call. They may
be internally computed or just in general not interesting when testing for
correctness.

::

    stub::function<void(uint32_t,uint32_t)> function;

    function(3U,4U);
    function(4U,3U);

    // Is matched by:
    bool works = function.expect_calls()
        .with(stub::ignore(), 4U)
        .with(4U, stub::ignore())
        .to_bool();

    assert(works);

Check if specific arguments are values other than null
......................................................

Sometimes it is useful to check if specific arguments to a function call are
not null.
If a pointer given to a function is internally computed it can be impossible or
complex to know what the correct value is. There for not_nullptr can be used
when that is the only value not allowed.

::

    stub::function<void(uint8_t*, uint32_t)> function;

    std::vector<uint8_t> buffer(1);
    function(buffer.data(), buffer.size());

    // Is matched by:
    bool works = function.expect_calls()
        .with(stub::not_nullptr(), 1U)
        .to_bool();

    assert(works);

Comparing custom arguments
..........................

The default behavior for the ``expect_calls()`` function is to
compare arguments passed though the ``with(...)`` function to the
actual arguments using ``operator==(...)``. However,
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

    stub::function<void(const cup&)> function;

    function(cup{2.3});
    function(cup{4.5});

    assert(function.expect_calls()
        .with(stub::make_compare([](auto& c){return c.m_volume == 2.3;}))
        .with(stub::make_compare([](auto& c){return c.m_volume == 4.5;}))
        .to_bool());

In this case we are using a c++14 lambda function as comparison
function.

As another example use a custom comparison for objects that do have
``operator==(...)`` but where we have custom equality criteria.

In this case we consider two ``std::pair<uint32_t,uint32_t>`` objects
equal if their second element is equal. To do this with the stub
library we need to provide a custom comparison function.

::

    using element = std::pair<uint32_t, uint32_t>;

    auto expect = [](uint32_t expected, const element& actual) -> bool
    { return expected == actual.second; };

    stub::function<void(const element&)> function;
    function(element(2,3));
    function(element(20,3));

    using namespace std::placeholders;
    // We have called the function more than once
    assert(false == function.expect_calls()
           .with(stub::make_compare(std::bind(expect, 3, _1))).to_bool());

    // Works since we only match the second value of the pair
    assert(true == function.expect_calls()
           .with(stub::make_compare(std::bind(expect, 3, _1)))
           .with(stub::make_compare(std::bind(expect, 3, _1))).to_bool());

    // Without the custom comparison it fails
    assert(false == function.expect_calls()
           .with(element(1,3))
           .with(element(2,3)).to_bool());

Building an Expectation
.......................
If we have many function calls it can be tedious to setup an expectation
inline:

::

    stub::function<void(uint32_t)> some_function;

    // Call the function
    for (uint32_t i = 0; i < 10; ++i)
    {
        some_function(i);
    }

    // Check the expectation.
    assert(some_function.expect_calls()
        .with(0U)
        .with(1U
        .with(2U)
        .with(3U)
        .with(4U)
        .with(5U)
        .with(6U)
        .with(7U)
        .with(8U)
        .with(9U));

Instead an expectation can be built by storing it as a variable and calling the
``with`` member function:

::

    stub::function<void(uint32_t)> some_function;

    auto some_function_expectation = some_function.expect_calls();

    // Call the function and setup expectation
    for (uint32_t i = 0; i < 10; ++i)
    {
        some_function(i);
        some_function_expectation.with(i);
    }

    // Check the expectation.
    assert(some_function_expectation);

Function return values
----------------------

We can also define a ``stub::function`` which returns a value:

::

    stub::function<bool(uint32_t)> some_function;

Here we have to specify what return value we expect:

::

    some_function.set_return(true);

    bool a = some_function(23);
    bool b = some_function(13);

    assert(a == true);
    assert(b == true);

Or alternatively we can set multiple return values:

::

    stub::function<uint32_t()> some_function;

    some_function.set_return(4U,3U);

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

    stub::function<uint32_t()> some_function;
    some_function.set_return(3U);

    uint32_t a = some_function();
    uint32_t b = some_function();
    uint32_t c = some_function();

    assert(a == 3U && b == 3U && c == 3U);

This behavior can be change by calling ``no_repeat()`` in which case
the return_handler can only be invoked once per return value
specified:

::

    stub::function<uint32_t()> some_function;
    some_function.set_return(1U).no_repeat();

    uint32_t a = some_function();
    // uint32_t b = some_function(); // <---- Will crash

    some_function.set_return(1U,2U,3U).no_repeat();

    uint32_t e = some_function();
    uint32_t f = some_function();
    uint32_t g = some_function();
    // uint32_t h = some_function(); // <---- Will crash

    assert(a == 1U && e == 1U && f == 2U && g == 3U);

In addition to the functionality shown in this example the
``stub::function`` object provides a couple of extra functions for
checking the current state. See the src/stub/function.hpp header for more
information.

For more information on the options for return values see the
src/stub/return_handler.hpp

Using stub with template arguments
----------------------------------

One place where stub works well is when testing policy classes or template code.

Member function
...............

As a small example, say we have the following::

    struct paper
    {
        // Call the print function on the printer object
        template<class Printer>
        void print(Printer& printer)
        {
            printer.print("Hello world");
        }
    };

Lets define a ``Printer`` object that we can use to test the behaviour of a
`paper` object::

    // Test stub printer object
    struct printer
    {
        stub::function<void(std::string)> print;
    };

Our unit test code could now look something along the lines of::

    printer printer;
    paper hello;

    hello.print(printer);

    assert(printer.print.expect_calls()
        .with("Hello world")
        .to_bool());


Static member function
......................

If our ``paper`` class was invoking a static method on the the ``Printer`` type
then our test code could look as follows::

    struct static_paper
    {
        // Call the static print function on the Printer type
        template<class Printer>
        void print()
        {
            Printer::print("Hello world");
        }
    };

Define our static printer object::

    struct static_printer
    {
        static stub::function<void(std::string)> print;
    };

    // Definition of the static stub
    stub::function<void(std::string)> static_printer::print;

The unit test code::

    static_paper hello;

    hello.print<static_printer>();

    assert(static_printer::print.expect_calls()
        .with("Hello world")
        .to_bool());

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
