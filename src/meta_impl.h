#include <utility>
#include <assert.h>

namespace avakar {
namespace meta {

template <typename... Tn>
struct length<list<Tn...>>
	: std::integral_constant<size_t, sizeof...(Tn)>
{
};

template <typename T>
struct contains<T, list<>>
	: std::false_type
{
};

template <typename T, typename... Tn>
struct contains<T, list<T, Tn...>>
	: std::true_type
{
};

template <typename T, typename T0, typename... Tn>
struct contains<T, list<T0, Tn...>>
	: contains<T, list<Tn...>>
{
};

template <typename T>
struct contains_unique<T, list<>>
	: std::false_type
{
};

template <typename T, typename... Tn>
struct contains_unique<T, list<T, Tn...>>
	: std::integral_constant<bool, !contains<T, list<Tn...>>::value>
{
};

template <typename T, typename T0, typename... Tn>
struct contains_unique<T, list<T0, Tn...>>
	: contains_unique<T, list<Tn...>>
{
};

template <typename T, typename... Tn>
struct first_index_of<T, list<T, Tn...>>
	: std::integral_constant<size_t, 0>
{
};

template <typename T, typename T0, typename... Tn>
struct first_index_of<T, list<T0, Tn...>>
	: std::integral_constant<size_t, 1 + first_index_of<T, list<Tn...>>::value>
{
};

template <typename T, typename... Tn>
struct index_of<T, list<T, Tn...>>
	: std::enable_if<!contains<T, list<Tn...>>::value, std::integral_constant<size_t, 0>>::type
{
};

template <typename T, typename T0, typename... Tn>
struct index_of<T, list<T0, Tn...>>
	: std::integral_constant<size_t, 1 + index_of<T, list<Tn...>>::value>
{
};

template <typename T0, typename... Tn>
struct sub<list<T0, Tn...>, 0>
{
	using type = T0;
};

template <typename T0, typename... Tn, size_t I>
struct sub<list<T0, Tn...>, I>
{
	using type = sub_t<list<Tn...>, I - 1>;
};

template <>
struct concat<>
{
	using type = list<>;
};

template <typename T>
struct concat<T>
{
	using type = list<T>;
};

template <typename... Tn>
struct concat<list<Tn...>>
{
	using type = list<Tn...>;
};

template <typename... T1n, typename... T2n>
struct concat<list<T1n...>, list<T2n...>>
{
	using type = list<T1n..., T2n...>;
};

template <typename T1, typename... T2n>
struct concat<T1, list<T2n...>>
{
	using type = list<T1, T2n...>;
};

template <typename... T1n, typename T2>
struct concat<list<T1n...>, T2>
{
	using type = list<T1n..., T2>;
};

template <typename T1, typename T2>
struct concat<T1, T2>
{
	using type = list<T1, T2>;
};

template <typename L1, typename L2, typename... Ln>
struct concat<L1, L2, Ln...>
{
	using type = concat_t<concat_t<L1, L2>, Ln...>;
};

template <typename T0, typename... Tn>
struct overload_sandbox<list<T0, Tn...>>
	: overload_sandbox<list<Tn...>>
{
	using overload_sandbox<list<Tn...>>::f;
	static T0 f(T0);
};

template <typename T0>
struct overload_sandbox<list<T0>>
{
	static T0 f(T0);
};

template <typename... Tn>
struct overload_sandbox<list<void, Tn...>>
	: overload_sandbox<list<Tn...>>
{
};

template <typename L>
struct _storage_filter;

template <>
struct _storage_filter<list<>>
{
	using type = list<>;
};

template <typename T0, typename... Tn>
struct _storage_filter<list<T0, Tn...>>
{
	using type = concat_t<T0, typename _storage_filter<list<Tn...>>::type>;
};

template <typename... Tn>
struct _storage_filter<list<void, Tn...>>
	: _storage_filter<list<Tn...>>
{
};

template <typename T0, typename... Tn>
struct _storage_filter<list<T0 &, Tn...>>
{
	using type = concat_t<T0 *, typename _storage_filter<list<Tn...>>::type>;
};

template <typename L>
struct _aligned_storage_impl;

template <typename... Tn>
struct _aligned_storage_impl<list<Tn...>>
	: std::aligned_union<0, Tn...>
{
};

template <typename L>
struct aligned_storage
	: _aligned_storage_impl<typename _storage_filter<L>::type>
{
};

template <bool Skip, typename R, typename T, size_t I, typename Visitor, typename... Args>
struct _visit_one
{
	static R fn(Visitor && visitor, Args &&... args)
	{
		return std::forward<Visitor>(visitor)(list_item<T, I>(), std::forward<Args>(args)...);
	}
};

template <typename R, typename T, size_t I, typename Visitor, typename... Args>
struct _visit_one<true, R, T, I, Visitor, Args...>
{
	static constexpr std::nullptr_t fn = nullptr;
};

template <typename Skip, typename L, typename I, typename Visitor, typename... Args>
struct _visit_impl;

template <typename Skip, typename... Tn, size_t... In, typename Visitor, typename... Args>
struct _visit_impl<Skip, list<Tn...>, std::index_sequence<In...>, Visitor, Args...>
{
	using return_type = std::common_type_t<
		decltype(std::declval<Visitor>()(std::declval<list_item<Tn, In>>(), std::declval<Args>()...))...>;

	static return_type visit(Visitor && visitor, size_t index, Args &&... args)
	{
		using visit_fn = return_type(Visitor && visitor, Args &&... args);
		static visit_fn * const fns[] = {
			_visit_one<contains<Tn, Skip>::value, return_type, Tn, In, Visitor, Args...>::fn... };

		assert(index < sizeof...(Tn));
		auto fn = fns[index];

		assert(fn != nullptr);
		return fn(std::forward<Visitor>(visitor), std::forward<Args>(args)...);
	}
};

template <typename L, typename Skip, typename Visitor, typename... Args>
auto visit(size_t index, Visitor && visitor, Args &&... args)
{
	return _visit_impl<Skip, L, std::make_index_sequence<length<L>::value>, Visitor, Args...>
		::visit(std::forward<Visitor>(visitor), index, std::forward<Args>(args)...);
}

}
}
