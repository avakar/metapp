[![Build Status](https://travis-ci.org/avakar/metapp.svg?branch=master)](https://travis-ci.org/avakar/metapp)

# Meta++

A small header-only C++ library for manipulating lists of types.

## Installation

Clone anywhere and make "include" directory visible. If you're using CMake, you can link against avakar_metapp target and the include directories will be made available automatically.

## Getting started

Lists of types are represented using `avakar::meta::list` template.

    #include <avakar/meta.h>
    namespace meta = avakar::meta;
    
    using L = meta::list<T_0, T_1, /*...*/, T_n>;

Lists can be empty or contain repeated elements. Note that the `list` class is incomplete and cannot be instantiated. There is a collection of metafunctions one would expect to have to manipulate lists. 

  * `length<L>`: returns `n`.
  * `contains<T, L>`: returns true if there is an `i` such that `T_i == T`.
  * `contains_unique<T, L>`: returns true if there is exactly one `i` such that `T_i == T`.
  * `first_index_of<T, L>`: returns the first `i` such that `T_i == T`. The class is incomplete if `!contains<T, L>`.
  * `index_of<T, L>`: returns `i` such that `T_i == T`. Incomplete if `!contains_unique<T, L>`.
  * `sub<L, i>`: returns `T_i`.
  * `concat<L_0, L_1, ..., L_m>`: returns a concatenation of lists `L_i`. Arguments that are not `list` are wrapped in one; as such this metafunction also works as an append.

## Choosing a type at runtime

The major reason for the library is the function `visit`, which accepts a list as a template argument, and an index and a visitor function as its regular arguments. Visit calls the visitor with an argument of type `list_item<T_i, i>`.

    void * storage = /*...*/;

    using L = meta::list<int, std::string>;
    size_t index = 1;

    // prints the content of `storage` as if it contained a std::string
    meta::visit<L>(index, [storage](auto m) {
        std::cout << *static_cast<decltype(m) *>(storage) << "\n";
    });
