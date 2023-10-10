# chapter-23

## 23-1

**尽管 Linux 将 alarm()实现为系统调用，但这当属蛇足。请setitimer()实现 alarm()。**

这里的问题是一开始光想着特判了，其实seconds=0的时候，也就是屏蔽现有定时器的功能扽时候，使用setitimer也足够了。

```c
new_value.it_value = {seconds, 0};
```

当seconds=0，it_value的两个成员都是0，setitimer会起到取消现有定时器的功能。

## 23-2

试着将程序清单 23-3（t_nanosleep.c）程序置于后台运行，并设置 60 秒的休眠间隔，同时使用如下命令发送尽可能多的 SIGINT 信号给后台进程：

```shell
while true; do kill -INT pid; done
```

**应该能注意到程序休眠时间要长于预期。将 nanosleep()用 clock_gettime()（使用CLOCK_REALTIME 时钟）和设置 TIMER_ABSTIME 标志的 clock_nanosleep()来替换。（此练习需要 Linux 2.6 版本。）反复测试修改后的程序，并解释新老程序间的差别。**

我直接在代码中将休眠间隔设定为了60s。在命令行中输入:

```shell
mayuri@mayuri-virtual-machine:~/文档/23$ ./23-2 &
[5] 5790
mayuri@mayuri-virtual-machine:~/文档/23$ while true; do kill -INT 5790; done
```

会发现如下输出:

```plain text
Remaining: 69.397796723
Remaining: 69.397749650
Slept for: 46.442211 secs
Slept for: 46.444710 secs
Slept for: 46.446938 secs
Remaining: 69.399931065
Remaining: 69.399419273
Slept for: 46.472772 secs
Remaining: 69.402192051
Remaining: 69.402269265
Slept for: 46.474155 secs
Remaining: 69.401715889
Remaining: 69.401958052
Remaining: 69.406769166
Slept for: 46.494207 secs
Remaining: 69.402959763
Slept for: 46.498186 secs
Remaining: 69.404705467
Remaining: 69.405266032
Remaining: 69.404393116
Remaining: 69.404471109
Slept for: 46.507810 secs
Remaining: 69.403994047
Slept for: 46.508673 secs
Remaining: 69.404207919
Slept for: 46.523749 secs
Remaining: 69.404890228
Remaining: 69.405029428
Remaining: 69.405077538
Slept for: 46.530182 secs
Remaining: 69.408065636
Remaining: 69.409845799
Remaining: 69.408889677
Slept for: 46.539883 secs
Remaining: 69.408243010
Remaining: 69.408277054
Remaining: 69.408420717
Slept for: 46.542197 secs
Remaining: 69.408600970
Slept for: 46.543745 secs
Remaining: 69.411122690
Remaining: 69.410639222
Slept for: 46.556563 secs
Remaining: 69.410098824
Remaining: 69.410417963
Remaining: 69.411046090
Slept for: 46.562184 secs
Slept for: 46.564927 secs
Remaining: 69.414039049
Remaining: 69.415167756
Slept for: 46.572827 secs
Remaining: 69.412294713
Slept for: 46.574109 secs
Remaining: 69.411187863
Slept for: 46.575132 secs
Remaining: 69.412397938
Slept for: 46.581963 secs
Slept for: 46.585204 secs
Slept for: 46.586266 secs
Remaining: 69.414366680
```

to do

## 23-3

**编写一个程序验证：如果调用 timer_create()时将参数 evp 置为 NULL，那么这就等同于将 evp 设为指向 sigevent 结构的指针，并将该结构中的 sigev_notify 置为 SIGEV_ SIGNAL，将 sigev_signo 置为 SIGALRM，将 si_value.sival_int 置为定时器 ID。**

to do

## 23-4

**修改程序清单 23-5（ptmr_sigev_signal.c）中程序，并用 sigwaitinfo()替换信号处理器函数。**

to do
