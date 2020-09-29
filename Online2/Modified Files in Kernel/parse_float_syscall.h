#ifndef _ADD_SYSCALL_H_
#define _ADD_SYSCALL_H_
#include <linux/linkage.h>
asmlinkage long sys_parse_float_syscall(char __user *, int);
#endif