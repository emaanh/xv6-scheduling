#include "types.h"
#include "user.h"
#include "schedstats.h"

int
main(int argc, char *argv[])
{
  struct schedstats st;
  int pid;

  printf(1, "pid\tdisp_cnt\tdisp_avg\tdisp_max\twait_sum\tswtch_cnt\tswtch_sum\n");
  for(pid = 1; pid < 64; pid++){
    if(get_schedstats(pid, &st) == 0){
      uint avg = 0;
      if(st.dispatch_count > 0)
        avg = st.dispatch_sum / st.dispatch_count;
      printf(1, "%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
             st.pid, st.dispatch_count, avg, st.dispatch_max,
             st.wait_sum, st.swtch_count, st.swtch_sum);
    }
  }
  exit();
}
