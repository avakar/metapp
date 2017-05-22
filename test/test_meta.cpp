#include <avakar/meta.h>
#include <mutest/test.h>

namespace meta = avakar::meta;

TEST("a list can be empty")
{
	using L = meta::list<>;
	chk meta::length<L>::value == 0;
}

TEST("a list can be non-empty")
{
	using L = meta::list<int>;
	chk meta::length<L>::value == 1;
}

TEST("a list can have multiple elements")
{
	using L = meta::list<int, long>;
	chk meta::length<L>::value == 2;
}

TEST("a list can have repeating elements")
{
	using L = meta::list<int, int>;
	chk meta::length<L>::value == 2;
}

TEST("contains works")
{
	chk !meta::contains<int, meta::list<>>::value;
	chk !meta::contains<int, meta::list<long>>::value;
	chk meta::contains<int, meta::list<int>>::value;
	chk meta::contains<int, meta::list<int, int>>::value;
	chk meta::contains<int, meta::list<int, long>>::value;
	chk !meta::contains<int, meta::list<long, long>>::value;
}

TEST("contains_unique works")
{
	chk !meta::contains_unique<int, meta::list<>>::value;
	chk !meta::contains_unique<int, meta::list<long>>::value;
	chk meta::contains_unique<int, meta::list<int>>::value;
	chk !meta::contains_unique<int, meta::list<int, int>>::value;
	chk meta::contains_unique<int, meta::list<int, long>>::value;
	chk !meta::contains_unique<int, meta::list<long, long>>::value;
}

TEST("first_index_of works")
{
	chk meta::first_index_of<int, meta::list<int>>::value == 0;
	chk meta::first_index_of<int, meta::list<int, long>>::value == 0;
	chk meta::first_index_of<int, meta::list<int, int>>::value == 0;
	chk meta::first_index_of<int, meta::list<long, int>>::value == 1;
}

TEST("index_of works")
{
	chk meta::index_of<int, meta::list<int>>::value == 0;
	chk meta::index_of<int, meta::list<int, long>>::value == 0;
	chk meta::index_of<int, meta::list<long, int>>::value == 1;
}

TEST("sub works")
{
	chk std::is_same<meta::sub_t<meta::list<int>, 0>, int>::value;
	chk std::is_same<meta::sub_t<meta::list<int, long>, 0>, int>::value;
	chk std::is_same<meta::sub_t<meta::list<long, int>, 0>, long>::value;
	chk std::is_same<meta::sub_t<meta::list<int, int>, 0>, int>::value;
	chk std::is_same<meta::sub_t<meta::list<int, int>, 1>, int>::value;
	chk std::is_same<meta::sub_t<meta::list<int, long>, 1>, long>::value;
}

TEST("concat works")
{
	chk std::is_same<meta::concat_t<>, meta::list<>>::value;
	chk std::is_same<meta::concat_t<int>, meta::list<int>>::value;
	chk std::is_same<meta::concat_t<int, long>, meta::list<int, long>>::value;
	chk std::is_same<meta::concat_t<meta::list<int>, long>, meta::list<int, long>>::value;
	chk std::is_same<meta::concat_t<meta::list<int, long>>, meta::list<int, long>>::value;
	chk std::is_same<meta::concat_t<meta::list<int, long>, meta::list<char, short>>,
		meta::list<int, long, char, short>>::value;
}

TEST("choose_overload_t works")
{
	chk std::is_same<meta::choose_overload_t<int, meta::list<int, char>>, int>::value;
}
