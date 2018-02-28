# Argo
A simple command line argument parsing header file
Based on argparse, but made to be a header-only library.
The library was designed to be as simple to use as possible, but the code isn't the nicest at the moment, but I plan to clean it up over time.

## Usage
Include the header file `argo.h` then create an array of `argo_arg`s and finally call the function `argo_parse` and boom! You're done!

I've added an example program showing the most basic usage of the library in [`examples/example.c`](examples/example.c)
