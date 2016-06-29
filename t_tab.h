#pragma  once
#include<type_traits>
namespace fas
{
//nullcon
struct null_con
{
	typedef null_con type;
	typedef null_con remain_t;
	enum :unsigned{size=0};
};
//type_con
template<typename T,typename... Args>
struct type_con
{
	typedef T type;
	typedef type_con<Args...> remain_t;
	enum :unsigned{size=sizeof...(Args)+1};
};
template<typename T>
struct type_con<T>
{
	typedef T type;
	typedef null_con remain_t;
	enum :unsigned{size=1};
};

//get_type
template<typename con,unsigned place>
struct get_type
{
	typedef typename get_type<typename con::remain_t,place-1>::type type;
};
template<typename con>
struct get_type<con,0>
{
	typedef typename con::type type;
};
template<unsigned place>
struct get_type<null_con,place>
{
	typedef null_con type;
};
template<>
struct get_type<null_con,0>
{
	typedef null_con type;
};
//popf
template<typename con>
struct pop_front
{
	typedef typename con::remain_t type;
};
//pushf
template<typename T,typename... Ts>
struct push_front
{};
template<typename T,typename... Ts>
struct push_front<type_con<Ts...>,T>
{
	typedef type_con<T,Ts...> type;
};
template<typename T>
struct push_front<null_con,T>
{
	typedef type_con<T> type;
};
//pushb
template<typename T,typename... Ts>
struct push_back
{};
template<typename T,typename... Ts>
struct push_back<type_con<Ts...>,T>
{
	typedef type_con<Ts...,T> type;
};
template<typename T>
struct push_back<null_con,T>
{
	typedef type_con<T> type;
};

//insert
template<typename T,unsigned P,typename... Ts>
struct insertb
{
	typedef type_con<Ts...,T> type;
};

template<typename T,unsigned P,typename Tb>
struct insertb<null_con,P,T,type_con<Tb>>
{
	typedef type_con<Tb,T> type;
};
template<typename T,typename Ts,unsigned P,typename Tb>
struct insertb<type_con<Ts>,P,T,type_con<Tb>>
{
	typedef type_con<Tb,Ts,T> type;
};
template<typename T,typename Ts,typename Tb>
struct insertb<type_con<Ts>,0,T,type_con<Tb>>
{
	typedef type_con<Tb,T,Ts> type;
};
template<typename T,typename... Ts,typename Tb>
struct insertb<type_con<Ts...>,0,T,type_con<Tb>>
{
	typedef type_con<Tb,T,Ts...> type;
};

template<typename T,unsigned P,typename... Tb>
struct insertb<null_con,P,T,type_con<Tb...>>
{
	typedef type_con<Tb...,T> type;
};
template<typename T,typename... Ts>
struct insertb<type_con<Ts...>,0,T>
{
	typedef type_con<T,Ts...> type;
};
template<typename T,typename Ts>
struct insertb<type_con<Ts>,1,T>
{
	typedef type_con<Ts,T> type;
};
template<typename T,typename Ts,unsigned P,typename... Tb>
struct insertb<type_con<Ts>,P,T,type_con<Tb...>>
{
	typedef type_con<Tb...,Ts,T> type;
};
template<typename T,typename Ts,typename... Tb>
struct insertb<type_con<Ts>,0,T,type_con<Tb...>>
{
	typedef type_con<Tb...,T,Ts> type;
};
template<typename T,typename... Ts,typename... Tb>
struct insertb<type_con<Ts...>,0,T,type_con<Tb...>>
{
	typedef type_con<Tb...,T,Ts...> type;
};
template<typename T,typename... Ts,unsigned P,typename... Tb>
struct insertb<type_con<Ts...>,P,T,type_con<Tb...>>
{
	typedef typename type_con<Ts...>::type con;
	typedef typename push_back<type_con<Tb...>,con>::type pust;
	typedef typename type_con<Ts...>::remain_t rmt;
	using type=typename insertb<rmt,P-1,T,pust>::type;
};
template<typename T,unsigned P,typename... Ts>
struct insertb<type_con<Ts...>,P,T>
{
	typedef typename type_con<Ts...>::type con;
	typedef typename type_con<Ts...>::remain_t rmt;
	using type=typename insertb<rmt,P-1,T,type_con<con>>::type;
};


template<typename T,unsigned P,typename... Ts>
struct insert
{};
template<typename T,unsigned P,typename... Ts>
struct insert<type_con<Ts...>,P,T>
{
	typedef typename insertb<type_con<Ts...>,P,T>::type type;
};


//erase

template<typename T,unsigned P,typename... Ts>
struct eraseb{};
template<typename... rm,typename T,typename... hd>
struct eraseb<type_con<rm...>,0,T,type_con<hd...>>
{
	typedef type_con<hd...,rm...> type;
};
template<typename... hd,unsigned P>
struct eraseb<null_con,P,type_con<hd...>>
{
	typedef type_con<hd...> type;
};
template<typename... rm,unsigned P,typename T,typename... hd>
struct eraseb<type_con<rm...>,P,T,type_con<hd...>>
{
	typedef typename type_con<rm...>::remain_t rmt;
	typedef typename type_con<rm...>::type cont;
	typedef typename push_back<type_con<hd...>,cont>::type ptype;
	typedef typename eraseb<rmt,P-1,cont,ptype>::type type;
};
template<typename... Ts>
struct eraseb<type_con<Ts...>,0>
{
	typedef typename type_con<Ts...>::remain_t type;
};
template<typename... Ts>
struct eraseb<type_con<Ts...>,1>
{
	typedef typename type_con<Ts...>::remain_t::remain_t rmtb;
	typedef typename type_con<Ts...>::type cont;
	typedef typename push_front<rmtb,cont>::type type;
};
template<typename A,typename B>
struct eraseb<type_con<A,B>,1>
{
	typedef type_con<A> type;
};
template<typename Ts>
struct eraseb<type_con<Ts>,0>
{
	typedef null_con type;
};
template<unsigned P>
struct eraseb<null_con,P>
{
	typedef null_con type;
};
template<typename... Ts,unsigned P>
struct eraseb<type_con<Ts...>,P>
{
	typedef typename type_con<Ts...>::remain_t rmta;
	typedef typename rmta::remain_t rmtb;
	typedef typename rmtb::type boolt;
	typedef typename type_con<Ts...>::type cont;
	typedef typename eraseb<rmtb,P-1,boolt,type_con<cont>>::type type;
};

template<typename T,unsigned P>
struct erase{};
template<typename... Ts,unsigned P>
struct erase<type_con<Ts...>,P>
{
	typedef typename eraseb<type_con<Ts...>,P>::type type;
};

//pop_back
template<typename T>
struct pop_back
{};
template<typename... Ts>
struct pop_back<type_con<Ts...>>
{
	//static const unsigned s=type_con<Ts...>::size-1;
	typedef typename erase<type_con<Ts...>,type_con<Ts...>::size-1>::type type;
};

}
