# Contribution Style Guide

When contributing to this code, there are a few style preferences that should be followed.

## When to use define vs constexpr

When defining pinout for hardware integrate, prefer `#define`.
When defining a constant, prefer `constexpr`.

## int const vs const int

When using the `const` keyword, it should be placed after the thing that is a constant.
e.g. `int const * const ptr` should read "ptr, is a const pointer to a const int.