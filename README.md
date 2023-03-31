# UserRegEnum
域内普通域用户权限查找域内所有计算机上登录的用户

## 使用方法

```
Usage example:
  program.exe -d missyou.com -dc dc.missyou.com -o result.csv
Options:
  -d,  --domain=Domain               domain name to search
  -dc, --dc=domainControllers        domain controller to query
  -t,  --threads=Theads              number of threads, default: 1000
  -o,  --output=outputResult         output result file
  -h,  --help                        display this help and exit
```

其中 `-t` 是非必须参数, 默认是 1000, 在本地调试的时候觉得 1000 线程挺合适的, 如果没有特殊需求可以不设置这个参数.

`-d`, `-dc`, `-o` 这三个参数时必须的.

![image](https://github.com/TryA9ain/UserRegEnum/blob/master/Pictures/Xnip2023-03-31_12-50-08.png)

结果会保存在当前 `.exe` 同级目录下.

![image](https://github.com/TryA9ain/UserRegEnum/blob/master/Pictures/Xnip2023-03-31_12-57-56.png)

**域内计算机越多, 程序执行时间越长, 目前已知 2w 余台机器的域查询耗时 7 分钟左右.**

## 原理

通过枚举每一台计算机上的注册表 HKEY_USERS 项的 key 值来查询谁登录过, 具体原理请看文章.
