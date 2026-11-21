#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/utsname.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/mt_wq_debug.h>
#include <linux/xlog.h>
#include "prof_ctl.h"


#include <linux/pid.h>
#define SEQ_printf(m, x...)	    \
 do {			    \
    if (m)		    \
	seq_printf(m, x);	\
    else		    \
	printk(x);	    \
 } while (0)

#define MT_DEBUG_ENTRY(name) \
static int mt_##name##_show(struct seq_file *m, void *v);\
static int mt_##name##_write(struct file *filp, const char *ubuf, size_t cnt, loff_t *data);\
static int mt_##name##_open(struct inode *inode, struct file *file) \
{ \
    return single_open(file, mt_##name##_show, inode->i_private); \
} \
\
static const struct file_operations mt_##name##_fops = { \
    .open = mt_##name##_open, \
    .write = mt_##name##_write,\
    .read = seq_read, \
    .llseek = seq_lseek, \
    .release = single_release, \
};\
void mt_##name##_switch(int on);



/*
 * Ease the printing of nsec fields:
 */
static long long nsec_high(unsigned long long nsec)
{
    if ((long long)nsec < 0) {
	nsec = -nsec;
	do_div(nsec, 1000000);
	return -nsec;
    }
    do_div(nsec, 1000000);

    return nsec;
}

static unsigned long nsec_low(unsigned long long nsec)
{
    if ((long long)nsec < 0)
	nsec = -nsec;

    return do_div(nsec, 1000000);
}
#define SPLIT_NS(x) nsec_high(x), nsec_low(x)

static void print_task(struct seq_file *m, struct task_struct *p)
{
    SEQ_printf(m, "%15s %5d %9Ld %5d ",
	p->comm, p->pid,
	(long long)(p->nvcsw + p->nivcsw),
	p->prio);
#ifdef CONFIG_SCHEDSTATS
    SEQ_printf(m, "%9Ld.%06ld %9Ld.%06ld %9Ld.%06ld\n",
	SPLIT_NS(p->se.vruntime),
	SPLIT_NS(p->se.sum_exec_runtime),
	SPLIT_NS(p->se.statistics.sum_sleep_runtime));
#else
    SEQ_printf(m, "%15Ld %15Ld %15Ld.%06ld %15Ld.%06ld %15Ld.%06ld\n",
	0LL, 0LL, 0LL, 0L, 0LL, 0L, 0LL, 0L);
#endif
}
/*========================================================================*/
/* Real work */
/*========================================================================*/
/* 1. sched info */
MT_DEBUG_ENTRY(sched_debug);
static int mt_sched_debug_show(struct seq_file *m, void *v)
{
    struct task_struct *g, *p;
    unsigned long flags;
    SEQ_printf(m, "=== mt Scheduler Profiling ===\n");
    SEQ_printf(m,
	    "\nrunnable tasks:\n"
	    "            task   PID   switches  prio"
	    "     exec-runtime         sum-exec        sum-sleep\n"
	    "------------------------------------------------------"
	    "----------------------------------------------------\n");
    read_lock_irqsave(&tasklist_lock, flags);

    do_each_thread(g, p) {
	print_task(m, p);
    } while_each_thread(g, p);

    read_unlock_irqrestore(&tasklist_lock, flags);
    return 0;
}
static ssize_t mt_sched_debug_write(struct file *filp, const char *ubuf,
	   size_t cnt, loff_t *data)
{
    return cnt;
}

/* 2. cputime */
MT_DEBUG_ENTRY(cputime);
static int mt_cputime_show(struct seq_file *m, void *v)
{
	return 0;
}

static ssize_t mt_cputime_write(struct file *filp, const char *ubuf,
	   size_t cnt, loff_t *data)
{
    return cnt;

}

/* 4. prof status*/
MT_DEBUG_ENTRY(status);
#define MT_CPUTIME 1
unsigned long mtprof_status = 0;
static int mt_status_show(struct seq_file *m, void *v)
{
    SEQ_printf(m, "%lu\n", mtprof_status);
    return 0; 
}

static ssize_t mt_status_write(struct file *filp, const char *ubuf,
	size_t cnt, loff_t *data)
{
    char buf[64];
    unsigned long val;
    int ret;
    if (cnt >= sizeof(buf))
	return -EINVAL;

    if (copy_from_user(&buf, ubuf, cnt))
	return -EFAULT;

    buf[cnt] = 0;

    ret = strict_strtoul(buf, 10, &val);
    if (ret < 0)
	return ret;
    //0: off, 1:on
    printk("[mtprof] status = 0x%x\n", (unsigned int)mtprof_status);
    mtprof_status = val;
    printk("[mtprof] new status = 0x%x\n", (unsigned int)mtprof_status);
    return cnt;
}
/* 5. workqueue debug */
#ifdef CONFIG_MTK_WQ_DEBUG

extern int mt_dump_wq_debugger(void);
int wq_tracing = 0;
int wq_debugger_enable = 1;

MT_DEBUG_ENTRY(wq_dump);
static int mt_wq_dump_show(struct seq_file *m, void *v)
{
    return 0;
}

static ssize_t mt_wq_dump_write(struct file *filp, const char *ubuf,
	size_t cnt, loff_t *data)
{
    char buf[64];
    unsigned long val;
    int ret;
    if (cnt >= sizeof(buf))
		return -EINVAL;

    if (copy_from_user(&buf, ubuf, cnt))
		return -EFAULT;

    buf[cnt] = 0;

    ret = strict_strtoul(buf, 10, &val);
    if (ret < 0)
		return ret;

    mt_dump_wq_debugger();

    return cnt;
}

static void print_help(struct seq_file *m)
{
#define WQ_LOG_TAG "wq_debug"

	if (m != NULL) {
		SEQ_printf(m, "\n*** Usage ***\n");
		SEQ_printf(m, "commands to enable logs\n");
		SEQ_printf(m, "  echo [queue work] [activate work] [execute work] [wq debugger] > wq_enable_logs\n");
		SEQ_printf(m, "  ex. echo 1 1 1 1  > wq_enable_logs, to enable all logs \n");
		SEQ_printf(m, "  ex. echo 1 0 0 1  > wq_enable_logs, to enable \"queue work\" & \"wq debug\" logs \n");
		SEQ_printf(m, "  Note, please try to enable [wq debugger] as possible as you can.\n");	
	} else {
		xlog_printk(ANDROID_LOG_ERROR, WQ_LOG_TAG, "\n*** Usage ***\n");
		xlog_printk(ANDROID_LOG_ERROR, WQ_LOG_TAG, "commands to enable logs\n");
		xlog_printk(ANDROID_LOG_ERROR, WQ_LOG_TAG, "  echo [queue work] [activate work] [execute work] [wq debugger] > wq_enable_logs\n");
		xlog_printk(ANDROID_LOG_ERROR, WQ_LOG_TAG, "  ex. echo 1 1 1 1  > wq_enable_logs, to enable all logs \n");
		xlog_printk(ANDROID_LOG_ERROR, WQ_LOG_TAG, "  ex. echo 1 0 0 1  > wq_enable_logs, to enable \"queue work\" & \"wq debug\" logs \n");
		xlog_printk(ANDROID_LOG_ERROR, WQ_LOG_TAG, "  Note, please try to enable [wq debugger] as possible as you can.\n");
	}	
}

MT_DEBUG_ENTRY(wq_log);
static int mt_wq_log_show(struct seq_file *m, void *v)
{
	if (wq_tracing & WQ_DUMP_QUEUE_WORK)
		SEQ_printf(m, "wq: queue work log enabled\n");
	if (wq_tracing & WQ_DUMP_ACTIVE_WORK)
		SEQ_printf(m, "wq: active work log enabled\n");
	if (wq_tracing & WQ_DUMP_EXECUTE_WORK)
		SEQ_printf(m, "wq: execute work log enabled\n");
	if (wq_tracing == 0)
		SEQ_printf(m, "wq: no log enabled\n");

	if (wq_debugger_enable)
		SEQ_printf(m, "wq: wq debugger is enabled\n");
	else
		SEQ_printf(m, "wq: wq debugger is disabled\n");

	print_help(m);
    return 0;
}

static ssize_t mt_wq_log_write(struct file *filp, const char *ubuf,
	size_t cnt, loff_t *data)
{
	int log_queue_work = 0, log_activate_work = 0, log_execute_work = 0;
	int log_wq_debugger_enable = 0;
    char buf[64];

    if (cnt > sizeof(buf))
		return -EINVAL;
		
    if (copy_from_user(&buf, ubuf, cnt))
		return -EFAULT;

    buf[cnt] = '\0';
	printk (KERN_DEBUG "rex %s\n", buf);
	
    if (sscanf(buf, "%d %d %d %d", &log_queue_work, &log_activate_work, &log_execute_work, &log_wq_debugger_enable) == 4)
    {
		wq_tracing = 0;
		wq_tracing |= (log_queue_work ? WQ_DUMP_QUEUE_WORK : 0);
		wq_tracing |= (log_activate_work ? WQ_DUMP_ACTIVE_WORK : 0);
		wq_tracing |= (log_execute_work ? WQ_DUMP_EXECUTE_WORK : 0);

		wq_debugger_enable = log_wq_debugger_enable;
    }
	else
		print_help(NULL);
    
    return cnt;
}

#endif //CONFIG_MTK_WQ_DEBUG

/*-------------------------------------------------------------------*/
static int __init init_mtsched_prof(void)
{
    struct proc_dir_entry *pe;
    if (!proc_mkdir("mtprof", NULL)){
	return -1;
    }
    pe = proc_create("mtprof/sched", 0444, NULL, &mt_sched_debug_fops);
    if (!pe)
	return -ENOMEM;
    pe = proc_create("mtprof/cputime", 0664, NULL, &mt_cputime_fops);
    if (!pe)
	return -ENOMEM;

#ifdef CONFIG_MTK_WQ_DEBUG
    pe = proc_create("mtprof/wq_dump", 0664, NULL, &mt_wq_dump_fops);
    if (!pe)
        return -ENOMEM;

    pe = proc_create("mtprof/wq_enable_logs", 0664, NULL, &mt_wq_log_fops);
    if (!pe)
        return -ENOMEM;
#endif

	mt_cpu_num = num_present_cpus();
	mt_cpu_info_head = kmalloc(mt_cpu_num * sizeof(struct mt_cpu_info), GFP_ATOMIC);
	if(mt_cpu_info_head == NULL)
	{
		return -ENOMEM;
	}
#ifdef CONFIG_MT_ENG_BUILD
   
    pe = proc_create("mtprof/status", 0666, NULL, &mt_status_fops);
    if (!pe)
	return -ENOMEM;
#endif    
    return 0;
}
__initcall(init_mtsched_prof);
