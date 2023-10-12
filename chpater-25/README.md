# chapter 25

## 25-1

**����ӽ��̵��� exit(-1)�������̽��ῴ�������˳�״̬���� WEXITSTATUS()���أ���**

```shell
mayuri@mayuri-virtual-machine:~/�ĵ�/25$ ./25-1
255
```

���Կ���wait��manual�еģ�

```plain text
WEXITSTATUS(wstatus)
    returns  the  exit  status  of  the child.  This consists of the
    least significant 8 bits of the status argument that  the  child
    specified  in  a  call to exit(3) or _exit(2) or as the argument
    for a return statement in main().  This macro should be employed
    only if WIFEXITED returned true.
```

-1��32λ�����Ʊ�ʾΪ11111111111111111111111111111111��������������֪��statusֻ�еͰ�λ��Ϊ���������õģ����Խ�ȡ�Ͱ�λ�Ľ����11111111����������˵����-1.�����أ�status��Ȼ��int����������status����:00000000000000001111111111111111,Ҳ����255.��Ȼ�����ϵķ�������ֻ���ҵĲ²⡣
