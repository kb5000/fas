# fas
A useful lib for C++
  
       fas实用命名空间
       
   使用说明：
   
   faster(void)：内联函数，关闭了c的stdio语句与iostream之间的流同步。在处理大规模字符串时有用
   
   tstring(auto n)：将数字等各种类型转化为string型。不能用于字符数组。要将字符数组转换为string,请使用string的构造函数char *c;std::string s(c);。要在自定义的类型中使用它，请定义operator<<(ostringstream,你的类型)函数
   
   tonum<类型>(string)将字符串转换为数字等类型。当无法转换（数字错误）时，会抛出invalid_argument异常。要转化为自定义类型，请定义 operator>>(istringstream,类型)
   
   fmcout(void)设置cout的浮点输出位数为16位，这是为了解决c4droid默认只输出6位浮点数，只是记忆方便。
   
   timer类用于测量某操作所用时间。有start()和stop()两个成员函数用于内部计时器。要获取时间，请用gettimes(unit)函数。unit是一个枚举，包含nano(纳秒),micro(微秒),milli(毫秒),sec(秒),min(分)以及hour(小时)六个单位。也可以用sleep(时间,单位)让程序暂停一段时间。
   
   定义了一个名为cswitch(变量,值,可执行对象,值,可执行对象,...,默认(default)情况的可执行对象)的函数，可以实现任意对象的switch，这要求使用对象的==运算符。本函数将在匹配到一个结果后退出，如没有结果，会执行default的内容。可执行对象可以返回一个值（或者是void），但每个可执行对象都应是相同的返回类型，它将会作为cswitch的返回值。注意，可执行对象泛指函数指针、重载了operator()的对象以及lambda，但都不应接受参数(空形参列表)。最后一个参数(default)可以省略，但前面的值和可执行对象必须成对出现。要使用自定义运算符(如大于)，请用mif。
   
   mif(表达式,可执行对象,表达式,可执行对象,...,可执行对象(else))将执行第一个使表达式为true的相对应的可执行对象。类似于cswitch，这个可执行对象不应接受参数，但可以返回一个值（void也可以），每个可执行对象的返回都应相同。且会在执行一个可执行对象之后退出，如果找不到，会执行可选的else可执行对象。除else外，表达式和可执行对象要成对出现。
   
   aif(表达式,可执行对象,表达式,可执行对象,...,可执行对象(else))类似于mif，但会执行所有使表达式为true的对应可执行对象。且只有所有表达式为false时，才执行else部分。当可执行对象返回一个值时，会返回一个包含有返回值的vector。
   
   定义了一个名为get_stype<类型A,类型B,返回类型A,返回类型B>的元函数。当类型A与类型B相同时，其成员type为返回类型A，当两类型不同时，type为返回类型B。如stype<int,double,char,unsigned>::type为unsigned。
   
   定义了get_rtype<目标类型,默认类型,类型1,类型2...>元函数。它会将目标类型与后面的类型列表相匹配，若找到相同的类型，返回它，否则返回默认类型。
   
   定义了一个point<类型,个数>类，表示指定个数的同一类型的集合，它比array更小更简单，不支持迭代器，可以用下标或p.x(),p.y(),p.z(),p.w()的形式访问。其中，x,y,z,w是四个成员函数，分别代表p[0],p[1],p[2],p[3]。可以使用make_point(point,point)合并两个point，要求这两个point类型相同，长度可以不同。也可以用make_point(array)从数组或array容器中建立point。
   
   定义了一个noncopyable类，与boost的noncopyable相似，防止发生拷贝。
   
   定义了一个print(数据,数据,...)可执行对象。可以方便地输出一系列数据，它也可以指定fas::endl来输出换行。它也可以直接输出数组和容器，每个元素之间用end_t来分隔，通过print.end_t()访问和print.end_t(string)设置。注意，容器同时也应该定义size()获取大小。字符串输出不受影响（自定义的除外）。
   
   定义了一个用于测试类是否含有指定成员函数的宏，要先用HAS_MEMBER(函数名)初始化，再用has_member_函数名<类名>::value获取值。
   #来源：http://www.csdn.net/article/2015-12-03/2826381
   
   定义了一个日志类log_t和它的一个对象log。其构造函数为：log_t(uint序号,(开始记录时的字符串,结束时的字符串)),log_t(目录及文件名开头,(序号,开始记录时的字符串,结束时的字符串))。有一个changeNum(序号)用于改变序号，同时改变文件。clearLog()用于清空并重新记录当前的日志文件。使用operator()(内容,...)或operator<<进行写入，存于"自定义文件名_序号.log"中。每次写入（执行一次operator()）都会生成一行，自定义的开始和结束字符串也会在构造和析构时调用并生成新行。也可以使用fas::endl直接开始新行。默认的对象log会将文件存在当前目录的log_0.log中，开始和结束的字符串为空并不产生新行。
   
       高级字符串工具
       只对命令行输出有效
       
       
     使用方法：
      scolor(字符串,前景色,背景色)设置颜色
    可用颜色参考下方宏定义
    underline(字符串)设置下划线
    antic(字符串)反色
    movec(方向,数目)或(x坐标,y坐标)(左上角为1,1)移动光标//方向指'l'左'd'下'r'右'u'上
    clrall()清屏
    clrlin(数目)清除前n个字符
    cursav()保存光标位置
    curu()恢复上一次保存的位置
   已经定义了一个tpro对象
     如：
	    string v="text",u;
	    
	    u=fas::tpro.scolor(v,fblue,bred);
	    
	    cout<<u<<endl;
	    
	    u=fas::tpro.underline(v);
	    
	    cout<<u<<endl;
	    
	    cout<<fas::tpro.movec('l',3);
	    
	    cout<<fas::tpro.movec(1,1);
	    
	    u=fas::tpro.antic(v);
	    
	    cout<<u<<endl;
	    
	    cout<<fas::tpro.clrall();
	    


