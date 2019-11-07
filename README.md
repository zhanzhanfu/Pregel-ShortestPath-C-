### Pregel 最短路径 C++实现

这是一次 C++ 作业，网上用C++实现 pregel 的的比较少，因此记录一下，仅供参考。关于pregel 图模型的介绍，网上很多，这里随便放一篇：

 [https://cshihong.github.io/2018/05/30/Pregel%EF%BC%88%E5%9B%BE%E8%AE%A1%E7%AE%97%EF%BC%89%E6%8A%80%E6%9C%AF%E5%8E%9F%E7%90%86/](https://cshihong.github.io/2018/05/30/Pregel（图计算）技术原理/) 

* hama 生成两张图，一张10个点，一张100点，拿到 windows  vs2019中测试

```c++
./bin/hama jar hama-examples-0.7.1.jar gen fastgen -v 10 -e 3 -o graph10.txt  
./bin/hama jar hama-examples-0.7.1.jar gen fastgen -v 100 -e 5 -o graph100.txt
```

* 代码说明：
  * src_vertex 为原点
  * 顶点距离设为1
  * 实现怎么简单怎么来，Vertex 类放在代码里但并没有用到，可以直接使用 ShortestPathVertex 类
  * vector<vector<int> > messages(105) 作为消息载体，MessageIterator 构造函数中清空 messages[i]，作为最终结束的判断。
  * MessageIterator 构造函数应该可以进行优化，这里存在 vector<int>  的复制，用指针也许更好。
  * 为了尽可能少的修改 ShortestPathVertex 的结构，定义了 Iterator 基类
  * 最终结束的判断没有使用 bool isHalt，只判断 messages 是否为空，此处已经足够
