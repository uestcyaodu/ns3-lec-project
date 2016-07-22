#Project 1
###17级 中国矿业大学 杜耀
---
##Project要求
- 用命令行读入自己的学号和姓名参数以及输出频率，周期性输出姓名和学号,不能使用硬编码
- 利用UNIX实用工具wc（word count）统计输出的总次数
- 利用UNIX实用工具grep筛选特定信息

##代码分析
- 传递参数为四个，在主函数中用cmd.ADDValue()添加：
'''C++
static void printHello(std::string name,int number,float stop_at,float cycle)

cmd.AddValue ("name", "my_name", name);//姓名
cmd.AddValue ("number","ID", number);//学号
cmd.AddValue ("stop_at","running_time", stop_at);//总时间
cmd.AddValue ("cycle","running_cycle", cycle);//周期
'''
- 子函数printHello()中主要使能仿真器代码为：
'''C++
  Simulator::Schedule(Seconds(cycle),&printHello,name,number,stop_at,cycle);
'''
- 主函数调用printHello()：
‘’’C++
	printHello(name,number,stop_at,cycle);
'''
*注明：由于事件列表从0s开始，为了输入与输出的统一，将总执行时间stop_at值加1：*
‘’’C++
	Simulator::Stop(Seconds(stop_at+1));
'''

##运行截图
![Image of pro_1]
(http://photo.weibo.com/5029498630/photos/large/photo_id/4000015113666529/album_id/4000015059140049)
- 输入运行时间：至20s停止(0s输出第1行)，输出频率为4s/次
- 统计输出行数： wc -l 共六行，即总共输出了6次，结果正确
- 筛选12s的输出： grep "+12"
