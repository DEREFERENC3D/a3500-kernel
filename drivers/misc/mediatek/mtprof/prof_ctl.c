#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/utsname.h>
#include <linux/jiffies.h>
#include <linux/kernel_stat.h>
#include <asm/uaccess.h>
#include <linux/tick.h>
#include "prof_main.h"
#include <linux/version.h>

struct mt_proc_struct *mt_proc_curr = NULL;
struct mt_proc_struct *mt_proc_head = NULL;
int proc_count = 0;
int mtsched_enabled = 0;
unsigned long long prof_start_ts, prof_end_ts, prof_dur_ts;
static DEFINE_MUTEX(mt_cputime_lock);
static DEFINE_MUTEX(mt_memprof_lock);

struct mt_cpu_info *mt_cpu_info_head = NULL;
int mt_cpu_num = 1;

static inline u64 get_cpu_idle_time_jiffy(unsigned int cpu, u64 *wall)
{
	u64 idle_time;
	u64 cur_wall_time;
	u64 busy_time;

	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());

	busy_time  = kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_STEAL];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];

	idle_time = cur_wall_time - busy_time;
	if (wall)
		*wall = jiffies_to_usecs(cur_wall_time);

	return jiffies_to_usecs(idle_time);
}

void mt_cputime_switch(int on);
unsigned long long mtprof_get_cpu_idle(int cpu)
{
    u64 unused = 0, idle_time = 0;
	idle_time = get_cpu_idle_time_us(cpu, NULL);

	if (idle_time == -1ULL)
	{	
		return get_cpu_idle_time_jiffy(cpu, &unused);
	}
	else
	{	
		idle_time += get_cpu_iowait_time_us(cpu, &unused);
	}
	printk("update time is is %llu\n", unused);
					
	return idle_time;
}
unsigned long long mtprof_get_cpu_iowait(int cpu)
{
    unsigned long long *unused = 0;
    return get_cpu_iowait_time_us(cpu, unused);
}
void mt_task_times(struct task_struct *p, cputime_t *ut, cputime_t *st){
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)   
   task_times(p,ut,st);
#else
   task_cputime_adjusted(p,ut,st);
#endif
}
/********************
     MT cputime prof
*********************/
void setup_mtproc_info(struct task_struct *p, unsigned long long ts){
    return;
}
void save_mtproc_info(struct task_struct *p, unsigned long long ts){
    return;
}
void end_mtproc_info(struct task_struct *p){
    return;
}
void set_mtprof_comm(char* comm, int pid){
    return;
}
void start_record_task(void){
    return;
}
void stop_record_task(void){
    return;
}
void reset_record_task(void){
    return;
}
void mt_cputime_switch(int on){
    return;
}
