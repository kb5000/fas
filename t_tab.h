#pragma  once
#include<type_traits>
namespace fas
{
	//type_con
	template<typename T, typename... Args>
	struct type_con
	{
		typedef T type;
		typedef type_con<Args...> remain_t;
		enum :unsigned {
			size = sizeof...(Args)+1
		};
	};
	template<typename T>
	struct type_con<T>
	{
		typedef T type;
		typedef void remain_t;
		enum :unsigned {
			size = 1
		};
	};
	//get_type
	template<typename con, unsigned place>
	struct get_type
	{
		typedef typename get_type<typename con::remain_t, place - 1>::type type;
	};
	template<typename con>
	struct get_type<con, 0>
	{
		typedef typename con::type type;
	};
	template<unsigned place>
	struct get_type<void, place>
	{
		typedef void type;
	};
	template<>
	struct get_type<void, 0>
	{
		typedef void type;
	};
	//popf
	template<typename con>
	struct pop_front
	{
		typedef typename con::remain_t type;
	};
	//pushf
	template<typename T, typename... Ts>
	struct push_front
	{};
	template<typename T, typename... Ts>
	struct push_front<type_con<Ts...>, T>
	{
		typedef type_con<T, Ts...> type;
	};
	//pushb
	template<typename T, typename... Ts>
	struct push_back
	{};
	template<typename T, typename... Ts>
	struct push_back<type_con<Ts...>, T>
	{
		typedef type_con<Ts..., T> type;
	};
	//insert


	template<typename T, unsigned P, typename... Ts>
	struct insertb
	{
		typedef type_con<Ts..., T> type;
	};

	template<typename T, unsigned P, typename Tb>
	struct insertb<void, P, T, type_con<Tb>>
	{
		typedef type_con<Tb, T> type;
	};
	template<typename T, typename Ts, unsigned P, typename Tb>
	struct insertb<type_con<Ts>, P, T, type_con<Tb>>
	{
		typedef type_con<Tb, Ts, T> type;
	};
	template<typename T, typename Ts, typename Tb>
	struct insertb<type_con<Ts>, 0, T, type_con<Tb>>
	{
		typedef type_con<Tb, T, Ts> type;
	};
	template<typename T, typename... Ts, typename Tb>
	struct insertb<type_con<Ts...>, 0, T, type_con<Tb>>
	{
		typedef type_con<Tb, T, Ts...> type;
	};

	template<typename T, unsigned P, typename... Tb>
	struct insertb<void, P, T, type_con<Tb...>>
	{
		typedef type_con<Tb..., T> type;
	};
	template<typename T, typename... Ts>
	struct insertb<type_con<Ts...>, 0, T>
	{
		typedef type_con<T, Ts...> type;
	};
	template<typename T, typename Ts>
	struct insertb<type_con<Ts>, 1, T>
	{
		typedef type_con<Ts, T> type;
	};
	template<typename T, typename Ts, unsigned P, typename... Tb>
	struct insertb<type_con<Ts>, P, T, type_con<Tb...>>
	{
		typedef type_con<Tb..., Ts, T> type;
	};
	template<typename T, typename Ts, typename... Tb>
	struct insertb<type_con<Ts>, 0, T, type_con<Tb...>>
	{
		typedef type_con<Tb..., T, Ts> type;
	};
	template<typename T, typename... Ts, typename... Tb>
	struct insertb<type_con<Ts...>, 0, T, type_con<Tb...>>
	{
		typedef type_con<Tb..., T, Ts...> type;
	};
	template<typename T, typename... Ts, unsigned P, typename... Tb>
	struct insertb<type_con<Ts...>, P, T, type_con<Tb...>>
	{
		typedef typename type_con<Ts...>::type con;
		typedef typename push_back<type_con<Tb...>, con>::type pust;
		typedef typename type_con<Ts...>::remain_t rmt;
		using type = typename insertb<rmt, P - 1, T, pust>::type;
	};
	template<typename T, unsigned P, typename... Ts>
	struct insertb<type_con<Ts...>, P, T>
	{
		typedef typename type_con<Ts...>::type con;
		typedef typename type_con<Ts...>::remain_t rmt;
		using type = typename insertb<rmt, P - 1, T, type_con<con>>::type;
	};


	template<typename T, unsigned P, typename... Ts>
	struct insert
	{};
	template<typename T, unsigned P, typename... Ts>
	struct insert<type_con<Ts...>, P, T>
	{
		typedef typename insertb<type_con<Ts...>, P, T>::type type;
	};


	//erase

	template<typename T, unsigned P, typename... Ts>
	struct eraseb {};

	template<typename... Ts, unsigned P, typename T>
	struct eraseb < type_con<Ts...>,
		template<typename... Ts, unsigned P>
	struct eraseb<type_con<Ts...>, P>
	{
		typedef typename type_con<Ts...>::remain_t rmt;
		typedef typename type_con<Ts...>::type cont;
		typedef eraseb<rmt, P - 1, cont> type;
	};


	template<typename T, unsigned P>
	struct erase {};
	template<typename... Ts, unsigned P>
	struct erase<type_con<T...>, P>
	{

	};

}
