Lab Work 2
=============

Prerequisites
-------------

- GCC: Ensure you have the GNU Compiler Collection (GCC) installed on your system.


Compiling the Project
----------------------

To compile the entire project, including different tasks, you can simply run:

    make

Compiling Individual Tasks
--------------------------

You can also compile specific tasks individually. For example, to compile TASK0, use:

    make /l1_cache/TASK1

To compile other tasks, replace TASK0 with the appropriate task name (TASK1, TASK2, or TASK3).

Debug and Optimization
-----------------------

- By default, the code is compiled with debugging symbols (-g). To deactivate debugging symbols, run `make DEBUG=no`.
- By default, the code is compiled with optimization level 3 (-O3). To deactivate optimizations, run `make OPTIM=no`.

Running the Code
----------------

To run the compiled executables, use the following commands:

- For TASK0:
    ./prof_l1_cache/TASK0

- For TASK1:
    ./l1_cache/TASK1

- For TASK2:
    ./l2_cache/TASK2

- For TASK3:
    ./l2_cache_2w/TASK3

Cleaning Up
-----------

To remove the compiled object files and executables, run:

    make clean

License
-------

This project is licensed under the  GNU GENERAL PUBLIC LICENSE - see the LICENSE file for details.

Acknowledgments
---------------

Utils where provided by DEI/IST/ULisboa (at Operating Systems course)

