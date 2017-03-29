News for stub
=============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* tbd

6.0.0
-----
* Major: Upgrade to waf-tools 4
* Minor: Upgrade to gtest 4

5.0.1
-----
* Patch: Fixed memory leak in compare_call due to missing virtual destructor.

5.0.0
-----
* Major: Renamed stub::call to stub::function as this better reflects its true
  purpose.
* Major: Change the approach to providing custom comparisons of function
  arguments. This is now done by passing stub::compare to
  expect_calls().with(...).
* Minor: Added the ability to ignore specific arguments when comparing function
  calls in expect_calls().
* Minor: Added the ability to clear function objects
* Major: Removed the need to use ``std::initializer_list`` when specifying
  multiple return values.

4.0.1
-----
* Patch: Fix printing of pointers. We rely on ``std::ostream::operator<<``
  for printing pointer values. This caused problems with raw binary data
  (char* or uint8_t*) since ``std::ostream::operator<<`` tries to print
  those as zero-terminated c-style strings. Instead we now always print the
  address pointed to rather than trying to print the actual data.

4.0.0
-----
* Major: Upgrade to waf-tools 3
* Minor: Upgrade to gtest 3

3.0.0
-----
* Major: Removed the ``repeat`` member function as it does not have a proper use
  case.
* Major: Removed the ``ignore`` member function as it does not have a proper use
  case.

2.3.0
-----
* Minor: Added the ``to_bool`` function - a more verbose version of converting
* Patch: Added assert to ensure that expectations has been setup prior to
  the expectation to a boolean value.
* Patch: Added ``assert`` to ensure that expectations has been setup prior to
  evaluation.

2.2.0
-----
* Minor: Added ``buildbot.py`` for coverage reports.
* Minor: Added methods allowing a ``stub::function`` object to be printed to an
  ``std::ostream``.

2.1.2
-----
* Patch: Fixed MSVC type conversion warnings.

2.1.1
-----
* Patch: Fix version define.

2.1.0
-----
* Minor: Added version define.

2.0.1
-----
* Bug: Added const modifier to the ``stub::function::expect_calls()``
  functions. They do not modify the object so they should be ``const``.
* Bug: Updated documentation to specify in which order arguments are
  passed to a custom predicate function.

2.0.0
-----
* Major: Replaced the ``called_once_with(...)``, ``called_with(...)`` and
  ``has_calls(...)`` functions with the ``expect_calls(...)`` functions. The
  ``expect_calls(...)`` functions provide the same functionality as the
  replaced functions, but are more convenient to use.

1.1.0
-----
* Minor: Added the ``call_arguments(..)`` function to access a specific
  function's arguments

1.0.1
-----
* Bug: Add missing assert checking that the ``return_handler`` has actual
  values to return

1.0.0
-----
* Initial release of the stub library.
