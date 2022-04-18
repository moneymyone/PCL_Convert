# PCL_Convert
This is a demo for converting custom format pcd files

source pcd file format:
![image](https://user-images.githubusercontent.com/7404471/163747916-6bce7f01-5343-4883-8ee4-991b71e61bbb.png)

可以看出来总共有6个字段，其中两个字段是`_`，没有具体规定是什么字段。
现在要把这两个不知名的字段去掉，保留剩余4个字段 `x y z intensity`。


通过自定义点类型实现点云文件转换。

定义点类型的时候，短横类型设置为int 或者 string对于结果没有影响。

## 编译
```bash
mkdir build
cmake ..
make
./convert
```
