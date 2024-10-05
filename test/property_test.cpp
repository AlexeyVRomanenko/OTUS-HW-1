#define BOOST_TEST_MODULE property_tests

#include <boost/test/unit_test.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <memory>

#include <prop_react/property.h>

class CMovable
	: public std::wstring
{
	int _Value = 0;

public:
	CMovable()
		: _Value(0) {
		NDefCtors++;
	}

	CMovable(int v)
		: _Value(v) {
		NDefCtors++;
	}

	CMovable(const CMovable& rhs)
		: _Value(rhs._Value) {
		NCopyCtors++;
	}

	CMovable(CMovable&& rhs) noexcept
		: _Value(rhs._Value)
	{
		rhs._Value = 0;
		NMoveCtors++;
	}

	~CMovable() {
		NDtors++;
	}

	CMovable& operator=(const CMovable& rhs)
	{
		NCopyOps++;
		_Value = rhs._Value;
		return *this;
	}

	CMovable& operator=(CMovable&& rhs) noexcept
	{
		NMoveOps++;
		_Value = rhs._Value;	//emulate moving by moving value
		rhs._Value = 0;
		return *this;
	}

	bool operator ==(const CMovable& rhs) const
	{
		return _Value == rhs._Value;
	}

	bool IsInitialized() const
	{
		return _Value != 0;
	}

	void SetValue(int v)
	{
		_Value = v;
	}

	int GetValue() const
	{
		return _Value;
	}

	bool operator<(const CMovable& rhs) const
	{
		return _Value < rhs._Value;
	}

public:
	static int NDefCtors, NCopyCtors, NMoveCtors, NDtors, NCopyOps, NMoveOps;
};

int CMovable::NDefCtors = 0;
int CMovable::NCopyCtors = 0;
int CMovable::NMoveCtors = 0;
int CMovable::NDtors = 0;
int CMovable::NCopyOps = 0;
int CMovable::NMoveOps = 0;

#define BOOST_CHECK_MOVING_(ctors, cctors, mctors, dtors, cops, mops)	\
	BOOST_CHECK_EQUAL( CMovable::NDefCtors, ctors ); \
	BOOST_CHECK_EQUAL( CMovable::NCopyCtors, cctors ); \
	BOOST_CHECK_EQUAL( CMovable::NMoveCtors, mctors ); \
	BOOST_CHECK_EQUAL( CMovable::NDtors, dtors ); \
	BOOST_CHECK_EQUAL( CMovable::NCopyOps, cops ); \
	BOOST_CHECK_EQUAL( CMovable::NMoveOps, mops );

#define RESET_COUNTERS \
	CMovable::NDefCtors = CMovable::NCopyCtors = CMovable::NMoveCtors = CMovable::NDtors = \
	CMovable::NCopyOps = CMovable::NMoveOps = 0;

#define BOOST_CHECK_MOVING(ctors, cctors, mctors, dtors, cops, mops)	\
	BOOST_CHECK_EQUAL( ctors+cctors+mctors, dtors ); \
	BOOST_CHECK_MOVING_(ctors, cctors, mctors, dtors, cops, mops); \
	RESET_COUNTERS

using namespace prop_react;

BOOST_AUTO_TEST_CASE(simple_demo_test)
{
	property<int> first_prop;
	property<int> second_prop; // second_prop always equal first_prop * 5

	first_prop.add_subscriber(second_prop,
		[&](const property<int>& prop)
		{
			second_prop = prop.val() * 5;
		}
	);

	first_prop = 5;
	BOOST_CHECK(first_prop.val() == 5);
	BOOST_CHECK(second_prop.val() == 25);

	property<std::string> third_propery; // always str of second_prop

	second_prop.add_subscriber(third_propery,
		[&](const auto& i)
		{
			third_propery = std::to_string(i);
		}
	);

	first_prop = 10;
	BOOST_CHECK(first_prop.val() == 10);
	BOOST_CHECK(second_prop.val() == 50);
	BOOST_CHECK(third_propery.val() == "50");
}

BOOST_AUTO_TEST_CASE(cyclic_dependency_test)
{
	property<int> first_prop;
	property<int> second_prop;

	first_prop.add_subscriber(second_prop,
		[&second_prop](const auto& f)
		{
			second_prop = f;
		}
	);

	second_prop.add_subscriber(first_prop,
		[&first_prop](const auto& s)
		{
			first_prop = s;
		}
	);

	first_prop = 20;
	BOOST_CHECK(first_prop.val() == 20);
	BOOST_CHECK(second_prop.val() == 20);

	second_prop = 30;
	BOOST_CHECK(first_prop.val() == 30);
	BOOST_CHECK(second_prop.val() == 30);
}

BOOST_AUTO_TEST_CASE(harder_cyclic_dependency_test)
{
	property<int> first_prop;
	property<int> second_prop;
	property<int> third_prop;

	first_prop.add_subscriber(second_prop,
		[&second_prop](const auto& f)
		{
			second_prop = f;
		}
	);

	second_prop.add_subscriber(third_prop,
		[&third_prop](const auto& s)
		{
			third_prop = s;
		}
	);

	third_prop.add_subscriber(first_prop,
		[&first_prop](const auto& f)
		{
			first_prop = f;
		}
	);

	first_prop = 20;
	BOOST_CHECK(first_prop.val() == 20);
	BOOST_CHECK(second_prop.val() == 20);
	BOOST_CHECK(third_prop.val() == 20);

	second_prop = 30;
	BOOST_CHECK(first_prop.val() == 30);
	BOOST_CHECK(second_prop.val() == 30);
	BOOST_CHECK(third_prop.val() == 30);

	third_prop = 40;
	BOOST_CHECK(first_prop.val() == 40);
	BOOST_CHECK(second_prop.val() == 40);
	BOOST_CHECK(third_prop.val() == 40);
}

BOOST_AUTO_TEST_CASE(even_more_hard_cyclic_dependency_test)
{
	property<int> first_prop;
	property<int> second_prop;
	property<int> third_prop;
	property<int> firth_prop;

	first_prop.add_subscriber(second_prop,
		[&second_prop](const auto& f)
		{
			second_prop = f;
		}
	);

	second_prop.add_subscriber(third_prop,
		[&third_prop](const auto& s)
		{
			third_prop = s;
		}
	);

	third_prop.add_subscriber(firth_prop,
		[&firth_prop](const auto& f)
		{
			firth_prop = f;
		}
	);

	firth_prop.add_subscriber(second_prop,
		[&second_prop](const auto& f)
		{
			second_prop = f;
		}
	);


	first_prop = 20;
	BOOST_CHECK(first_prop.val() == 20);
	BOOST_CHECK(second_prop.val() == 20);
	BOOST_CHECK(third_prop.val() == 20);
	BOOST_CHECK(firth_prop.val() == 20);

	second_prop = 30;
	BOOST_CHECK(first_prop.val() == 20);
	BOOST_CHECK(second_prop.val() == 30);
	BOOST_CHECK(third_prop.val() == 30);
	BOOST_CHECK(firth_prop.val() == 30);

	third_prop = 40;
	BOOST_CHECK(first_prop.val() == 20);
	BOOST_CHECK(second_prop.val() == 40);
	BOOST_CHECK(third_prop.val() == 40);
	BOOST_CHECK(firth_prop.val() == 40);

	firth_prop = 50;
	BOOST_CHECK(first_prop.val() == 20);
	BOOST_CHECK(second_prop.val() == 50);
	BOOST_CHECK(third_prop.val() == 50);
	BOOST_CHECK(firth_prop.val() == 50);
}

BOOST_AUTO_TEST_CASE(scoped_connection_test)
{
	{
		bool is_fired = false;
		property<int> first_prop;

		{
			double b = 0;
			auto conn = first_prop.add_subscriber([&b, &is_fired](const auto& first) { b = first * 10.; is_fired = true; });

			first_prop = 10;

			BOOST_CHECK(b == 100.);
			BOOST_CHECK(is_fired);
			is_fired = false;
		}

		first_prop = 20; // must be no failure
		BOOST_CHECK(!is_fired);
	}
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
	property<int> first_prop = 20;
	property<double> slave_prop = 0.;
	first_prop.add_subscriber(slave_prop, [&slave_prop](const auto&) {slave_prop = 100.; });

	property<int> second_prop(first_prop);
	BOOST_CHECK(second_prop.val() == 20);

	second_prop = 20;
	BOOST_CHECK(slave_prop.val() == 0);
}

BOOST_AUTO_TEST_CASE(hard_dependency_test)
{
	property<int> first = 0;
	property<int> second = 0;
	property<int> third = 0;

	first.add_subscriber(third, [&third](const auto& f)
		{
			third = third.val() + 1;
		}
	);
	first.add_subscriber(second, [&second](const auto& f)
		{
			second = second.val() + 1;
		}
	);
	second.add_subscriber(third, [&third](const auto& f)
		{
			third = third.val() + 1;
		}
	);

	first = 5;

	BOOST_CHECK(second.val() == 1);
	BOOST_CHECK(third.val() == 2);
}

BOOST_AUTO_TEST_CASE(container_test)
{
	std::vector<property<int>> vec(5, property<int>(0));

	for (size_t i = 0; i < vec.size() - 1; i++)
	{
		vec[i].add_subscriber(vec[i + 1], [&a = vec[i + 1]](const auto& p)
			{ a = p; }
		);

		for (size_t j = 0; j < i; j++)
		{
			vec[i + 1].add_subscriber(vec[j], [&a = vec[j]](const auto& p)
				{ a = p; }
			);
		}
	}

	vec[0] = 5;
	for (const auto& prop : vec)
	{
		BOOST_CHECK(prop.val() == 5);
	}

	*vec.rbegin() = 10;
	for (const auto& prop : vec)
	{
		BOOST_CHECK(prop.val() == 10);
	}
}

BOOST_AUTO_TEST_CASE(property_over_class)
{
	property<std::vector<int>> vector_prop;
	property<size_t> vector_size_prop;

	vector_prop.add_subscriber(vector_size_prop, [&vector_size_prop](const auto& vp) { vector_size_prop = vp.size(); });

	vector_prop.var().push_back(1);
	vector_prop.var().push_back(2);

	BOOST_CHECK(vector_size_prop == 2);
}

BOOST_AUTO_TEST_CASE(property_over_class_scope)
{
	property<std::vector<int>> vector_prop;
	property<size_t> vector_size_prop;
	property<size_t> vector_changes_prop = 0;

	vector_prop.add_subscriber(vector_size_prop, [&vector_size_prop](const auto& vp) { vector_size_prop = vp.size(); });
	vector_prop.add_subscriber(vector_changes_prop, [&vector_changes_prop](const auto& vp) { vector_changes_prop = vector_changes_prop + 1; });

	{
		property<std::vector<int>>::value_scoped_holder holder = vector_prop.var();
		std::vector<int>& vec = static_cast<std::vector<int> &>(holder);
		vec.push_back(1);
		vec.push_back(2);
	}

	BOOST_CHECK(vector_size_prop == 2);
	BOOST_CHECK(vector_changes_prop == 1);
}

BOOST_AUTO_TEST_CASE(custom_operator_test)
{
	using namespace prop_react;

	bool is_fired = false;
	property<int> int_prop = 5;
	auto conn = int_prop.add_subscriber([&is_fired](const auto&) {is_fired = true; });
	{
		int_prop += 5;
		BOOST_CHECK(is_fired);
		BOOST_CHECK(int_prop == 10);

		is_fired = false;
	}

	{
		int_prop *= 5;
		BOOST_CHECK(is_fired);
		BOOST_CHECK(int_prop == 50);

		is_fired = false;
	}

	{
		int_prop /= 5;
		BOOST_CHECK(is_fired);
		BOOST_CHECK(int_prop == 10);

		is_fired = false;
	}

	{
		int_prop -= 5;
		BOOST_CHECK(is_fired);
		BOOST_CHECK(int_prop == 5);

		is_fired = false;
	}

	{
		property<int> sec_prop = 10;
		sec_prop += int_prop;
		BOOST_CHECK(sec_prop == 15);
		BOOST_CHECK(!is_fired);
	}
}

BOOST_AUTO_TEST_CASE(property_equality_test_assign_test)
{
	using namespace prop_react;
	bool fired = false;
	{
		property<int> prop = 5;
		auto conn = prop.add_subscriber([&fired](const auto&) {fired = true; });

		prop = 10;
		BOOST_CHECK(fired == true);
		fired = false;

		prop = 10;
		BOOST_CHECK(fired == false);
	}

	{
		struct SomeStructWithEqualityOperator
		{
			int a = 10;
			void SetA(int _a) { a = _a; }

			bool operator ==(const SomeStructWithEqualityOperator& rhs) const { return a == rhs.a; }
			bool operator !=(const SomeStructWithEqualityOperator& rhs) const { return !(*this == rhs); }
		};

		property<SomeStructWithEqualityOperator> s;
		auto conn = s.add_subscriber([&fired](const auto&) {fired = true; });

		s.var().SetA(15);
		BOOST_CHECK(s.val().a == 15);
		BOOST_CHECK(fired == true);
		fired = false;

		s = SomeStructWithEqualityOperator{ 15 };
		BOOST_CHECK(fired == false);
	}

	{
		struct SomeStructWithoutEqualityOperator
		{
			int a = 10;
			void SetA(int _a) { a = _a; }
		};

		property<SomeStructWithoutEqualityOperator> s;
		auto conn = s.add_subscriber([&fired](const auto&) {fired = true; });

		s.var().SetA(15);
		BOOST_CHECK(s.val().a == 15);
		BOOST_CHECK(fired == true);
		fired = false;

		s.var().SetA(15);
		BOOST_CHECK(fired == true);
	}
}

BOOST_AUTO_TEST_CASE(var_test)
{
	using namespace prop_react;

	{
		property<std::vector<int>> vec;
		bool is_fired = false;

		vec.var().push_back(1);
		BOOST_CHECK(vec.val().size() == 1);

		auto conn = vec.add_subscriber([&](const auto&) {is_fired = true; });

		{
			auto scoped_holder = vec.var();
			BOOST_CHECK_THROW(vec = std::vector<int>{}, std::logic_error); // var moves value into scoped holder
			BOOST_CHECK_NO_THROW(vec.val()); // var moves value into scoped holder
			BOOST_CHECK(is_fired == false);

			scoped_holder.push_back(2);
			BOOST_CHECK(is_fired == false);
			scoped_holder.push_back(3);
			BOOST_CHECK(is_fired == false);
		}

		BOOST_CHECK(is_fired == true);
		BOOST_CHECK(vec.val().size() == 3);
	}

	{
		bool is_fired = false;
		property<CMovable> movable;

		auto conn = movable.add_subscriber([&is_fired](const auto&) {is_fired = true; });

		{
			auto scoped_holder = movable.var();
			scoped_holder.SetValue(scoped_holder.GetValue() + 1);
		}

		BOOST_CHECK(is_fired);

		BOOST_CHECK_MOVING_(1, 1, 0, 1, 0, 1);
	}
	RESET_COUNTERS;

	{
		bool is_fired = false;
		property<CMovable> movable;

		auto conn = movable.add_subscriber([&is_fired](const auto&) {is_fired = true; });

		{
			auto scoped_holder = movable.var();
			scoped_holder.SetValue(scoped_holder.GetValue());
		}

		BOOST_CHECK(!is_fired); // no fire because object is not changed

		BOOST_CHECK_MOVING_(1, 1, 0, 1, 0, 0); // no move operator because object is not changed
	}
	RESET_COUNTERS;

	{
		struct CMovableWithoutEqualityOperator : public CMovable
		{
			bool operator == (const CMovableWithoutEqualityOperator& rhs) const = delete;

			CMovableWithoutEqualityOperator() = default;

			CMovableWithoutEqualityOperator(const CMovableWithoutEqualityOperator& rhs)
				: CMovable(static_cast<const CMovable&>(rhs))
			{}

			CMovableWithoutEqualityOperator(CMovableWithoutEqualityOperator&& rhs) noexcept
				: CMovable(static_cast<CMovable&&>(rhs))
			{}

			CMovableWithoutEqualityOperator& operator=(const CMovableWithoutEqualityOperator& rhs)
			{
				CMovable::operator=(static_cast<const CMovable&>(rhs));
				return *this;
			}
			CMovableWithoutEqualityOperator& operator=(CMovableWithoutEqualityOperator&& rhs) noexcept
			{
				CMovable::operator=(static_cast<CMovable&&>(rhs));
				return *this;
			}
		};
		static_assert(!prop_react::equality_comparable<CMovableWithoutEqualityOperator>);

		bool is_fired = false;
		property<CMovableWithoutEqualityOperator> movable;

		auto conn = movable.add_subscriber([&is_fired](const auto&) {is_fired = true; });

		{
			auto scoped_holder = movable.var();
			scoped_holder.SetValue(scoped_holder.GetValue());
		}

		BOOST_CHECK(is_fired); // no fire because there is no equality compare operator

		BOOST_CHECK_MOVING_(1, 0, 1, 1, 0, 1);
	}
	RESET_COUNTERS;

	{
		property<CMovable> movable;

		CMovable rhs(5);
		movable = std::move(rhs);

		BOOST_CHECK_MOVING_(2, 0, 0, 0, 0, 1);
	}
	RESET_COUNTERS;
}

BOOST_AUTO_TEST_CASE(test_callbacks_with_prop_signature)
{
	property<int> a = 0;
	property<int> b;

	bool is_fired = false;
	auto conn = a.add_subscriber([&is_fired](const int&) {is_fired = true; });

	a = 10;
	BOOST_CHECK(is_fired == true);

	a.add_subscriber(b, [&b](const int& a) { b = a + 5; });

	a = 11;
	BOOST_CHECK(b == 16);
}

BOOST_AUTO_TEST_CASE(test_smart_val)
{
	property<std::string> a("");

	{
		auto scoped_holder = a.var();
		scoped_holder = a.val() + "a";

		BOOST_CHECK(scoped_holder == "a");
		BOOST_CHECK(a.val() == "a");

		scoped_holder = a.val() + "b";
		BOOST_CHECK(scoped_holder == "ab");
		BOOST_CHECK(a.val() == "ab");
	}
}

BOOST_AUTO_TEST_CASE(test_var_vector_clear)
{
	property<std::vector<int>> a = std::vector<int>{ 1, 2 };
	size_t b = 2;
	auto conn = a.add_subscriber([&b](const std::vector<int>& prop) {
		b = prop.size();
		});

	{
		auto scoped_holder = a.var();
		scoped_holder.clear();
	}

	BOOST_CHECK(b == 0);
}

BOOST_AUTO_TEST_CASE(compilate_inheritance_under_prop)
{
	class A : public property<int>
	{
	};

	class B
	{
		int b;
	};

	class C : public property<B>
	{
	};
}

BOOST_AUTO_TEST_CASE(check_dummy_usage)
{
	property<int> prop;

	BOOST_CHECK_THROW(prop.add_subscriber(prop, std::function<void(const int&)>{}), std::logic_error);
	BOOST_CHECK_THROW(prop.add_dependency(prop), std::logic_error);
}

BOOST_AUTO_TEST_CASE(check_boost_function)
{
	property<int> master = 0;
	int slave = 0;

	boost::function<void(const int&)> callback = boost::bind(
		+[](const int& master, int& slave)
		{
			slave = master + 1;
		},
		boost::placeholders::_1,
		boost::ref(slave)
	);

	auto conn = master.add_subscriber(callback);

	master = 5;
	BOOST_CHECK(master == 5);
	BOOST_CHECK(slave == 6);
}

static int _i = 0;

void functor_callback(const int&)
{
	_i += 1;
}

BOOST_AUTO_TEST_CASE(check_functor)
{
	property<int> master = 0;

	auto conn = master.add_subscriber(functor_callback);

	master = 5;
	BOOST_CHECK(_i == 1);
}

BOOST_AUTO_TEST_CASE(test_bind_equal)
{
	property<int> a = 0;
	property<int> b = 1;

	prop_react::bind_equal(a, b);

	BOOST_CHECK_EQUAL(b.val(), 0);
	BOOST_CHECK_EQUAL(a.val(), b.val());

	a = 5;

	BOOST_CHECK_EQUAL(b.val(), 5);
	BOOST_CHECK_EQUAL(a.val(), b.val());

	b = 10;

	BOOST_CHECK_EQUAL(a.val(), 10);
	BOOST_CHECK_EQUAL(a.val(), b.val());
}

BOOST_AUTO_TEST_CASE(cast_test)
{
	enum some_enum : int
	{
		a = 0,
		b = 1
	};

	{
		property<some_enum> enum_prop = some_enum::a;
		property<int>& int_prop = static_cast<property<int> &>(enum_prop);
		BOOST_CHECK_EQUAL(int_prop, 0);
		enum_prop = some_enum::b;
		BOOST_CHECK_EQUAL(int_prop, 1);

		bool is_fired = false;
		auto conn = enum_prop.add_subscriber([&is_fired](const auto&) {is_fired = true; });

		int_prop = 0;
		BOOST_CHECK(is_fired);
	}

	{
		auto enum_prop_ptr = std::make_shared<property<some_enum>>(some_enum::a);
		auto int_prop_ptr = std::reinterpret_pointer_cast<property<int>>(enum_prop_ptr);
		BOOST_CHECK_EQUAL(int_prop_ptr->val(), 0);
		*enum_prop_ptr = some_enum::b;
		BOOST_CHECK_EQUAL(int_prop_ptr->val(), 1);

		bool is_fired = false;
		auto conn = enum_prop_ptr->add_subscriber([&is_fired](const auto&) {is_fired = true; });

		*int_prop_ptr = 0;
		BOOST_CHECK(is_fired);
	}
	BOOST_CHECK(0);

	{ // prop_vm use case
		auto enum_prop_ptr = boost::make_shared<property<some_enum>>(some_enum::a);
		auto int_prop_ptr = boost::reinterpret_pointer_cast<property<int>>(enum_prop_ptr);
		BOOST_CHECK_EQUAL(int_prop_ptr->val(), 0);
		*enum_prop_ptr = some_enum::b;
		BOOST_CHECK_EQUAL(int_prop_ptr->val(), 1);

		bool is_fired = false;
		auto conn = enum_prop_ptr->add_subscriber([&is_fired](const auto&) {is_fired = true; });

		*int_prop_ptr = 0;
		BOOST_CHECK(is_fired);
	}
}

BOOST_AUTO_TEST_CASE(test_signature_with_slave)
{
	property<size_t> a1 = 0;
	property<size_t> a2 = 0;
	property<std::string> b;

	b.add_subscriber(
		a1,
		[](const std::string& master, property<size_t>& slave)
		{
			slave = master.size();
		}
	);

	b.get_eventable_base().add_subscriber(
		a2,
		[](const property<std::string>& master, property<size_t>& slave)
		{
			slave = master.val().size();
		}
	);

	b = "aaaa";
	BOOST_CHECK_EQUAL(a1.val(), 4);
	BOOST_CHECK_EQUAL(a2.val(), 4);
}

BOOST_AUTO_TEST_CASE(test_get_observable_field_)
{
	struct SomeStruct {
		int id;
		int value;

		auto operator <=>(const SomeStruct&) const = default;
	};
	property<std::vector<SomeStruct>> vecProp;

	const auto FindById = [](const std::vector<SomeStruct>& props, size_t id) -> const int*
		{
			for (const auto& prop : props)
				if (prop.id == id)
					return &prop.value;

			return nullptr;
		};

	std::shared_ptr<prop_react::property<int>> id1Prop = vecProp.get_observable_field_<int>(std::bind(FindById, std::placeholders::_1, 1));
	bool isFired = false;
	auto conn = id1Prop->add_subscriber([&isFired](const auto& prop) {
		isFired = true;
		});

	//
	vecProp.var().push_back({ 0, 0 });
	BOOST_CHECK(!isFired);

	//
	isFired = false;
	vecProp.var().push_back({ 1, 1 });
	BOOST_CHECK(isFired);
	BOOST_CHECK(id1Prop->val() == 1);

	//
	isFired = false;
	{
		auto scopedEdit = vecProp.var();
		scopedEdit[1].value = 2;
	}
	BOOST_CHECK(isFired);
	BOOST_CHECK(id1Prop->val() == 2);

	//
	*id1Prop = 3;
	BOOST_CHECK(vecProp.val()[1].value == 3);
}

struct custom_struct
{
	int a = 0;
	double b = 10.;

	bool operator ==(const custom_struct& rhs) const
	{
		return a == a && b == b;
	}
};

namespace prop_react
{
	template<>
	struct custom_equal<custom_struct> : std::true_type // if you want to always false equal operator you can inherit from std::false_type
	{
		constexpr static bool equals(const custom_struct& lhs, const custom_struct& rhs)
		{
			return false;
		}
	};
}

BOOST_AUTO_TEST_CASE(test_move)
{
	property<custom_struct> A, B;

	BOOST_CHECK(A.val() == B.val());

	bool is_fired = false;
	auto conn = A.add_subscriber([&is_fired](const auto&) { is_fired = true; });

	A = B.val();
	BOOST_CHECK(is_fired);
}

BOOST_AUTO_TEST_CASE(scoped_lock_test)
{
	bool is_fired = false;

	struct A : public eventable_base<A>
	{
		void fire() { _fire_event(); } // fire somehow
	};

	A some_class;

	auto conn = some_class.add_subscriber([&is_fired](const auto&) { is_fired = true; });

	{
		prop_react::scoped_lock lock(some_class);

		some_class.fire();
		BOOST_CHECK(!is_fired);

		some_class.fire();
		BOOST_CHECK(!is_fired);
	}

	BOOST_CHECK(is_fired);
	is_fired = false;

	{
		prop_react::scoped_lock lock(some_class);
		BOOST_CHECK(!is_fired);
	}

	BOOST_CHECK(!is_fired);
	is_fired = false;

	{
		property<int> prop = 0;
		auto conn = prop.add_subscriber([&is_fired](const auto&) { is_fired = true; });

		{
			prop_react::scoped_lock lock(prop);

			for (int i = 0; i < 10; i++)
			{
				prop += 1;
				BOOST_CHECK(!is_fired);
			}
		}

		BOOST_CHECK(is_fired);
		is_fired = false;

		BOOST_CHECK_EQUAL(prop.val(), 10);
		{
			prop_react::scoped_lock lock(prop);
			prop = 10;
		}

		BOOST_CHECK(!is_fired);
		is_fired = false;
	}
}