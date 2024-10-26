prop_react library v1.0

Introduction

This is с++ header-only library is designed to describe the interaction between different properties, which is commonly used in GUI design. The convenient syntax makes it easier to describe dependencies between different properties.

Usage example 1:

	property<int> first_prop;
	property<int> second_prop; // second_prop always equal first_prop * 5

	first_prop.add_subscriber(second_prop,
		[&](const property<int>& prop)
		{
			second_prop = prop.val() * 5;
		}
	);

	first_prop = 5;
	assert(first_prop.val() == 5);
	assert(second_prop.val() == 25);

Usage example 2:

	property<int> first_prop;
	property<int> second_prop;

	first_prop.add_subscriber(second_prop,
		[&second_prop](const auto& f)
		{
			second_prop = f;
		}
	);

	second_prop.add_subscriber(first_prop,
		[&first_prop](const auto& s)
		{
			first_prop = s;
		}
	);

	first_prop = 20;
	assert(first_prop.val() == 20);
	assert(second_prop.val() == 20);

	second_prop = 30;
	assert(first_prop.val() == 30);
	assert(second_prop.val() == 30);

Requirements

 OS
   - Linux
   - Windows
   - MacOS

 complier
   - C++ 20 or later (g++, clang++, msvc, etc.)	
   - Use cmake 3.28 to build this project (Optional)

Test

	See test\property_test.cpp

License

   - Free