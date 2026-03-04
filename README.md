# xv6 scheduler profiler

A modified version of xv6 that tracks how long processes wait to get scheduled. basically I added instrumentation to the kernel so you can see dispatch latency, runnable wait times, and context switch overhead for every process. I am working on implementing MLFQ rn and later my own scheduling algorthims. 

The idea is: every time the scheduler picks a process to run, I record how long that process was sitting around waiting. Then you can query those stats with a syscall and print them out.

## why
Og xv6 has 0 visibility into scheduling behavior. I want to implement my own scheduling algorthims and compare against MLFQ, so I needed something to test them. This fixes that by adding lightweight per-process counters that track.

- **dispatch latency**: how long between a process becoming RUNNABLE and actually getting the CPU
- **total wait time**: total time spent in the RUNNABLE queue
- **context switch time**: time measured around `swtch()` calls
- all with O(1) overhead per event (just a few integer ops, no extra locks)

## how it works

Every time a process state changes to RUNNABLE, I stamp the current tick count. When the scheduler picks that process, I compute `ticks - runnable_ts` to get the dispatch latency. I also measure time around `swtch()` in the scheduler loop.

All stats live  in `struct proc` (no extra data structures needed).

## demo

### basic run (no load)

```
$ schedstat
pid   disp_cnt   disp_avg   disp_max   wait_sum   swtch_cnt   swtch_sum
1     37         0          1          3          37          36
2     59         0          1          12         59          37
6     35         0          1          4          34          42
```

- **pid 1** = `init`, **pid 2** = `sh`, **pid 6** = `schedstat` itself
- dispatch avg is 0 because processes get scheduled in under 1 tick (~10ms)

### under load (two stressfs processes)

```
$ stressfs &; stressfs &; sleep 100; schedstat
```

you'll see higher `disp_max` and `wait_sum` values since processes compete for the cpu.

### what the columns mean

- **pid**: the pid...
- **disp_cnt**: how many times this process got dispatched (RUNNABLE -> RUNNING)
- **disp_avg**: average dispatch latency in ticks
- **disp_max**: worst/max dispatch latency in ticks
- **wait_sum**: total ticks spent sitting in RUNNABLE state
- **swtch_cnt**: number of context switches
- **swtch_sum**: total ticks measured around swtch() calls (includes process runtime)

```bash
#boot xv6 using docker
#build
make qemu-nox
$ schedstat