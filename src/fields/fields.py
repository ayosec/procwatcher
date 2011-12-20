
from generator import Builder

builder = Builder()
group = builder.group
field = builder.field
compf = builder.compf

Int = "%d"
Unsigned = "%u"
Char = "%c"
ULongLong = "%llu"
Long = "%ld"
UnsignedLong = "%lu"

# Descriptions was taken from /usr/include/proc/readproc.h

group("IDs")

field("tid",     Int, "task id, the POSIX thread ID (see also: tgid)")
field("ppid",    Int, "PID of parent process")
field("pgrp",    Int, "process group ID")
field("session", Int, "session ID")

group("Scheduling info")

field("state",     Char,         "process state (R: running, S: sleeping (interruptible), D: waiting (uninterruptible), Z: zombie, T: traced or stopped (on a signal), W: paging)")
field("nlwp",      Int,          "number of threads, or 0 if no clue")
field("priority",  Long,         "kernel scheduling priority")
field("nice",      Long,         "standard unix nice level of process")
field("rtprio",    UnsignedLong, "real-time priority")
field("sched",     UnsignedLong, "scheduling class")
field("processor", Int,          "current (or most recent?) CPU")

group("Times")

compf("%cpu",       "pcpu",      "CPU usage")
compf("timestamp",  "timestamp", "Timestamp of every printed line (in ms)")
field("utime",      ULongLong,   "user-mode CPU time accumulated by process")
field("stime",      ULongLong,   "kernel-mode CPU time accumulated by process")
field("cutime",     ULongLong,   "cumulative utime of process and reaped children")
field("cstime",     ULongLong,   "cumulative stime of process and reaped children")
field("start_time", ULongLong,   "start time of process -- seconds since 1-1-70")

group("Memory")

field("size",     Long,         "total virtual memory (as # pages)")
field("resident", Long,         "resident non-swapped memory (as # pages)")
field("share",    Long,         "shared (mmap'd) memory (as # pages)")
field("trs",      Long,         "text (exe) resident set (as # pages)")
field("drs",      Long,         "data+stack resident set (as # pages)")
field("vm_size",  UnsignedLong, "equals 'size' (as kb)")
field("vm_lock",  UnsignedLong, "locked pages (as kb)")
field("vm_rss",   UnsignedLong, "equals 'rss' and/or 'resident' (as kb)")
field("vm_data",  UnsignedLong, "data only size (as kb)")
field("vm_stack", UnsignedLong, "stack only size (as kb)")
field("vm_swap",  UnsignedLong, "based on linux-2.6.34 'swap ents' (as kb)")
field("vm_exe",   UnsignedLong, "equals 'trs' (as kb)")
field("vm_lib",   UnsignedLong, "total, not just used, library pages (as kb)")
field("vsize",    UnsignedLong, "number of pages of virtual memory ...")
field("rss_rlim", UnsignedLong, "resident set size limit?")
field("flags",    UnsignedLong, "kernel flags for the process")
field("min_flt",  UnsignedLong, "number of minor page faults since process start")
field("maj_flt",  UnsignedLong, "number of major page faults since process start")
field("cmin_flt", UnsignedLong, "cumulative min_flt of process and child processes")
field("cmaj_flt", UnsignedLong, "cumulative maj_flt of process and child processes")

group("User and group identifiers")

field("tgid",  Int, "thread group ID, the POSIX PID (see also: tid)")
field("euid",  Int, "effective user id")
field("egid",  Int, "effective group id")
field("ruid",  Int, "real user id")
field("rgid",  Int, "real group id")
field("suid",  Int, "saved user id")
field("sgid",  Int, "saved group id")
field("fuid",  Int, "fs (used for file access only) user id")
field("fgid",  Int, "fs (used for file access only) user id")
field("tpgid", Int, "terminal process group id")


if __name__ == "__main__":
  import sys
  if len(sys.argv) == 2:
    output = open(sys.argv[1], "w")
  else:
    output = sys.stdout

  builder.build(output)
