# regex-engine

A Regular expression engine for C++

## Why did I build it?

I wanted to see if I could. I got to see practical use cases for certain data structures too, so that's a win.

## Want to use the engine in your project?

Simple, just copy the `tokenizer.hpp`, and the `regexengine.hpp` header files to your project, and import the `regexengine.hpp` header file into the code you want to use it in.

```cpp
#include "regexengine.hpp"
```

## How can you use the engine?

First, create an instance of the engine, replace `"re-pattern"` with the regular expression you want to match strings with:
```cpp
RegexEngine engine("re-pattern");
```

Then, call the `match` method and pass in the string you want to match the pattern with:
```cpp
bool result = engine.match("string");
```

That's all you need to use it.

## regex-engine is missing a feature?

Raise an issue to describe the feature. Links, resources, or just the name of the feature would be helpful. I haven't used regular expression that deeply to know everything you can and can't do with it.

## regex-engine is matching strings that shouldn't be matched?

Raise an issue too. It is not easy to check for every possible edge case.

## You want to contribute to the project?

There's no contribution guide and this is just a hobby project. So the only thing I have to say is feel free to raise issues, make pull-requests, or fork anything. Just be nice :)