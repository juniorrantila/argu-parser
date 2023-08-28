# argu-parser

**argu-parser** is minimal C++17 CLI argument parser

## Usage

First, install the wrap file:

    meson wrap install argu-parser

Then add the following line to your project's `meson.build`:

```meson

argu_parser_dep = subproject('argu-parser').get_variable('argu_parser_dep')

```

### Example

```cpp

#include <argu/parser.h>
#include <stdio.h>

int main(int argc, char const* argv[])
{
    auto argument_parser = argu::parser(argc, argv);

    char const* foo = nullptr;
    argument_parser.add_positional_argument("foo", "Explain foo.", [&](char const* arg) {
        foo = arg;
    });

    bool bar = false;
    argument_parser.add_flag("--bar", "-b", "Explain bar.", [&] {
        bar = true;
    });

    char const* baz = nullptr;
    argument_parser.add_option("--baz", "-bz", "placeholder value", "Explain baz.", [&](char const* arg) {
        baz = arg;
    });

    if (auto error = argument_parser.run()) {
        fprintf(stderr, "%s\n", error->c_str());
        return 1;
    }

    printf("foo: %s\nbar: %d\nbaz %s\n", foo, bar, baz);

    return 0;
}

```

## Build instructions

### Setup:

```sh

meson setup build

```

### Build:

```sh

ninja -C build

```
