# chapter-23

## 23-1

**���� Linux �� alarm()ʵ��Ϊϵͳ���ã����⵱�����㡣��setitimer()ʵ�� alarm()��**

�����������һ��ʼ�����������ˣ���ʵseconds=0��ʱ��Ҳ�����������ж�ʱ���Ĺ��ܒYʱ��ʹ��setitimerҲ�㹻�ˡ�

```c
new_value.it_value = {seconds, 0};
```

��seconds=0��it_value��������Ա����0��setitimer����ȡ�����ж�ʱ���Ĺ��ܡ�

## 23-2

���Ž������嵥 23-3��t_nanosleep.c���������ں�̨���У������� 60 ������߼����ͬʱʹ����������;����ܶ�� SIGINT �źŸ���̨���̣�

```shell
while true; do kill -INT pid; done
```

**Ӧ����ע�⵽��������ʱ��Ҫ����Ԥ�ڡ��� nanosleep()�� clock_gettime()��ʹ��CLOCK_REALTIME ʱ�ӣ������� TIMER_ABSTIME ��־�� clock_nanosleep()���滻��������ϰ��Ҫ Linux 2.6 �汾�������������޸ĺ�ĳ��򣬲��������ϳ����Ĳ��**

��ֱ���ڴ����н����߼���趨Ϊ��60s����������������:

```shell
mayuri@mayuri-virtual-machine:~/�ĵ�/23$ ./23-2 &
[5] 5790
mayuri@mayuri-virtual-machine:~/�ĵ�/23$ while true; do kill -INT 5790; done
```

�ᷢ���������:

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

**��дһ��������֤��������� timer_create()ʱ������ evp ��Ϊ NULL����ô��͵�ͬ�ڽ� evp ��Ϊָ�� sigevent �ṹ��ָ�룬�����ýṹ�е� sigev_notify ��Ϊ SIGEV_ SIGNAL���� sigev_signo ��Ϊ SIGALRM���� si_value.sival_int ��Ϊ��ʱ�� ID��**

to do

## 23-4

**�޸ĳ����嵥 23-5��ptmr_sigev_signal.c���г��򣬲��� sigwaitinfo()�滻�źŴ�����������**

to do
