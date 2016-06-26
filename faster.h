

#ifndef FASTERA
#define FASTERA
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>
#include <fstream>
/*
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
定义了一个日志类log_t和它的一个对象log。其构造函数为：log_t(uint序号,(开始记录时的字符串,结束时的字符串)),log_t(目录及文件名开头,(序号,开始记录时的字符串,结束时的字符串))。有一个changeNum(序号)用于改变序号，同时改变文件。clearLog()用于清空并重新记录当前的日志文件。使用operator()(内容,...)或operator<<进行写入，存于"自定义文件名_序号.log"中。每次写入（执行一次operator()）都会生成一行，自定义的开始和结束字符串也会在构造和析构时调用并生成新行。也可以使用fas::endl直接开始新行。默认的对象log会将文件存在当前目录的log_0.log中，开始和结束的字符串为空并不产生新行。
*/


//fas实用命名空间
namespace fas 
{
//取消c和c++中输入输出语句的同步，加快速度
  inline void faster()
  {
  //close sync with c++ istream and c stream
std::ios_base::sync_with_stdio(false);

  }
//替代版tostring
  template<typename T>
  std::string tstring(T n)
  {
  	std::string s;
  	std::ostringstream f(s);
 	 if (f<<n)
  	  return f.str();
  	else
  	  throw std::runtime_error("unknown error");
  }
//替代版tonum
  template<typename T>
  T tonum(const std::string& str)
  {
  	std::istringstream p(str);
  	T num;
  	if (p>>num)
  	  return num;
  	else
  	  throw std::invalid_argument("wrong number");
  	return 0;
  }
  template<>
  int tonum(const std::string& str)
  {
  	return atoi(str.c_str());
  }
  template<>
  double tonum(const std::string& str)
  {
  	return atof(str.c_str());
  }
  template<>
  long tonum(const std::string& str)
  {
  	return atol(str.c_str());
  }
  template<>
  long long tonum(const std::string& str)
  {
  	return atoll(str.c_str());
  }
//设置cout格式
  inline void fmcout()
  {
  	std::cout<<std::flush;
  	std::cout.precision(16);
  }
//计时类
  class timer
  {
  public:
    enum class unit:int
    { nano,micro,milli,sec,min,hour };
    void start()
    {
    	if (!isStart)
    	{
    	  isStart=true;
    	  	t=std::chrono::high_resolution_clock::now();
    	}
    }
    void stop()
    {
    	if (isStart)
    	{
      	tend=std::chrono::high_resolution_clock::now();
    	  isStart=false;
    	}
    }
    timer_t gettimes(unit u)
    {
     if (!isStart)
    	switch (u)
    	{
      case unit::nano:
        return std::chrono::duration_cast<std::chrono::nanoseconds>(tend-t).count();
      case unit::micro:
        return std::chrono::duration_cast<std::chrono::microseconds>(tend-t).count();
      case unit::milli:
        return std::chrono::duration_cast<std::chrono::milliseconds>(tend-t).count();
      case unit::sec:
        return std::chrono::duration_cast<std::chrono::seconds>(tend-t).count();
      case unit::min:
        return std::chrono::duration_cast<std::chrono::minutes>(tend-t).count();
      case unit::hour:
        return std::chrono::duration_cast<std::chrono::hours>(tend-t).count();
    	}
    	return timer_t();
    }
    
    void sleep(timer_t time,unit u)
    {
    	switch (u)
    	{
    	case unit::nano:
    	  std::this_thread::sleep_for(std::chrono::nanoseconds(time));
    	  break;
      case unit::micro:
     std::this_thread::sleep_for(std::chrono::microseconds(time));
        break;
      case unit::milli:
    	  std::this_thread::sleep_for(std::chrono::milliseconds(time));
    	  break;
    	case unit::sec:
    	  std::this_thread::sleep_for(std::chrono::seconds(time));
    	  break;
    	case unit::min:
    	  std::this_thread::sleep_for(std::chrono::minutes(time));
    	  break;
    	case unit::hour:
    	  std::this_thread::sleep_for(std::chrono::hours(time));
    	  break;
    	}
    }
    
    //timer()=default;
      timer():t(std::chrono::high_resolution_clock::now()),tend(std::chrono::high_resolution_clock::now()){}
    //timer(const timer&)=delete;
    //timer& operator=(const timer&)=delete;
  private:
     std::chrono::time_point<std::chrono::high_resolution_clock> t,tend;
     bool isStart=false;
  };
  
//泛型switch
/*  template<typename T,typename N,typename Fuc>
void eswitch(const T& a,N b,Fuc f)
{
	if (a==b)
	  f();
}
template<typename T,typename Fuc>
void eswitch(const T& a,Fuc f)
{
	f();
}
  template<typename T,typename N,typename Fuc,typename... Args>
void eswitch(const T& a,N b,Fuc f,const Args&... arg)
{
	if (a==b)
	{
		f();
		//eswitch(a,arg...);
	}
	else
	{
	  eswitch(a,arg...);
	}
}*/
template<typename A,typename Fuc>
decltype(auto) cswitch(A a,Fuc f)
{
	if (std::is_void<decltype(f())>::value)
	  f();
	else
	  return f();
}
template<typename A,typename B,typename Fuc>
decltype(auto) cswitch(A a,B b,Fuc f)
{
	if (std::is_void<decltype(f())>::value)
	{
		if (a==b)
		  f();
	}
	else
	{
		if (std::is_pod<decltype(f())>::value)
		{
		  decltype(f()) re=0;
		  if (a==b)
		    return f();
		  return re;
		}
		else
		{
			decltype(f()) rc;
			if (a==b)
			  return f();
			return rc;
		}
	}
}
template<typename A,typename B,typename Fuc,typename... Args>
decltype(auto) cswitch(A a,B b,Fuc f,Args... arg)
{
	if (std::is_void<decltype(f())>::value)
	{
		if (a==b)
		{
		  f();
		}
		else
		{
		  cswitch(a,arg...);
		}
	}
	else
	{
		if (a==b)
		  return f();
		//auto x=cswitch(a,arg...);
		return cswitch(a,arg...);
	}
}

//多分支if
template<typename Fuc>
decltype(auto) mif(bool exp,Fuc f)
{
	if (exp)
	{
		//std::cout<<exp;
		if (std::is_void<decltype(f())>::value)
		{
			f();
		}
		else
		{
		  return f();
		}
	}
	else
	{
		if (std::is_pod<decltype(f())>::value)
		{
		  decltype(f()) rv=0;
		  return rv;
		}
		else
	  {
			decltype(f()) rz;
			return rz;
		}
	}
}
template<typename Fuc>
decltype(auto) mif(Fuc f)
{
	if (std::is_void<decltype(f())>::value)
	{
		f();
	}
	else
	{
		return f();
	}
}
template<typename Fuc,typename... Args>
decltype(auto) mif(bool exp,Fuc f,Args... arg)
{
	if (exp)
	{
		//std::cout<<exp;
	  if (std::is_void<decltype(f())>::value)
	  {
	    f();
	  }
	  else
	  {
	  	return f();
	  }
	}
	else
	{
	  return mif(arg...);
	}
}

//类型判断
template<typename A,typename B,typename M,typename N>
struct get_stype
{
	typedef N type;
};
template<typename A,typename M,typename N>
struct get_stype<A,A,M,N>
{
	typedef M type;
};


//类型匹配器
template<typename M,typename D,typename B,typename... N>
struct get_rtype
{
  typedef typename get_rtype<M,D,N...>::type type;
};
template<typename M,typename D,typename... N>
struct get_rtype<M,D,M,N...>
{
  typedef M type;
};
template<typename M,typename D,typename B>
struct get_rtype<M,D,B>
{
	typedef D type;
};
template<typename M,typename D>
struct get_rtype<M,D,M>
{
	typedef M type;
};

//非中断if
class __flagOfAif__
{
	template<typename Fuc>
  friend auto aif(bool exp,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
  template<typename Fuc>
friend auto aif(Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
  template<typename Fuc>
friend auto gaif(std::vector<auto>& vcon,bool exp,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
  template<typename Fuc>
friend auto gaif(std::vector<auto> vcon,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
  template<typename Fuc,typename... Args>
friend auto gaif(std::vector<auto> vcon,bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
  template<typename Fuc,typename... Args>
friend auto aif(bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
  bool f=false;
}__flagOfAif;
template<typename Fuc>
auto aif(bool exp,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
	__flagOfAif.f=false;
	if (exp)
	{
		if (std::is_void<decltype(f())>::value)
		{
			f();
		}
		else
		{
			std::vector<decltype(f())> con{f()};
		  return con;
		}
	}
	else
	{
		if (!std::is_void<decltype(f())>::value)
		{
			std::vector<decltype(f())>  con;
			return con;
		}
	}
}
template<typename Fuc>
auto aif(Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
	if (!__flagOfAif.f)
	{
		if (std::is_void<decltype(f())>::value)
		{
	    f();
		}
	  else
	  {
	  	std::vector<decltype(f())> con{f()};
	  	return con;
	  }
	}
	else
	  __flagOfAif.f=false;
}
template<typename Fuc>
auto gaif(std::vector<auto>& vcon,bool exp,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
	__flagOfAif.f=false;
	if (exp)
	{
			vcon.push_back(f());
	}
	return vcon;
}
template<typename Fuc>
auto gaif(std::vector<auto> vcon,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
	if (!__flagOfAif.f)
	{
	  	vcon.push_back(f());
	}
	else
	  __flagOfAif.f=false;
	return vcon;
}

template<typename Fuc,typename... Args>
auto gaif(std::vector<auto> vcon,bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
	if (exp)
	{
			__flagOfAif.f=true;
			vcon.push_back(f());
	}
	return gaif(vcon,arg...);
}

template<typename Fuc,typename... Args>
auto aif(bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
	if (exp)
	{
		if (std::is_void<decltype(f())>::value)
		{
			__flagOfAif.f=true;
	    f();
	    aif(arg...);
		}
		else
		{
			__flagOfAif.f=true;
      std::vector<decltype(f())> con{f()};
			return gaif(con,arg...);
		}
	}
	else
	{
	  return aif(arg...);
	}
}


//point快速类
template<typename T,unsigned N>
class point
{
public:
	point()=default;
	point(const point<T,N>& p)
	{
	  for (unsigned i=0;i<p.length;i++)
	  {
	  	pr[i]=p.pr[i];
	  }
	}
	point(point<T,N>&& p) noexcept
	{
		for (unsigned i=0;i<length;i++)
		{
			pr[i]=p.pr[i];
		}
	}
	point(T dat)
	{
		for (unsigned i=0;i<length;i++)
		{
			pr[i]=dat;
		}
	}
	point(std::initializer_list<T> itl)
	{
		if (itl.size()<=length)
		{
			unsigned i=0;
		  for (const auto& j:itl)
		  {
		  	pr[i++]=j;
		  }
		}
		else
		{
			throw std::invalid_argument("too much argument");
		}
	}
  point& operator=(const point<T,N>& p)
  {
	  for (unsigned i=0;i<p.length;i++)
	  {
	  	pr[i]=p.pr[i];
	  }
	  return *this;
  }
  point& operator=(point<T,N>&& p) noexcept
  {
  	for (unsigned i=0;i<length;i++)
		{
			pr[i]=p.pr[i];
		}
  	return *this;
  }
  point& operator=(std::initializer_list<T> itl)
	{
		if (itl.size()<=length)
		{
			unsigned i=0;
		  for (const auto& j:itl)
		  {
		  	pr[i++]=j;
		  }
		}
		else
		{
			throw std::invalid_argument("too much argument");
		}
		return *this;
	}
  T& operator[](unsigned place)
  {
  	if (place<length)
  	  return pr[place];
  	else
  	  throw std::out_of_range("too big argument");
  }
  const T& operator[](unsigned place) const
  {
  	if (place<length)
  	  return pr[place];
  	else
  	  throw std::out_of_range("too big argument");
  }
  constexpr unsigned size() const
  {
  	return length;
  }
  T& x()
  {
  	return pr[0];
  }
  void x(const T& num)
  {
  	pr[0]=num;
  }
  T& y()
  {
  	if (length>1)
  	  return pr[1];
  	else
  	  throw std::out_of_range("it has no pr[1]");
  }
  void y(const T& num)
  {
  	if (length>1)
  	  pr[1]=num;
  	else
  	  throw std::out_of_range("it has no pr[1]");
  }
  T& z()
  {
  	if (length>2)
  	  return pr[2];
  	else
  	  throw std::out_of_range("it has no pr[2]");
  }
  void z(const T& num)
  {
  	if (length>2)
  	  pr[2]=num;
  	else
  	  throw std::out_of_range("it has no pr[2]");
  }
  T& w()
  {
  	if (length>3)
  	  return pr[3];
  	else
  	  throw std::out_of_range("it has no pr[3]");
  }
  void w(const T& num)
  {
  	if (length>3)
  	  pr[3]=num;
  	else
  	  throw std::out_of_range("it has no pr[3]");
  }
  const T& x() const
  {
  	return pr[0];
  }
  const T& y() const
  {
  	if (length>1)
  	  return pr[1];
  	else
  	  throw std::out_of_range("it has no pr[1]");
  }
  const T& z() const
  {
  	if (length>2)
  	  return pr[2];
  	else
  	  throw std::out_of_range("it has no pr[2]");
  }
  const T& w() const
  {
  	if (length>3)
  	  return pr[3];
  	else
  	  throw std::out_of_range("it has no pr[3]");
  }
  
private:
	T pr[N];
	const unsigned length=N;	
	
};

template<typename T,unsigned L1,unsigned L2>
auto make_point(const point<T,L1>& lt,const point<T,L2>& rt)->point<T,L1+L2>
{
	point<T,L1+L2> p;
	for (unsigned i=0;i<L1;i++)
	{
		p[i]=lt[i];
	}
	//const unsigned all=L1+L2;
	for (unsigned i=0;i<L2;i++)
	{
		p[i+L1]=rt[i];
	}
	return p;
}
template<typename T,unsigned Len>
auto make_point(const std::array<T,Len>& ary)
{
	point<T,Len> p;
	for (unsigned i=0;i<Len;i++)
	{
		p[i]=ary[i];
	}
	return p;
}
template<typename T,unsigned Len>
auto make_point(T (&ary)[Len])
{
	point<T,Len> p;
	for (unsigned i=0;i<Len;i++)
	{
		p[i]=ary[i];
	}
	return p;
}


//成员函数测试
#define HAS_MEMBER(member)\
    template<typename T, typename... Args>struct has_member_##member\
{\
private:\
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
    template<typename U> static auto Check(...) -> decltype(std::false_type()); \
public:\
static const bool value = std::is_same<decltype(Check<T>(0)), std::true_type>::value; \
}; \

//noncobyable
struct noncopyable
{
  noncopyable()=default;
	noncopyable(const noncopyable&) =delete;
	noncopyable& operator=(const noncopyable&)=delete;
	~noncopyable()=default;
};

//print
struct _endl
{}endl;

HAS_MEMBER(begin);
HAS_MEMBER(end);

class print_t:noncopyable
{
public:
	print_t()=default;
	// end_t="\n";
	std::string end_t()
	{
		return ends;
	}
	void end_t(std::string end)
	{
		ends=end;
	}
	
	template<typename T,unsigned Len>
  void operator()(const point<T,Len> p)
  {
  	unsigned i=0;
	  for (;i<Len-1;i++)
	  {
		  std::cout<<p[i]<<ends;
	  }
	  std::cout<<p[i];
  }
  template<typename T,unsigned Len,typename... Args>
  void operator()(const point<T,Len> p,Args... args)
  {
  	unsigned i=0;
	  for (;i<Len-1;i++)
	  {
		  std::cout<<p[i]<<ends;
	  }
	  std::cout<<p[i];
	  (*this)(args...);
  }
	
	
	template<typename T,std::size_t Len>
	void operator()(T (&data)[Len])
	{
		std::size_t i=0,l=Len-1;
		for (;i<l;i++)
		{
			std::cout<<data[i]<<ends;
		}
		std::cout<<data[i];
	}
	template<typename T,std::size_t Len,typename... Args>
	void operator()(T (&data)[Len],Args... args)
	{
		std::size_t i=0,l=Len-1;
		for (;i<l;i++)
		{
			std::cout<<data[i]<<ends;
		}
		std::cout<<data[i];
		(*this)(args...);
	}
	template<typename T>
	void operator()(std::initializer_list<T> itl)
	{
		std::size_t x=0,l=itl.size();
		for (auto i:itl)
		{
			std::cout<<i<<(++x<l?ends:"");
		}
	}
	template<typename T,typename... Args>
	void operator()(std::initializer_list<T> itl,Args... args)
	{
		std::size_t x=0,l=itl.size();
		for (auto i:itl)
		{
			std::cout<<i<<(++x<l?ends:"");
		}
		(*this)(args...);
	}
	
	template<typename T>
	void operator()(const T& data)
	{
		show<T,has_member_begin<T>::value&&has_member_end<T>::value> s(ends);
		s(data);
	}
	template<typename T,typename... Args>
  void operator()(const T& data,Args... args)
	{
    show<T,has_member_begin<T>::value&&has_member_end<T>::value> s(ends);
		s(data);
		(*this)(args...);
  }
  //template<>
  void operator()(_endl)
  {
  	std::cout<<std::endl;
  }
  template<typename... Args>
  void operator()(_endl,Args... args)
  {
  	std::cout<<std::endl;
    (*this)(args...);
  }
  void operator()(const std::string& data)
  {
  	std::cout<<data;
  }
  template<typename... Args>
  void operator()(const std::string& data,Args... args)
  {
	  std::cout<<data;
	  (*this)(args...);
  }
  void operator()(const char* data)
  {
  	std::cout<<data;
  }
  template<typename... Args>
  void operator()(const char* data,Args... args)
  {
  	std::cout<<data;
  	(*this)(args...);
  }
  /*void operator()(const wchar_t* data)
  {
  	std::cout<<data;
  }
  template<typename... Args>
  void operator()(const wchar_t* data,Args... args)
  {
  	std::cout<<data;
  	(*this)(args...);
  }*/
private:
/*  template<typename T>
  void showWithArray(const T& data)
  {
  	for (const auto& i:data)
  	{
  		std::cout<<i<<end_t;
  	}
  }*/
  
  std::string ends="\n";
  
  template<typename T,bool B>
  struct show
  {};
  
  template<typename T>
  struct show<T,false>
  {
  	show()=default;
  	show(std::string) noexcept
  	{}
  	void operator()(const T& data)
  	{
  		std::cout<<data;
  	}
  };
  
  template<typename T>
  struct show<T,true>
  {
  	show()=default;
  	show(std::string s):end_s(s)
  	{}
    void operator()(const T& datas)
  	{
  		std::size_t l=0,x=datas.size();
  		for (auto i:datas)
  		{
  			std::cout<<i<<(++l<x?end_s:"");
  		}
  	}
  private:
  	std::string end_s="\n";
  };
  
  /*template<typename T>
  void showNoArray(const T& data)
  {
  	std::cout<<data;
  }*/
  
  
}print;




//log
class log_t:noncopyable
{
public:
	log_t()
	{
		ofs.open(getName(),std::ofstream::app);
	}
	log_t(unsigned num,std::string beginstr="",std::string endstr=""):l(num),begins(beginstr),ends(endstr)
	{
	   ofs.open(getName(),std::ofstream::app);
	  if (begins!="")
	    ofs<<begins<<std::endl;
	}
	log_t(std::string filePathName,unsigned num=0,std::string beginstr="",std::string endstr=""):ofpath(filePathName),l(num),begins(beginstr),ends(endstr)
	{
			ofs.open(getName(),std::ofstream::app);
		if (begins!="")
		  ofs<<begins<<std::endl;
	}
	
	template<typename T>
	void operator()(const T& data)
	{
		ofs<<data<<std::endl;
	}
	template<typename T,typename... Args>
	void operator()(const T& data,Args... args)
	{
		ofs<<data;
		(*this)(args...);
  }
  //template<>
  void operator()(_endl)
  {
  	ofs<<std::endl;
  }
  template<typename... Args>
  void operator()(_endl,Args... args)
  {
  	ofs<<std::endl;
    (*this)(args...);
  }
  template<typename T>
  log_t& operator<<(const T& data)
  {
  	(*this)(data);
  	return *this;
  }
  void clearLog()
  {
  	ofs.close();
  	ofs.open(getName(),std::ostream::out);
  	if (begins!="")
  	  ofs<<begins<<std::endl;
  }
	
	void changeNum(unsigned num)
	{
		if (ends!="")
		  ofs<<ends<<std::endl;
		ofs.close();
		l=num;
		ofs.open(getName(),std::ofstream::app);
	  if (begins!="")
		  ofs<<begins<<std::endl;
	}
	
	~log_t()
	{
		if (ends!="")
		  ofs<<ends<<std::endl;
		ofs.close();
	}
	
private:
  std::string begins="",ends="";
	std::string ofpath="log";
	unsigned l=0;
	std::ofstream ofs;
	std::string getName()
	{
		std::string temp=ofpath;
		return temp.append("_").append(tstring(long(l))).append(".log");
	}
}log;

}
#endif