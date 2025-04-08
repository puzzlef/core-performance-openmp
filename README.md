Measure the performance of individual cores with OpenMP.

You can use this program to measure the performance of individual CPU cores of a system. It works by running a pleasingly parallel workload on each core and measuring the time it takes to complete. The workload is a simple loop that find the sum of all the elements of an array (1 billion 64-bit floating point numbers by default). The program is written in C++ and uses OpenMP to parallelize the workload. To prevent the operating system from moving threads between cores, the program uses the OpenMP affinity API to pin each thread to a specific core.

> [!NOTE]
> You can just copy `main.sh` to your system and run it. \
> For the code, refer to `main.cxx`.

<br>

```bash
$ ./main.sh
# OMP_NUM_THREADS=64
# {run=000, thread=000, node=0, core=000, time=2407.0969ms, flops=4.1544e+08}
# {run=000, thread=001, node=0, core=032, time=2407.0779ms, flops=4.1544e+08}
# {run=000, thread=002, node=1, core=001, time=2407.0649ms, flops=4.1544e+08}
# ...
```

<br>

I run this program on nodes 1, 2, and 4 of our cluster - no core-specific faults are present.

[![](https://i.imgur.com/tcbC45w.png)][sheets-node01]
[![](https://i.imgur.com/xywu4Qx.png)][sheets-node02]
[![](https://i.imgur.com/uxjtxtG.png)][sheets-node04]

Also below is runtime stabilisation plot (I perform 100 runs of summing a billion elements on each core).

[![](https://i.imgur.com/OS0WT1j.png)][sheets-node01]
[![](https://i.imgur.com/9SJYVWV.png)][sheets-node02]
[![](https://i.imgur.com/SCA5HvN.png)][sheets-node04]


<br>
<br>


## References

- [OpenMP topic: Affinity - Parallel Programming for Science Engineering: The Art of HPC, volume 2 by Victor Eijkhout](https://theartofhpc.com/pcse/omp-affinity.html#OpenMPthreadaffinitycontrol)
- [c - Openmp. How to retrieve the core id in which a thread is running - Stack Overflow](https://stackoverflow.com/questions/22492886/openmp-how-to-retrieve-the-core-id-in-which-a-thread-is-running)
- [c++ - OpenMP and CPU affinity - Stack Overflow](https://stackoverflow.com/questions/8325566/openmp-and-cpu-affinity)
- [c++ - Set CPU affinity when create a thread - Stack Overflow](https://stackoverflow.com/questions/24645880/set-cpu-affinity-when-create-a-thread)
- [Controlling OpenMP Thread Affinity](https://www.openmp.org/spec-html/5.0/openmpsu36.html)
- [Thread Affinity Interface](https://www.intel.com/content/www/us/en/docs/dpcpp-cpp-compiler/developer-guide-reference/2023-0/thread-affinity-interface.html)
- [OpenMP Application Program Interface Version 3.1](https://www.openmp.org/wp-content/uploads/OpenMP3.1.pdf)
- [Embarrassingly parallel - Wikipedia](https://en.wikipedia.org/wiki/Embarrassingly_parallel)

<br>
<br>


[![](https://img.youtube.com/vi/yqO7wVBTuLw/maxresdefault.jpg)](https://www.youtube.com/watch?v=yqO7wVBTuLw)<br>
[![ORG](https://img.shields.io/badge/org-puzzlef-green?logo=Org)](https://puzzlef.github.io)
[![DOI](https://zenodo.org/badge/717844333.svg)](https://zenodo.org/doi/10.5281/zenodo.10208366)
![](https://ga-beacon.deno.dev/G-KD28SG54JQ:hbAybl6nQFOtmVxW4if3xw/github.com/puzzlef/core-performance-openmp)

[sheets-node01]: https://docs.google.com/spreadsheets/d/1wMAnlMzDx6l54KRVJfEBNZdXVdGN0dhltxPMDOzPJu8/edit?usp=sharing
[sheets-node02]: https://docs.google.com/spreadsheets/d/1B-ymeIhSyIpu9U1y-O8uifIppIKufiGmRQwYLwlCq9c/edit?usp=sharing
[sheets-node04]: https://docs.google.com/spreadsheets/d/1ZTJ77bsp3tsfTEyCFqR_zqzcapJHJATmH4GlEgB4rgI/edit?usp=sharing
