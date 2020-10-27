#ifndef __ADD_WRAPPER_H_
#define __ADD_WRAPPER_H_

#define add_syscall(num1, num2) (syscall(440 , num1, num2))

#endif