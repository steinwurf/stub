News for stub
=============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

2.0.0
-----
* Major: Replaced the called_once_with(...), called_with(...) and
  has_calls(...) functions with the expect_calls(...) functions. The
  expect_calls(...) functions provide the same functionality as the
  replaced functions, but are more convenient to use.

1.1.0
-----
* Minor: Added the call_arguments(..) function to access a specific
  call's arguments

1.0.1
-----
* Bug: Add missing assert checking that the return_handler has actual
  values to return

1.0.0
-----
* Initial release of the stub library.
