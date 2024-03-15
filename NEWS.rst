News for stub
=============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Patch: Changed the container type for storing return values in `return_handler`
  from `std::vector` to `std::deque` to avoid a specific issue with `std::vector<bool>`.
  This change ensures safer and more predictable behavior when using STL algorithms.

7.1.0
-----
* Minor: Added the check function to allow a user to check if a function has
  been called with specific arguments. If not the check function will throw an
  exception.
* Minor: Added the side_effect function to allow a user to specify a function
  that will be called when a function is called.

7.0.0
-----
* Major: Use waf-tools 5.

6.5.0
-----
* Minor: Added CMake support.
* Minor: Updated waf.

6.4.0
-----
* Minor: Updating how docs are built
* Minor: Update waf build script

6.3.0
-----
* Minor: Adding documentation.

6.2.0
-----
* Minor: Added the ability to check if specific arguments are not null when
  comparing function calls in expect_calls().

6.1.0
-----
* Minor: Allow a user to clear only the function calls made.

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
