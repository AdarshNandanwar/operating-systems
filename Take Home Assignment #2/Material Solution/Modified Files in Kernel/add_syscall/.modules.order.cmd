cmd_add_syscall/modules.order := {  :; } | awk '!x[$$0]++' - > add_syscall/modules.order
