struct schedstats {
  int pid;
  uint dispatch_count;
  uint dispatch_sum;
  uint dispatch_max;
  uint wait_sum;
  uint swtch_count;
  uint swtch_sum;
};
