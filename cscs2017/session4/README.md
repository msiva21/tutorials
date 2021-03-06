
class: center, middle

# Hello World!
## Options and Running Applications

[Overview](..)

Previous: [Building HPX](../session3)

???
[Click here to view the Presentation](https://stellar-group.github.io/tutorials/cscs2016/session4/)

---
## General

* HPX comes with a large set of options you can pass through the command line
* We will cover a few
* [Read the docs!](http://stellar-group.github.io/hpx/docs/html/hpx/manual/init.html)

---
## Overview
### Command line parameters

```INI
$ ./bin/hello_world --hpx:help

Usage: unknown HPX application [options]:

HPX options (allowed on command line only):
  --hpx:help [=arg(=minimal)]           print out program usage (default: this
                                        message), possible values: 'full'
                                        (additionally prints options from
                                        components)
  --hpx:version                         print out HPX version and copyright
                                        information
  --hpx:info                            print out HPX configuration information
  --hpx:options-file arg                specify a file containing command line
                                        options (alternatively: @filepath)

HPX options (additionally allowed in an options file):
  --hpx:run-agas-server                 run AGAS server as part of this runtime
                                        instance
  --hpx:run-hpx-main                    run the hpx_main function, regardless
                                        of locality mode

... And much more ...
```

---
## Overview
### The HPX INI Config

```INI
$ ./bin/hello_world --hpx:dump-config
Configuration after runtime start:
----------------------------------
============================
  [application]
  [hpx]
    'affinity' : 'pu'
    'bind' : 'balanced'
    'cmd_line' : './bin/hello_world --hpx:dump-config'
    'component_path' : '$[hpx.location]:$[system.executable_prefix]' -> '/apps/daint/hpx/0.9.99/gnu_530/debug:/users/heller/tutorials/examples/build/debug'
    'component_path_suffixes' : '/lib/hpx:/bin/hpx'
    'cores' : '1'
    'expect_connecting_localities' : '1'
    'finalize_wait_time' : '-1.0'
    'first_pu' : '0'

... And much more ...
```
* Can be set with `-I...`
    * Example: `-Ihpx.bind=compact`

---
## Dumping Version Information
### `--hpx:version`

```
$ ./bin/hello_world --hpx:version

HPX - High Performance ParalleX
A general purpose parallel C++ runtime system for distributed applications
of any scale.

Copyright (c) 2007-2016, The STE||AR Group,
http://stellar-group.org, email:hpx-users@stellar.cct.lsu.edu

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

Versions:
  HPX: V1.0.0-trunk (AGAS: V3.0), Git: 4f281039e4
  Boost: V1.61.0
  Hwloc: V1.11.0
  MPI: MPICH V3.2rc1, MPI V3.1

Build:
  Type: debug
  Date: Sep 28 2016 11:42:06
  Platform: linux
  Compiler: GNU C++ version 5.3.0 20151204 (Cray Inc.)
  Standard Library: GNU libstdc++ version 20151204
  Allocator: JEMALLOC
```

---
## Controlling CPU binding
### General

* Use `--hpx:print-bind` to show the selected bindings
* Use `--hpx:threads=N` to select the number of threads per locality
* Use `--hpx:cores=N` to select the number of cores

---
## Controlling CPU binding
### Binding the HPX worker threads to specific CPU Cores

.left-column[
* `--hpx:bind=...`
    * `compact`
    * `scatter`
    * `balanced`
    * `numa-balanced`
]
.right-column[
![Thread affinities](images/affinities.png)
]

---
## Controlling CPU binding
### Binding the HPX worker threads to specific CPU Cores

* Bind Description:

.left-column[
```
mappings:
    distribution
    mapping(;mapping)*

distribution:
    'compact'
    'scatter
    'balanced'
    'numa-balanced'

mapping:
    thread-spec=pu-specs

thread-spec:
    'thread':range-specs

pu-specs:
    pu-spec(.pu-spec)*
```
]
.right-column[

```

pu-spec:
    type:range-specs
    ~pu-spec

range-specs:
    range-spec(,range-spec)*

range-spec:
    int
    int-int
    'all'

type:
    'socket' | 'numanode'
    'core'
    'pu'
```
]

---
## Controlling CPU binding
### Binding the HPX worker threads to specific CPU Cores

* Use `lstopo` to get an idea of your CPU topology
* Bind Description, Examples:

```
# 4 worker threads, running on the first 4 CPUs using the first Hyperthread
$ ./bin/hello_world --hpx:threads=4 --hpx:print-bind \
    --hpx:bind="thread:0-3=core:0-3.pu:0"
****************************************************************
locality: 0
   0: PU L#0(P#0), Core L#0(P#0), Socket L#0(P#0), Node L#0(P#0)
   1: PU L#1(P#2), Core L#1(P#1), Socket L#0(P#0), Node L#0(P#0)
   2: PU L#2(P#4), Core L#2(P#2), Socket L#0(P#0), Node L#0(P#0)
   3: PU L#3(P#6), Core L#3(P#3), Socket L#0(P#0), Node L#0(P#0):
```

```
# 4 worker threads, running on the first 2 run on NUMA domain 0, using the second
# and third core, worker thread 3 runs on the second NUMA on the 7th core, worker
# thread 4 on the 2nd NUMA and 8th worker core, using the first Hyperthread
$ ./bin/hello_world --hpx:threads=4 --hpx:print-bind \
    --hpx:bind="thread:0-1=socket:0.core:0-2.pu:0;thread:2-3=socket:1.core:6-7.pu:0"
****************************************************************
locality: 0
   0: PU L#0(P#0), Core L#0(P#0), Socket L#0(P#0), Node L#0(P#0)
   1: PU L#1(P#2), Core L#1(P#1), Socket L#0(P#0), Node L#0(P#0)
   2: PU L#13(P#11), Core L#13(P#5), Socket L#1(P#1), Node L#1(P#1)
   3: PU L#14(P#13), Core L#14(P#6), Socket L#1(P#1), Node L#1(P#1)
```

* Note: This was run on the head node of daint. Try it on the compute nodes!

---
## Distributed Runs
### General

* HPX itself doesn't come with a dedicated launcher (like `mpirun`)
    * `--hpx:localities=N`: using N localities
    * `--hpx:locality=i`: ith locality
    * `--hpx:agas=host`: The root (locality 0) process host
    * `--hpx:hpx=host`: The host this HPX process is listening on
* Manual setup tedious:
    * Start N processes, set correct parameters...

---
## Distributed Runs
### `hpxrun.py`

* Small wrapper script to allow easy setup for distributed runs:

```
  -h, --help            show this help message and exit
  -l LOCALITIES, --localities=LOCALITIES
                        Number of localities to run (environment variable
                        HPXRUN_LOCALITIES
  -t THREADS, --threads=THREADS
                        Number of threads per locality (environment variable
                        HPXRUN_THREADS)
  -p PARCELPORT, --parcelport=PARCELPORT
                        Which parcelport to use (Options are: ibverbs, ipc,
                        mpi, tcp) (environment variable HPXRUN_PARCELPORT
  -r RUNWRAPPER, --runwrapper=RUNWRAPPER
                        Which runwrapper to use (Options are: none, mpi, srun)
                        (environment variable HPXRUN_ (environment variable
                        HPXRUN_RUNWRAPPER)
  -e EXPECTED, --expected=EXPECTED
                        Expected return codes of all invoked processes
                        (environment variable HPXRUN_EXPECTED)
  -v, --verbose         Verbose output (environment variable HPXRUN_VERBOSE)
```
* Pro tip : use `ctest -R test_name -N -V` to get command line params

---
## Batch environments
### General

* The HPX startup routines can detect Batch systems
* Extracts the needed information to setup the Application:
    * Number of threads
    * Number of localities
    * Host names
* Supported environments: SLURM, PBS, ALPS, MPI
* Internal vars are setup automatically
```c++
typedef hpx::id_type id_t; // just to make the lines fit on slide
//
id_t                    here = hpx::find_here();
uint64_t                rank = hpx::naming::get_locality_id_from_id(here);
std::string             name = hpx::get_locality_name();
uint64_t              nranks = hpx::get_num_localities().get();
std::size_t          current = hpx::get_worker_thread_num();
std::vector<id_t>    remotes = hpx::find_remote_localities();
std::vector<id_t> localities = hpx::find_all_localities();
```
* This stuff is still setup if you don't use slurm, but
---
## Batch environments
### SLURM

* HPX startup parses the various Environment Variables set by SLURM:
    * Number of Nodes to use
    * Number of Threads to use
    * List of nodes of the allocation
* Use `salloc` to get an allocation, `srun` to start the application
* Useful parameters
    * `-n`: Number of Processes
    * `-N`: Number of Nodes to distribute Processes on
    * `-c`: Number of cores
    * `--hint=nomultithread`: Turn of multithreading

```sh
srun -n 4 /scratch/snx1600/biddisco/build/clang/linalg/bin/check_cholesky_d
-Ihpx.max_busy_loop_count=500 --hpx:bind=numa-balanced
--hpx:high-priority-threads=6  --hpx:threads=36 --hpx:run-hpx-main
--hpx:print-counter /runtime{locality#*/total}/memory/resident
--hpx:print-counter=/arithmetics/add@/papi{locality#0/worker-thread#*}/PAPI_L1_DCM
--hpx:print-counter-interval=100
--hpx:print-counter-destination=/dev/null
--col-proc=2 --row-proc=2 --size=10240 --nb=512 --use-pools --mpi-threads=2
--use-scheduler --hp-queues=6 --no-check
```

---
## Batch environments
### MPI

Note: The following only applies if you have the MPI parcelport compiled in

* Just use mpirun to start your distributed Application, we will use the number
 of ranks, and the rank to communicate with MPI routines.
* Read the documentation of your Supercomputer on how to launch MPI
 jobs.
* You usually want one process (==locality) per node!
* Note to Cray users:
    * If you have SLURM on your cray, srun will automatically activate MPI
    * If using plain ALPS, aprun has the same effect

---
## Distributed Runs
### Selecting parcelports

* `--hpx:list-parcel-ports`: Lists which parcelports are available and enabled
* Use the INI configuration to explicitly disable/enable Parcelports:
    * `-Ihpx.parcel.tcp.enable=0` will disable the TCP parcelport
    * `-Ihpx.parcel.mpi.enable=0`
    * `-Ihpx.parcel.libfabric.enable=0`

* Note libfabric stable but still experimental and only supported on daint
with assistence from JB
    * Users wanted for large scale runs of libfabric PP.

---
## Debugging options

* Attach a debugger:
    * `--hpx:attach-debugger`: This will stop the HPX application and wait for
        the debugger to be attached and the application being continued
```
PID: 116544 on daint103 ready for attaching debugger.
Once attached set i = 1 and continue
```
        * Note that this does not scale well.
        * N>4 = really tedious

    * `--hpx:attach-debugger=exception`: Stops the application if there was an exception
    * Use this every day - especially on machines like daint where you can
    ssh into a compute node.

* Logging:
    * `--hpx:debug-hpx-log`
* Debug command line parsing:
    * `--hpx:debug-clp`

---
## Performance Counters

* List all available performance counters:
    * `--hpx:list-counters`
    * Use this every day!
* Print counter:
    * `--hpx:print-counter counter`
    * This will print the counter once the application has been completed
* Set counter interval:
    * `--hpx:print-counter-interval time`
    * `--hpx:print-counter-destination=file`
        * or `/dev/null` is using APEX
* Print performance counters from your application:
    * `hpx::evaluate_active_counters(bool reset, char const* description)`

* Access a performance counter from code (blocking / using sync policy)
```c++
    performance_counter counter("<name>");
    std::cout << counter.get_counter<double>(hpx::launch::sync);
```

---
## HPX Application Startup
### `hpx::init`

```
#include <hpx/hpx_init.hpp>

int hpx_main()
{
    // Initiate HPX shutdown
    return hpx::finalize()
}

int main(int argc, char** argv)
{
    hpx::init(argc, argv);
}
```
* Init starts the runtime
    * `int main` is running on a OS thread
* return from init by calling finalize
    * `hpx_main` is running on an HPX thread

* **Warning** : By default, only locality 0 runs `hpx_main`

---
## Adding your own options
### Use C-style main

```
#include <hpx/hpx_init.hpp>

// Use regular "C-style-main" and parse non-consumed command line arguments
int hpx_main(int argc, char** argv)
{
    // Initiate HPX shutdown
    return hpx::finalize()
}
```
* Allow HPX to take (and hide) main
    * `hpx_main` is running on an HPX thread
    * I don't think you should use this!

* **Warning** : By default, only locality 0 runs `hpx_main`

---
## Adding your own options
### Using Boost.ProgramOptions

```
#include <hpx/hpx_init.hpp>

// Use Boost.ProgramOptions to retrieved non-consumed command line arguments
int hpx_main(boost::program_options::variables_map& vm)
{
    std::size_t Nx = vm["Nx"].as<std::size_t>();
    return hpx::finalize()
}

int main(int argc, char** argv)
{
    using namespace boost::program_options;

    options_description desc_commandline;
    desc_commandline.add_options()
        ("Nx", value<std::uint64_t>()->default_value(1024),
         "Elements in the x direction")
    hpx::init(desc_commandline, argc, argv);
}
```
* [Read the docs!](http://www.boost.org/doc/libs/release/doc/html/program_options.html)

---
## Adding your own options
### Using The HPX INI Config

```
#include <hpx/hpx_init.hpp>

int hpx_main()
{
    std::string val = hpx::get_config_entry("my.cool.option", "42");

    return hpx::finalize()
}

int main(int argc, char** argv)
{
    // Initialize and run HPX, this example requires to run hpx_main on all
    // localities. And an application specific setting
    std::vector<std::string> const cfg = {
        "hpx.run_hpx_main!=1",
        "my.cool.option!=yeha"
    };

    return hpx::init(argc, argv, cfg);
}
```

* Can be combined with an application specific `options_description` as well!

---
## Complex option setup
```c++
int main(int argc, char* argv[])
{
    boost::program_options::options_description desc_cmdline("Test options");
    desc_cmdline.add_options()
        ( "use-pools,u", "Enable advanced HPX thread pools and executors")
        ( "pool-threads,m",
          boost::program_options::value<int>()->default_value(1),
          "Number of threads to assign to custom pool");

    // HPX uses a boost program options variable map, but we need it before
    // hpx-main, so we will create another one here and throw it away after use
    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .allow_unregistered() // IMPORTANT
            .options(desc_cmdline)
            .run(),
        vm);

    if (vm.count("use-pools")) {
        use_pools = true;
    }
    pool_threads = vm["pool-threads"].as<int>();

    // Create the resource partitioner
    hpx::resource::partitioner rp(desc_cmdline, argc, argv);
```
* see examples
[named_pool_executor](../../examples/05_named_pool_executor/named_pool_executor.cpp)

---
## HPX Application Startup
### Alternative "replacing" main

* Alternative for providing `hpx_main` and calling `hpx_init`
* The HPX runtime gets set up in the background
* The application still accepts all regular HPX parameters

```
#include <hpx/hpx_main.hpp>

int main()
{
    return 0;
}
```

Or:

```
#include <hpx/hpx_main.hpp>

int main(int argc, char** argv)
{
    return 0;
}
```

---
class: center, middle
## Next

[Worked 2D Stencil Example - From Serial to Distributed](../session5)
