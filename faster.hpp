#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <random>
#include <cmath>


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
    time_t gettimes(unit u)
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
        return time_t();
    }

    void sleep(time_t time,unit u)
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
    timer():t(std::chrono::high_resolution_clock::now()),tend(std::chrono::high_resolution_clock::now()) {}
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
decltype(auto) cswitch(A,Fuc f)
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
    template<typename Fuc, typename T>
    friend auto gaif(std::vector<T>& vcon,bool exp,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
    template<typename Fuc, typename T>
    friend auto gaif(std::vector<T> vcon,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
    template<typename Fuc, typename T, typename... Args>
    friend auto gaif(std::vector<T> vcon,bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
    template<typename Fuc,typename... Args>
    friend auto aif(bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type;
    bool f=false;
} __flagOfAif;
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
template<typename Fuc, typename T>
auto gaif(std::vector<T>& vcon,bool exp,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
    __flagOfAif.f=false;
    if (exp)
    {
        vcon.push_back(f());
    }
    return vcon;
}
template<typename Fuc, typename T>
auto gaif(std::vector<T> vcon,Fuc f)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
{
    if (!__flagOfAif.f)
    {
        vcon.push_back(f());
    }
    else
        __flagOfAif.f=false;
    return vcon;
}

template<typename Fuc, typename T, typename... Args>
auto gaif(std::vector<T> vcon,bool exp,Fuc f,const Args&... arg)->typename get_stype<decltype(f()),void,void,std::vector<decltype(f())>>::type
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
        for (unsigned i=0; i<p.length; i++)
        {
            pr[i]=p.pr[i];
        }
    }
    point(point<T,N>&& p) noexcept
    {
        for (unsigned i=0; i<length; i++)
        {
            pr[i]=p.pr[i];
        }
    }
    point(T dat)
    {
        for (unsigned i=0; i<length; i++)
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
        for (unsigned i=0; i<p.length; i++)
        {
            pr[i]=p.pr[i];
        }
        return *this;
    }
    point& operator=(point<T,N>&& p) noexcept
    {
        for (unsigned i=0; i<length; i++)
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
    for (unsigned i=0; i<L1; i++)
    {
        p[i]=lt[i];
    }
    //const unsigned all=L1+L2;
    for (unsigned i=0; i<L2; i++)
    {
        p[i+L1]=rt[i];
    }
    return p;
}
template<typename T,unsigned Len>
auto make_point(const std::array<T,Len>& ary)
{
    point<T,Len> p;
    for (unsigned i=0; i<Len; i++)
    {
        p[i]=ary[i];
    }
    return p;
}
template<typename T,unsigned Len>
auto make_point(T (&ary)[Len])
{
    point<T,Len> p;
    for (unsigned i=0; i<Len; i++)
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
{} endl;

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
        for (; i<Len-1; i++)
        {
            std::cout<<p[i]<<ends;
        }
        std::cout<<p[i];
    }
    template<typename T,unsigned Len,typename... Args>
    void operator()(const point<T,Len> p,Args... args)
    {
        unsigned i=0;
        for (; i<Len-1; i++)
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
        for (; i<l; i++)
        {
            std::cout<<data[i]<<ends;
        }
        std::cout<<data[i];
    }
    template<typename T,std::size_t Len,typename... Args>
    void operator()(T (&data)[Len],Args... args)
    {
        std::size_t i=0,l=Len-1;
        for (; i<l; i++)
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


} print;




//log

#ifdef START_LOG
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
} log;
#endif

//random
template<typename engine=std::default_random_engine>
class autoran
{
public:
    autoran()
    {
        auto s=std::chrono::high_resolution_clock::now();
        seed=std::chrono::duration_cast<std::chrono::nanoseconds>(s.time_since_epoch()).count();
        eg.seed(seed);
    }
    autoran(time_t tseed):seed(tseed)
    {}
    uint64_t getInt(uint64_t min=0,uint64_t max=uint64_max)
    {
        std::uniform_int_distribution<uint64_t> ui(min,max);
        return ui(eg);
    }
    double getDouble(double min=0,double max=1)
    {
        std::uniform_real_distribution<double> uf(min,max);
        return uf(eg);
    }
    void setSeed(time_t tseed)
    {
        seed=tseed;
    }
    time_t getSeed() const
    {
        return seed;
    }
    template<typename distribution>
    auto getCustom(distribution d)
    {
        return d(eg);
    }
private:
    engine eg;
    time_t seed;
    static const uint64_t uint64_max=-1;
};
using fasran=autoran<>;

//fequal
bool fequal(double dl,double dr,uint precision=2)
{
    static_assert(sizeof(double)==8,"This function needs sizeof(double)==8");
    uint64_t idl=*reinterpret_cast<uint64_t*>(&dl);
    uint64_t idr=*reinterpret_cast<uint64_t*>(&dr);
    uint64_t idx=idl-idr;
    if (idx>>63)
        idx-=uint64_t(1)<<63; //abs
    return idx<precision?true:false;
}

bool fequal(float dl,float dr,uint precision=2)
{
    static_assert(sizeof(float)==4,"This function needs sizeof(float)==4");
    uint32_t idl=*reinterpret_cast<uint32_t*>(&dl);
    uint32_t idr=*reinterpret_cast<uint32_t*>(&dr);
    uint32_t idx=idl-idr;
    if (idx>>31)
        idx-=uint32_t(1)<<31; //abs
    return idx<precision?true:false;
}


}
