# VWAP Options pricer

I sympathize the fact that almost all quant codes are proprietary. I decided to release codes from my honour thesis. 
The material in this repository belongs to my honours project in Mathematical Finance during 2011 
on the topic of VWAP Options pricing under a matching-moment approach. Monte Carlo simulations was performed in validating
the analytic prices computed from our formulae.
I was working based on the implementation of Dr Tim Ling's option pricing header that use GNU gsl library and Boost.
In the current version, GNU gsl has been fully removed the standard library header <random>.

If you turn out to make any use of it, either on your project or quant work. Please do cite the repository :)
The codes clearly not so pretty. The main point here is problem solving. For any suggestions, feel free to drop me an 
email or over twitter or  cppslack@leanne.

## Compiler requirement

* Clang 10 (This is compiler I use over my CLION IDE on a Linux UBUNTU 20.4. One can certainly go with GCC or MSVC)

## Dependency

* A recent version of Boost

* CMake ( > 3.00 )



