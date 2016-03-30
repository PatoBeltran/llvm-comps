#Simple LICM Pass
##Asignment 3

###Author:
Patricio Beltran

[patriciobeltran@utexas.edu](mailto:patriciobeltran@utexas.edu)

**PMB784**

###Useful information:
if `opt` is ran using the `-debug` and `-stats` flags, the pass will output information about each instruction that tries to hoist and the statistics of the total amount of hoisted expressions.

####What's included
In the project directory you will find:

```
|- tests				C and human-readable LLVM IR files for different test cases
 - CMakeLists.txt		CMake compiling file
 - LICM.cpp			    Source code for pass
 - LICM.exports		    File needed by compiling file (based of Hello Pass)
 - README.md			Program description
```

####Materials used while researching
- http://llvm.org/docs/ProgrammersManual.html

**Extensive use of `DoxyGen` documentation**

- http://llvm.org/docs/doxygen/html/

