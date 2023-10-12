# chapter 25

## 25-1

**如果子进程调用 exit(-1)，父进程将会看到何种退出状态（由 WEXITSTATUS()返回）？**

```shell
mayuri@mayuri-virtual-machine:~/文档/25$ ./25-1
255
```

可以看到wait的manual中的：

```plain text
WEXITSTATUS(wstatus)
    returns  the  exit  status  of  the child.  This consists of the
    least significant 8 bits of the status argument that  the  child
    specified  in  a  call to exit(3) or _exit(2) or as the argument
    for a return statement in main().  This macro should be employed
    only if WIFEXITED returned true.
```

-1的32位二进制表示为11111111111111111111111111111111，而从书中我们知道status只有低八位是为父进程所用的，所以截取低八位的结果是11111111，理论上来说还是-1.但是呢，status依然是int，所以最终status如下:00000000000000001111111111111111,也就是255.当然，以上的分析过程只是我的猜测。
