/* (c) 2019 Галушин Павел Викторович, galushin@gmail.com

Данный файл -- часть библиотеки Grabin.

Grabin -- это свободной программное обеспечение: вы можете перераспространять ее и/или изменять ее
на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом
свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более
поздней версии.

Это программное обеспечение распространяется в надежде, что оно будет полезной, но БЕЗО ВСЯКИХ
ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ.
Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этим программным
обеспечение. Если это не так, см. https://www.gnu.org/licenses/.
*/

#include <grabin/optional/optional.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

namespace
{
    template <class T>
    struct optional_tester
    {
        static void check_types()
        {
            // Типы
            using Optional = grabin::optional<T>;

            static_assert(std::is_same<typename Optional::value_type, T>::value, "Must be same!");
        }

        static void check_types_and_empty_ctors()
        {
            optional_tester<T>::check_types();

            using Optional = grabin::optional<T>;

            // Конструктор без аргументов
            Optional const obj_1{};
            static_assert(noexcept(Optional()), "Must be noexcept");

            CHECK(obj_1.has_value() == false);
            static_assert(noexcept(obj_1.has_value()), "Must be noexcept");

            CHECK(static_cast<bool>(obj_1) == false);
            static_assert(noexcept(static_cast<bool>(obj_1)), "Must be noexcept");

            static_assert(std::is_convertible<Optional, bool>::value == false,
                          "Must not be implicitly convertible to bool");

            CHECK_THROWS_AS(obj_1.value(), grabin::bad_optional_access);

            // Конструктор из nullopt_t
            Optional const obj_2{grabin::nullopt};
            static_assert(noexcept(Optional(grabin::nullopt)), "Must be noexcept");

            CHECK(obj_2.has_value() == false);
            CHECK(static_cast<bool>(obj_2) == false);

            CHECK_THROWS_AS(obj_1.value(), grabin::bad_optional_access);
        }

        static void check_emplace_with_no_args()
        {
            using Optional = grabin::optional<T>;
            // @todo может ли быть constexpr?

            Optional obj;

            CHECK(obj.has_value() == false);

            obj.emplace();

            CHECK(obj.has_value() == true);

            CHECK(obj.value() == T());

            static_assert(!std::is_convertible<grabin::in_place_t, Optional>::value,
                          "Must be explicit!");
        }

        static void check_in_place_t_ctor_with_no_args()
        {
            using Optional = grabin::optional<T>;
            // @todo может ли быть constexpr?
            Optional const obj(grabin::in_place);

            CHECK(obj.has_value() == true);
            CHECK(static_cast<bool>(obj) == true);

            CHECK(obj.value() == T());

            static_assert(!std::is_convertible<grabin::in_place_t, Optional>::value,
                          "Must be explicit!");
        }
    };
}

#include <vector>
#include <string>
#include <complex>

namespace
{
    struct Empty
    {};

    bool operator==(Empty const &, Empty const &)
    {
        return true;
    }
}

TEST_CASE("optional : types and default ctor")
{
    optional_tester<bool>::check_types_and_empty_ctors();
    optional_tester<int>::check_types_and_empty_ctors();
    optional_tester<Empty>::check_types_and_empty_ctors();
    optional_tester<std::complex<double>>::check_types_and_empty_ctors();
    optional_tester<std::string>::check_types_and_empty_ctors();
    optional_tester<std::vector<std::vector<int>>>::check_types_and_empty_ctors();
    optional_tester<std::vector<std::vector<std::string>>>::check_types_and_empty_ctors();
}

TEST_CASE("optional : in_place_t ctor with no args")
{
    optional_tester<bool>::check_in_place_t_ctor_with_no_args();
    optional_tester<int>::check_in_place_t_ctor_with_no_args();
    optional_tester<Empty>::check_in_place_t_ctor_with_no_args();
    optional_tester<std::complex<double>>::check_in_place_t_ctor_with_no_args();
    optional_tester<std::string>::check_in_place_t_ctor_with_no_args();
    optional_tester<std::vector<std::vector<int>>>::check_in_place_t_ctor_with_no_args();
    optional_tester<std::vector<std::vector<std::string>>>::check_in_place_t_ctor_with_no_args();
}

TEST_CASE("optional : emplace with no args")
{
    optional_tester<bool>::check_emplace_with_no_args();
    optional_tester<int>::check_emplace_with_no_args();
    optional_tester<Empty>::check_emplace_with_no_args();
    optional_tester<std::complex<double>>::check_emplace_with_no_args();
    optional_tester<std::string>::check_emplace_with_no_args();
    optional_tester<std::vector<std::vector<int>>>::check_emplace_with_no_args();
    optional_tester<std::vector<std::vector<std::string>>>::check_emplace_with_no_args();
}

TEST_CASE("optional : in_place_t for int")
{
    using Value = int;

    auto property = [](Value const & value)
    {
        grabin::optional<Value> const obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(obj.value() == value);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : emplace for pair")
{
    using Value = std::pair<int, std::string>;

    auto property = [](Value const & value)
    {
        grabin::optional<Value> obj;

        CHECK(obj.has_value() == false);

        obj.emplace(value.first, value.second);

        CHECK(obj.has_value());
        CHECK(obj.value() == value);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : in_place_t for pair")
{
    using Value = std::pair<int, std::string>;

    auto property = [](Value const & value)
    {
        grabin::optional<Value> const obj(grabin::in_place, value.first, value.second);

        CHECK(obj.has_value());
        CHECK(obj.value() == value);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : reset on empty")
{
    grabin::optional<int> obj;

    obj.reset();

    static_assert(noexcept(obj.reset()), "Must be noexcept!");

    CHECK(obj.has_value() == false);
}

TEST_CASE("optional : reset on not empty")
{
    using Value = int;

    auto property = [](Value const & value)
    {
        grabin::optional<Value> obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(obj.value() == value);

        static_assert(noexcept(obj.reset()), "Must be noexcept!");

        obj.reset();

        CHECK(obj.has_value() == false);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : value non-const")
{
    using Value = int;

    auto property = [](Value const & old_value, Value const & new_value)
    {
        grabin::optional<Value> obj(grabin::in_place, old_value);

        CHECK(obj.has_value());
        CHECK(obj.value() == old_value);

        obj.value() = new_value;

        CHECK(obj.has_value());
        CHECK(obj.value() == new_value);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : copy_ctor without value")
{
    using Value = std::string;
    grabin::optional<Value> const obj{};
    auto const obj_copy(obj);

    CHECK(obj_copy.has_value() == false);
}

TEST_CASE("optional : copy_ctor with valued")
{
    using Value = std::string;

    auto property = [](Value const & value)
    {
        grabin::optional<Value> const obj(grabin::in_place, value);

        auto const obj_copy(obj);

        CHECK(obj_copy.has_value());
        CHECK(obj_copy.value() == value);
    };

    grabin_test::check(property);
}

static_assert(std::is_copy_constructible<grabin::optional<int>>::value, "");
static_assert(std::is_copy_constructible<grabin::optional<std::string>>::value, "");
static_assert(!std::is_copy_constructible<grabin::optional<std::unique_ptr<int>>>::value, "");

namespace
{
    template <class T, class Tag>
    struct wrapper
    {
    public:
        explicit wrapper(T init_value)
         : value(std::move(init_value))
        {}

        T value;
    };

    template <class T, class TagT, class U, class TagU>
    bool operator==(wrapper<T, TagT> const & x, wrapper<U, TagU> const & y)
    {
        return x.value == y.value;
    }

    template <class T, class TagT, class U, class TagU>
    bool operator!=(wrapper<T, TagT> const & x, wrapper<U, TagU> const & y)
    {
        return x.value != y.value;
    }

    template <class T, class TagT, class U, class TagU>
    bool operator<(wrapper<T, TagT> const & x, wrapper<U, TagU> const & y)
    {
        return x.value < y.value;
    }

    template <class T, class TagT, class U, class TagU>
    bool operator<=(wrapper<T, TagT> const & x, wrapper<U, TagU> const & y)
    {
        return x.value <= y.value;
    }

    template <class T, class TagT, class U, class TagU>
    bool operator>(wrapper<T, TagT> const & x, wrapper<U, TagU> const & y)
    {
        return x.value > y.value;
    }

    template <class T, class TagT, class U, class TagU>
    bool operator>=(wrapper<T, TagT> const & x, wrapper<U, TagU> const & y)
    {
        return x.value >= y.value;
    }

    template <class C, class CT, class T, class Tag>
    std::basic_ostream<C, CT> &
    operator<<(std::basic_ostream<C, CT> & os, wrapper<T, Tag> const & x)
    {
        return os << x.value;
    }
}

namespace grabin_test
{
    template <class T, class Tag>
    struct Arbitrary<wrapper<T, Tag>>
    {
        using value_type = wrapper<T, Tag>;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            return value_type(grabin_test::Arbitrary<T>::generate(rnd, generation));
        }
    };
}
// namespace grabin_test

TEST_CASE("optional : equality")
{
    using Value = std::string;
    using Optional1 = grabin::optional<wrapper<Value, std::true_type>>;
    using Optional2 = grabin::optional<wrapper<Value, std::false_type>>;

    auto property = [](Value const & value_1, Value const & value_2)
    {
        Optional1 const no_value_1{};
        Optional2 const no_value_2{};
        Optional1 const with_value_1(grabin::in_place, value_1);
        Optional2 const with_value_2(grabin::in_place, value_2);

        CHECK(no_value_1 == no_value_1);
        CHECK(no_value_1 == no_value_2);
        CHECK(!(no_value_1 != no_value_1));
        CHECK(!(no_value_1 != no_value_2));

        CHECK(with_value_1 == with_value_1);
        CHECK(!(with_value_1 != with_value_1));

        CHECK(with_value_2 == with_value_2);
        CHECK(!(with_value_2 != with_value_2));

        CHECK(!(no_value_1 == with_value_1));
        CHECK(!(no_value_2 == with_value_1));
        CHECK(no_value_1 != with_value_1);

        CHECK(!(no_value_1 == with_value_2));
        CHECK(!(no_value_2 == with_value_2));
        CHECK(no_value_1 != with_value_2);

        CHECK(!(with_value_1 == no_value_1));
        CHECK(!(with_value_1 == no_value_2));
        CHECK(no_value_1 != with_value_1);

        CHECK(!(no_value_1 == with_value_2));
        CHECK(!(no_value_2 == with_value_2));
        CHECK(no_value_1 != with_value_2);

        CHECK((with_value_1 == with_value_2) == (value_1 == value_2));
        CHECK((with_value_1 != with_value_2) == (value_1 != value_2));
    };

    grabin_test::check(property);
}

TEST_CASE("optional : assign nullopt")
{
    using Value = std::string;

    grabin::optional<Value> obj_0;
    obj_0 = grabin::nullopt;
    CHECK(!obj_0.has_value());

    static_assert(noexcept(obj_0 = grabin::nullopt), "Must be noexcept");

    auto property = [](Value const & value)
    {
        grabin::optional<Value> obj(grabin::in_place, value);

        CHECK(obj.has_value());

        obj = grabin::nullopt;

        CHECK(!obj.has_value());
    };

    grabin_test::check(property);
}

TEST_CASE("optional : equal to nullopt")
{
    using Value = std::string;

    grabin::optional<Value> const obj_0;

    CHECK(obj_0 == grabin::nullopt);
    CHECK(!(obj_0 != grabin::nullopt));

    CHECK(grabin::nullopt == obj_0);
    CHECK(!(grabin::nullopt != obj_0));

    static_assert(noexcept(obj_0 == grabin::nullopt), "Must be noexcept!");
    static_assert(noexcept(grabin::nullopt == obj_0), "Must be noexcept!");
    static_assert(noexcept(obj_0 != grabin::nullopt), "Must be noexcept!");
    static_assert(noexcept(grabin::nullopt != obj_0), "Must be noexcept!");

    auto property = [](Value const & value)
    {
        grabin::optional<Value> const obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(obj != grabin::nullopt);
        CHECK(!(obj == grabin::nullopt));

        CHECK(grabin::nullopt != obj);
        CHECK(!(grabin::nullopt == obj));
    };

    grabin_test::check(property);
}

TEST_CASE("optional : equality of T and optional<T>")
{
    using Value = std::string;
    using Optional = grabin::optional<wrapper<Value, std::true_type>>;

    auto property = [](Value const & value_1, Value const & value_2)
    {
        Optional const no_value{};
        Optional const with_value_1(grabin::in_place, value_1);
        wrapper<Value, std::false_type> const wrapped_1(value_1);
        wrapper<Value, std::false_type> const wrapped_2(value_2);

        CHECK(wrapped_2 != no_value);
        CHECK(!(wrapped_2 == no_value));

        CHECK(no_value != wrapped_2);
        CHECK(!(no_value == wrapped_2));

        CHECK(wrapped_1 == with_value_1);
        CHECK(with_value_1 == wrapped_1);

        CHECK(!(wrapped_1 != with_value_1));
        CHECK(!(with_value_1 != wrapped_1));

        CHECK((wrapped_2 == with_value_1) == (wrapped_2 == wrapped_1));
        CHECK((with_value_1 == wrapped_2) == (wrapped_1 == wrapped_2));

        CHECK((wrapped_2 != with_value_1) == !(wrapped_2 == wrapped_1));
        CHECK((with_value_1 != wrapped_2) == !(wrapped_1 == wrapped_2));
    };

    grabin_test::check(property);
}

TEST_CASE("optional : get_pointer const")
{
    using Value = std::string;

    auto property = [](Value const & value)
    {
        grabin::optional<Value> const obj0;
        grabin::optional<Value> const obj1(grabin::in_place, value);

        CHECK(obj0.get_pointer() == nullptr);

        REQUIRE(obj1.get_pointer() != nullptr);
        CHECK(*obj1.get_pointer() == value);

    };

    grabin_test::check(property);
}

TEST_CASE("optional : get_pointer non-const")
{
    using Value = std::string;

    auto property = [](Value const & value, Value const & new_value)
    {
        grabin::optional<Value> obj0;
        grabin::optional<Value> obj1(grabin::in_place, value);

        CHECK(obj0.get_pointer() == nullptr);

        REQUIRE(obj1.get_pointer() != nullptr);
        CHECK(*obj1.get_pointer() == value);

        *obj1.get_pointer() = new_value;
        CHECK(*obj1.get_pointer() == new_value);

    };

    grabin_test::check(property);
}

static_assert(std::is_nothrow_move_constructible<grabin::optional<int>>::value, "");

namespace
{
    class with_throwing_move_ctor
    {
    public:
        with_throwing_move_ctor(with_throwing_move_ctor && );

    private:

    };

    class not_move_constructible
    {
    public:
        not_move_constructible(not_move_constructible &&) = delete;
    };
}

static_assert(!std::is_nothrow_move_constructible<with_throwing_move_ctor>::value, "");
static_assert(!std::is_nothrow_move_constructible<grabin::optional<with_throwing_move_ctor>>::value, "");

static_assert(!std::is_move_constructible<not_move_constructible>::value, "");
static_assert(!std::is_move_constructible<grabin::optional<not_move_constructible>>::value, "");

TEST_CASE("optional : move ctor")
{
    using Value = int;
    using Optional = grabin::optional<std::unique_ptr<Value>>;

    auto property = [](Value const & value)
    {
        Optional src0;
        Optional src1(grabin::in_place, std::make_unique<Value>(value));

        Optional const dest0(std::move(src0));

        CHECK(dest0 == grabin::nullopt);

        Optional const dest1(std::move(src1));

        REQUIRE(src1.has_value());
        REQUIRE(dest1.has_value());
        REQUIRE(dest1.value() != nullptr);
        CHECK(*dest1.value() == value);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : less with nullopt_t")
{
    using Value = int;
    using Optional = grabin::optional<Value>;

    Optional const obj0;

    CHECK(!(obj0 < grabin::nullopt));
    CHECK(!(grabin::nullopt < obj0));

    static_assert(noexcept(obj0 < grabin::nullopt), "");
    static_assert(noexcept(grabin::nullopt < obj0), "");

    auto property = [](Value const & value)
    {
        Optional const obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(grabin::nullopt < obj);
        CHECK(!(obj < grabin::nullopt));
    };

    grabin_test::check(property);
}

TEST_CASE("optional : less or equal with nullopt_t")
{
    using Value = int;
    using Optional = grabin::optional<Value>;

    Optional const obj0;

    CHECK(obj0 <= grabin::nullopt);
    CHECK(grabin::nullopt <= obj0);

    static_assert(noexcept(obj0 <= grabin::nullopt), "");
    static_assert(noexcept(grabin::nullopt <= obj0), "");

    auto property = [](Value const & value)
    {
        Optional const obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(grabin::nullopt <= obj);
        CHECK(!(obj <= grabin::nullopt));
    };

    grabin_test::check(property);
}

TEST_CASE("optional : greater with nullopt_t")
{
    using Value = int;
    using Optional = grabin::optional<Value>;

    Optional const obj0;

    CHECK(!(obj0 > grabin::nullopt));
    CHECK(!(grabin::nullopt > obj0));

    static_assert(noexcept(obj0 > grabin::nullopt), "");
    static_assert(noexcept(grabin::nullopt > obj0), "");

    auto property = [](Value const & value)
    {
        Optional const obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(!(grabin::nullopt > obj));
        CHECK(obj > grabin::nullopt);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : greater or equal with nullopt_t")
{
    using Value = int;
    using Optional = grabin::optional<Value>;

    Optional const obj0;

    CHECK(obj0 >= grabin::nullopt);
    CHECK(grabin::nullopt >= obj0);

    static_assert(noexcept(obj0 <= grabin::nullopt), "");
    static_assert(noexcept(grabin::nullopt <= obj0), "");

    auto property = [](Value const & value)
    {
        Optional const obj(grabin::in_place, value);

        CHECK(obj.has_value());
        CHECK(!(grabin::nullopt >= obj));
        CHECK(obj >= grabin::nullopt);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : compare empty to value")
{
    using Value = wrapper<int, std::true_type>;
    using Optional = grabin::optional<wrapper<int, std::false_type>>;

    auto property = [](Value const & value)
    {
        Optional const obj{};

        CHECK(!obj.has_value());

        CHECK(obj < value);
        CHECK(!(value < obj));

        CHECK(obj <= value);
        CHECK(!(value <= obj));

        CHECK(!(obj > value));
        CHECK(value > obj);

        CHECK(!(obj >= value));
        CHECK(value >= obj);
    };

    grabin_test::check(property);
}

TEST_CASE("optional : compare non-empty to value")
{
    using Value = int;
    using Optional = grabin::optional<Value>;

    auto property = [](Value const & value, Value const & init_value)
    {
        Optional const obj(grabin::in_place, init_value);

        CHECK(obj.has_value());

        CHECK(!(obj < init_value));
        CHECK(!(init_value < obj));
        CHECK((obj < value) == (init_value < value));
        CHECK((value < obj) == (value < init_value));

        CHECK(obj <= init_value);
        CHECK(init_value <= obj);
        CHECK((obj <= value) == (init_value <= value));
        CHECK((value <= obj) == (value <= init_value));

        CHECK(!(obj > init_value));
        CHECK(!(init_value > obj));
        CHECK((obj > value) == (init_value > value));
        CHECK((value > obj) == (value > init_value));

        CHECK(obj >= init_value);
        CHECK(init_value >= obj);
        CHECK((obj >= value) == (init_value >= value));
        CHECK((value >= obj) == (value >= init_value));
    };

    grabin_test::check(property);
}

#include <set>

TEST_CASE("optional : in_place ctor with initializer_list")
{
    using Compare = bool(*)(int const &, int const & y);
    using Value = std::set<int, Compare>;
    using Optional = grabin::optional<Value>;

    static_assert(std::is_constructible<Value, std::initializer_list<int>&, Compare&&>::value, "");
    static_assert(std::is_constructible<Optional, grabin::in_place_t, std::initializer_list<int>&, Compare&&>::value, "");
    static_assert(!std::is_constructible<Value, std::initializer_list<int>&, int&&>::value, "");
    static_assert(!std::is_constructible<Optional, grabin::in_place_t, std::initializer_list<int>&, int&&>::value, "");

    auto const cmp = Compare([](int const & x, int const & y){ return x > y; });

    Value const expected({1, 2, 3, 4, 5}, cmp);
    Optional const actual(grabin::in_place, {1, 2, 3, 4, 5}, cmp);

    REQUIRE(actual.has_value());
    REQUIRE(actual.value() == expected);
}
