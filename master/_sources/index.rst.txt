Welcome to Stub's documentation!
================================


.. image:: http://buildbot.steinwurf.dk/svgstatus?project=stub
    :target: http://buildbot.steinwurf.dk/stats?projects=stub

stub is a simple C++ library which is meant to provide simple helper
functions to ease sometimes tedious testing tasks.

stub is especially helpful when testing template functions which
takes hard-to-construct objects as parameters.
With stub the task of making such tests short and self-contained, becomes much
simpler. See this example of how ``function_to_test`` is tested:

::

    #include <stub/function.hpp>
    #include <cassert>

    // Function to test
    template<class T>
    void function_to_test(T& t)
    {
        t.some_function(4);

        if (t.some_check())
        {
            t.do_something_else(false);
        }
    }

    // Dummy object to test function with
    struct dummy
    {
        stub::function<void(int)> some_function;
        stub::function<bool()> some_check;
        stub::function<void(bool)> do_something_else;
    };

    void main()
    {
        // Construct object
        dummy obj;

        // Setup return value
        obj.some_check.set_return(true);

        // Run function
        function_to_test(obj);

        // Check calls
        assert(obj.some_function.expect_calls().with(4));
        assert(obj.some_check.calls() == 1);
        assert(obj.do_something_else.expect_calls().with(false));
    }

.. toctree::
   :maxdepth: 2
   :glob:

   api
   usage

