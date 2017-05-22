#ifndef AVAKAR_META_H
#define AVAKAR_META_H

#include <stdlib.h>
#include <type_traits>

namespace avakar {
namespace meta {

template <typename... Tn>
struct list;

template <typename L>
struct length;

template <typename T, typename L>
struct contains;

template <typename T, typename L>
struct contains_unique;

template <typename T, typename L>
struct first_index_of;

template <typename T, typename L>
struct index_of;

template <typename L, size_t I>
struct sub;

template <typename... Ln>
struct concat;

template <typename... L>
struct overload_sandbox;

template <typename L>
struct aligned_storage;



template <typename L, size_t I>
using sub_t = typename sub<L, I>::type;

template <typename... Ln>
using concat_t = typename concat<Ln...>::type;

template <typename T, typename L>
using choose_overload_t = decltype(overload_sandbox<L>::f(std::declval<T>()));

template <typename L>
using aligned_storage_t = typename aligned_storage<L>::type;

template <typename T>
struct item
{
	using type = T;
};

template <typename T, size_t I>
struct list_item
	: item<T>
{
	static const size_t index = I;
};

template <typename L, typename Skip = meta::list<>, typename Visitor, typename... Args>
auto visit(size_t index, Visitor && visitor, Args &&... args);

}
}

#include "../../src/meta_impl.h"

#endif // AVAKAR_META_H
