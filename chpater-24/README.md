# chapter -24

## 24-1

**������ִ��������һϵ�� fork()���ú����������½��̣��ٶ�û�е���ʧ�ܣ���**

```C
fork();
fork();
fork();
```

����csapp�е����ݻ���ͼ�������ɺܶ��ˣ�����ܹ�����8�����̣�Ҳ����˵����7���½��̡�

![ͼ 1](../images/96432ac64852404064c05bd54877275b4f1a8defeb5f2d504d8689d982c800ec.png)  

## 24-2

**��дһ�������Ա���֤���� vfork()֮���ӽ��̿��Թر�һ�ļ������������磺������ 0������Ӱ���Ӧ�������е��ļ���������**

����ѡ��ر�STDOUT_FILENO��Ȼ���ڸ����̺��ӽ��̷ֱ����write���������������ɣ�

```shell
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ g++ -o 24-2 24-2.cpp
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ./24-2
child process is about to close STDOUT_FILENO
parent process is about to write to STDOUT_FILENO
you should see this
```

## 24-3

**��������޸ĳ���Դ���룬�����ĳһ�ض�ʱ������һ����ת����core dump���ļ�����ͬʱ���̵��Լ���ִ�У�**

forkһ���ӽ��̣����ӽ�����ֱ�ӵ���abort()���ɡ������������и��㲻���ĵ㣬����Ϊʲôû�а���Ԥ������coredump����ʹ��ulimit -c�������Ϊ0��Ȼ���Ҿ�ͨ��ulimit -c unlimited�޸���coredump�ļ���С���ޣ�������Ȼ�޷����ɡ�����һ������ֵ����飬�����Ͽ�ǰͨ��ulimit -c unlimited

```shell
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ulimit -c
0
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ulimit -c unlimited
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ulimit -c
unlimited
```

�����ҵ��������¿�ʼ�����ͱ��������:

```shell
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ulimit -c
0
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ulimit -c unlimited
bash: ulimit: core file size: �޷��޸����ƣ�������Ĳ���
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ sudo ulimit -c unlimited
sudo: ulimit���Ҳ�������
```

��������һ�£�����ԭ�����ڱ����ǲ�����ulimit�Ķ������ļ��ģ����Ҳ���Է�ӳ����ȥman ulimit������������һ��ϵͳ���á�ulimit��shell���õ�һ�����
�ο���<https://stackoverflow.com/questions/17483723/command-not-found-when-using-sudo-ulimit>

��ʹ�������еĽ���������ɹ��������޸ģ�

```shell
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ sudo prlimit --pid=$$ --core=unlimited
[sudo] mayuri �����룺 
mayuri@mayuri-virtual-machine:~/�ĵ�/24$ ulimit -c
unlimited
```

������Ȼ�޷�����coredump��
�һ�������Ҷ���Ŀ¼��Ȩ�ޣ�Ҳ����д��Ȩ�޵ģ��һ�ͨ��signal��SIGABRT�Ĵ������ع���Ĭ�ϣ�Ҳ���ǲ��С�

## 24-4

**������ UNIX ʵ����ʵ������嵥 24-5��fork_whos_on_first.c���еĳ��򣬲��ж���ִ�� fork()����Щϵͳ����ε��ȸ��ӽ��̵ġ�**

��û������UNIXʵ���ˣ��Ͳ�ʵ���ˡ�

## 24-5

**�ٶ��ڳ����嵥 24-6 �ĳ����У��ӽ���Ҳ��Ҫ�ȴ����������ĳЩ������Ϊȷ�������һĿ�ģ�Ӧ����޸ĳ���**

�������߼�����:
- �ӽ������һ���ֹ�������ɺ�ͨ��kill֪ͨ�����̣�����ʼ�ȴ�
- ������Ҳ��һ���ֹ����������kill֪ͨ�ӽ��̣�����ʼ�ȴ�
- �ӽ����յ��������źţ���ʼ���ʣ�๤������ɺ�kill֪ͨ�����̣��������
- �������յ��źţ���������ִ��
