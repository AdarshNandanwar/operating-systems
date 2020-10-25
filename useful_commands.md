# Display all running process of another terminal

```bash
watch -n 1 ps -o cmd,pid,ppid,stat -t /dev/pts/0
```