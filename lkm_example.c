#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
/* 
#define TASK_RUNNING         0
#define TASK_INTERRUPTIBLE   1
#define TASK_UNINTERRUPTIBLE 2
#define __TASK_STOPPED       4
#define __TASK_TRACED        8
#define EXIT_DEAD            16
#define EXIT_ZOMBIE          32
#define TASK_DEAD            64
#define TASK_WAKEKILL        128
#define TASK_WAKING          256
#define TASK_PARKED          512
#define TASK_NOLOAD          1024
#define TASK_STATE_MAX       2048
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tianhao Li");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static char* s[13] = {"TASK_RUNNING", "TASK_INTERRUPTIBLE", "TASK_UNINTERRUPTIBLE", "__TASK_STOPPED", "__TASK_TRACED", "EXIT_DEAD", "EXIT_ZOMBIE", "TASK_DEAD", "TASK_WAKEKILL", "TASK_WAKING", "TASK_PARKED", "TASK_NOLOAD", "TASK_STATE_MAX"};

static char* task_state = "TASK_UNINTERRUPTIBLE | TASK_UNINTERRUPTIBLE";
static int __init lkm_example_init(void){
  struct task_struct *task;

  char* temp_state;
  int k;
  for_each_process(task){
    int n = task->state, temp = 0, num = 0;
    temp_state = task_state;
    for (k = 0; k < 13; k++){
       if (k > 0) temp = 1 << (k-1);
       if (((n & temp) != 0) || (n == 0)){
         n ^= temp;
         if (num == 0) {
           memcpy(temp_state, s[k], strlen(s[k]));
           temp_state += strlen(s[k]);
         }
         else {
           *temp_state++ = ' '; *temp_state++ = '|'; *temp_state++ = ' ';
           memcpy(temp_state, s[k], strlen(s[k]));
           temp_state += strlen(s[k]);
         }
         num++;
       }
       if (n == 0) break; 
    }
    *temp_state = '\0';
    printk(KERN_INFO "%s [%d] state: %s\n", task->comm, task->pid , task_state);
  }
  return 0;
}

static void __exit lkm_example_exit(void){
  printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
