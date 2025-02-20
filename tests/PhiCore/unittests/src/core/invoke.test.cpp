#include <phi/test/test_macros.hpp>

#include <phi/compiler_support/compiler.hpp>
#include <phi/compiler_support/extended_attributes.hpp>
#include <phi/compiler_support/warning.hpp>
#include <phi/core/declval.hpp>
#include <phi/core/forward.hpp>
#include <phi/core/move.hpp>
#include <functional>
#include <type_traits>

PHI_GCC_SUPPRESS_WARNING_PUSH()
PHI_GCC_SUPPRESS_WARNING("-Wsign-conversion")
PHI_GCC_SUPPRESS_WARNING("-Wnoexcept")
PHI_GCC_SUPPRESS_WARNING("-Wuseless-cast")

#include <phi/core/invoke.hpp>

// TODO: Make tests work with MSVC
#if PHI_COMPILER_IS_NOT(MSVC) && PHI_HAS_WORKING_IS_INVOCABLE()

struct NonCopyable
{
    NonCopyable()
    {}

private:
    NonCopyable(NonCopyable const&) = delete;
    NonCopyable& operator=(NonCopyable const&) = delete;
};

struct TestClass
{
    explicit TestClass(int x)
        : data(x)
    {}

    int& operator()(NonCopyable&&) &
    {
        return data;
    }
    int const& operator()(NonCopyable&&) const&
    {
        return data;
    }
    int volatile& operator()(NonCopyable&&) volatile&
    {
        return data;
    }
    int const volatile& operator()(NonCopyable&&) const volatile&
    {
        return data;
    }

    int&& operator()(NonCopyable&&) &&
    {
        return phi::move(data);
    }
    int const&& operator()(NonCopyable&&) const&&
    {
        return phi::move(data);
    }
    int volatile&& operator()(NonCopyable&&) volatile&&
    {
        return phi::move(data);
    }
    int const volatile&& operator()(NonCopyable&&) const volatile&&
    {
        return phi::move(data);
    }

    int data;

private:
    TestClass(TestClass const&) = delete;
    TestClass& operator=(TestClass const&) = delete;
};

struct DerivedFromTestClass : public TestClass
{
    explicit DerivedFromTestClass(int x)
        : TestClass(x)
    {}
};

PHI_ATTRIBUTE_CONST int& foo(NonCopyable&&)
{
    static int data = 42;
    return data;
}

template <typename SignatureT, typename ExpectT, typename FunctorT>
void test_b12(FunctorT&& f)
{
    // Create the callable object.
    using ClassFunc    = SignatureT TestClass::*;
    ClassFunc func_ptr = &TestClass::operator();

    // Create the dummy arg.
    NonCopyable arg;

    // Check that the deduced return type of invoke is what is expected.
    using DeducedReturnType =
            decltype(phi::invoke(func_ptr, phi::forward<FunctorT>(f), phi::move(arg)));
    CHECK_SAME_TYPE(DeducedReturnType, ExpectT);

#    if PHI_COMPILER_IS(EMCC) || PHI_COMPILER_IS(WINCLANG)
    SKIP_CHECK(); // Emcc stdlib and winclang doesn't seem to have std::result_of
#    else
    // Check that result_of_t matches Expect.
    using ResultOfReturnType =
            typename std::result_of<ClassFunc && (FunctorT&&, NonCopyable &&)>::type;
    CHECK_SAME_TYPE(ResultOfReturnType, ExpectT);
#    endif

    // Run invoke and check the return value.
    DeducedReturnType ret = phi::invoke(func_ptr, phi::forward<FunctorT>(f), phi::move(arg));
    CHECK(ret == 42);
}

template <typename ExpectT, typename FunctorT>
void test_b34(FunctorT&& f)
{
    // Create the callable object.
    using ClassFunc                = int TestClass::*;
    ClassFunc             func_ptr = &TestClass::data;

    // Check that the deduced return type of invoke is what is expected.
    using DeducedReturnType = decltype(phi::invoke(func_ptr, phi::forward<FunctorT>(f)));
    CHECK_SAME_TYPE(DeducedReturnType, ExpectT);

    // Check that result_of_t matches Expect.
#    if PHI_COMPILER_IS(EMCC) || PHI_COMPILER_IS(WINCLANG)
    SKIP_CHECK(); // Emcc stdlib and winclang doesn't seem to have std::result_of
#    else
    using ResultOfReturnType = typename std::result_of<ClassFunc && (FunctorT &&)>::type;
    CHECK_SAME_TYPE(ResultOfReturnType, ExpectT);
#    endif

    // Run invoke and check the return value.
    DeducedReturnType ret = phi::invoke(func_ptr, phi::forward<FunctorT>(f));
    CHECK(ret == 42);
}

template <typename ExpectT, typename FunctorT>
void test_b5(FunctorT&& f)
{
    NonCopyable arg;

    // Check that the deduced return type of invoke is what is expected.
    using DeducedReturnType = decltype(phi::invoke(phi::forward<FunctorT>(f), phi::move(arg)));
    STATIC_REQUIRE((phi::is_same<DeducedReturnType, ExpectT>::value));

    // Check that result_of_t matches Expect.
#    if PHI_COMPILER_IS(EMCC) || PHI_COMPILER_IS(WINCLANG)
    SKIP_CHECK(); // Emcc stdlib and winclang doesn't seem to have std::result_of
#    else
    using ResultOfReturnType = typename std::result_of<FunctorT && (NonCopyable &&)>::type;
    STATIC_REQUIRE((phi::is_same<ResultOfReturnType, ExpectT>::value));
#    endif

    // Run invoke and check the return value.
    DeducedReturnType ret = phi::invoke(phi::forward<FunctorT>(f), phi::move(arg));
    CHECK(ret == 42);
}

TEST_CASE("invoke bullet one and two")
{
    {
        TestClass cl(42);
        test_b12<int&(NonCopyable &&)&, int&>(cl);
        test_b12<int const&(NonCopyable &&) const&, int const&>(cl);
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(cl);
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(cl);

        test_b12<int && (NonCopyable &&)&&, int&&>(phi::move(cl));
        test_b12<int const && (NonCopyable &&) const&&, int const&&>(phi::move(cl));
        test_b12<int volatile && (NonCopyable &&) volatile&&, int volatile&&>(phi::move(cl));
        test_b12<int const volatile && (NonCopyable &&) const volatile&&, int const volatile&&>(
                phi::move(cl));
    }
    {
        DerivedFromTestClass cl(42);
        test_b12<int&(NonCopyable &&)&, int&>(cl);
        test_b12<int const&(NonCopyable &&) const&, int const&>(cl);
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(cl);
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(cl);

        test_b12<int && (NonCopyable &&)&&, int&&>(phi::move(cl));
        test_b12<int const && (NonCopyable &&) const&&, int const&&>(phi::move(cl));
        test_b12<int volatile && (NonCopyable &&) volatile&&, int volatile&&>(phi::move(cl));
        test_b12<int const volatile && (NonCopyable &&) const volatile&&, int const volatile&&>(
                phi::move(cl));
    }
    {
        TestClass                         cl_obj(42);
        std::reference_wrapper<TestClass> cl(cl_obj);
        test_b12<int&(NonCopyable &&)&, int&>(cl);
        test_b12<int const&(NonCopyable &&) const&, int const&>(cl);
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(cl);
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(cl);

        test_b12<int&(NonCopyable &&)&, int&>(phi::move(cl));
        test_b12<int const&(NonCopyable &&) const&, int const&>(phi::move(cl));
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(phi::move(cl));
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(
                phi::move(cl));
    }
    {
        DerivedFromTestClass                         cl_obj(42);
        std::reference_wrapper<DerivedFromTestClass> cl(cl_obj);
        test_b12<int&(NonCopyable &&)&, int&>(cl);
        test_b12<int const&(NonCopyable &&) const&, int const&>(cl);
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(cl);
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(cl);

        test_b12<int&(NonCopyable &&)&, int&>(phi::move(cl));
        test_b12<int const&(NonCopyable &&) const&, int const&>(phi::move(cl));
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(phi::move(cl));
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(
                phi::move(cl));
    }
    {
        TestClass  cl_obj(42);
        TestClass* cl = &cl_obj;
        test_b12<int&(NonCopyable &&)&, int&>(cl);
        test_b12<int const&(NonCopyable &&) const&, int const&>(cl);
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(cl);
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(cl);
    }
    {
        DerivedFromTestClass  cl_obj(42);
        DerivedFromTestClass* cl = &cl_obj;
        test_b12<int&(NonCopyable &&)&, int&>(cl);
        test_b12<int const&(NonCopyable &&) const&, int const&>(cl);
        test_b12<int volatile&(NonCopyable &&) volatile&, int volatile&>(cl);
        test_b12<int const volatile&(NonCopyable &&) const volatile&, int const volatile&>(cl);
    }
}

TEST_CASE("invoke bullet tree and four")
{
    {
        using Fn = TestClass;
        Fn cl(42);
        test_b34<int&>(cl);
        test_b34<int const&>(static_cast<Fn const&>(cl));
        test_b34<int volatile&>(static_cast<Fn volatile&>(cl));
        test_b34<int const volatile&>(static_cast<Fn const volatile&>(cl));

        test_b34<int&&>(static_cast<Fn&&>(cl));
        test_b34<int const&&>(static_cast<Fn const&&>(cl));
        test_b34<int volatile&&>(static_cast<Fn volatile&&>(cl));
        test_b34<int const volatile&&>(static_cast<Fn const volatile&&>(cl));
    }
    {
        using Fn = DerivedFromTestClass;
        Fn cl(42);
        test_b34<int&>(cl);
        test_b34<int const&>(static_cast<Fn const&>(cl));
        test_b34<int volatile&>(static_cast<Fn volatile&>(cl));
        test_b34<int const volatile&>(static_cast<Fn const volatile&>(cl));

        test_b34<int&&>(static_cast<Fn&&>(cl));
        test_b34<int const&&>(static_cast<Fn const&&>(cl));
        test_b34<int volatile&&>(static_cast<Fn volatile&&>(cl));
        test_b34<int const volatile&&>(static_cast<Fn const volatile&&>(cl));
    }
    {
        using Fn = TestClass;
        Fn cl(42);
        test_b34<int&>(std::reference_wrapper<Fn>(cl));
        test_b34<int const&>(std::reference_wrapper<Fn const>(cl));
        test_b34<int volatile&>(std::reference_wrapper<Fn volatile>(cl));
        test_b34<int const volatile&>(std::reference_wrapper<Fn const volatile>(cl));
    }
    {
        using Fn = DerivedFromTestClass;
        Fn cl(42);
        test_b34<int&>(std::reference_wrapper<Fn>(cl));
        test_b34<int const&>(std::reference_wrapper<Fn const>(cl));
        test_b34<int volatile&>(std::reference_wrapper<Fn volatile>(cl));
        test_b34<int const volatile&>(std::reference_wrapper<Fn const volatile>(cl));
    }
    {
        using Fn = TestClass;
        Fn  cl_obj(42);
        Fn* cl = &cl_obj;
        test_b34<int&>(cl);
        test_b34<int const&>(static_cast<Fn const*>(cl));
        test_b34<int volatile&>(static_cast<Fn volatile*>(cl));
        test_b34<int const volatile&>(static_cast<Fn const volatile*>(cl));
    }
    {
        using Fn = DerivedFromTestClass;
        Fn  cl_obj(42);
        Fn* cl = &cl_obj;
        test_b34<int&>(cl);
        test_b34<int const&>(static_cast<Fn const*>(cl));
        test_b34<int volatile&>(static_cast<Fn volatile*>(cl));
        test_b34<int const volatile&>(static_cast<Fn const volatile*>(cl));
    }
}

TEST_CASE("invoke bullet five")
{
    using FooType = int&(NonCopyable &&);
    {
        FooType& fn = foo;
        test_b5<int&>(fn);
    }
    {
        FooType* fn = foo;
        test_b5<int&>(fn);
    }
    {
        using Fn = TestClass;
        Fn cl(42);
        test_b5<int&>(cl);
        test_b5<int const&>(static_cast<Fn const&>(cl));
        test_b5<int volatile&>(static_cast<Fn volatile&>(cl));
        test_b5<int const volatile&>(static_cast<Fn const volatile&>(cl));

        test_b5<int&&>(static_cast<Fn&&>(cl));
        test_b5<int const&&>(static_cast<Fn const&&>(cl));
        test_b5<int volatile&&>(static_cast<Fn volatile&&>(cl));
        test_b5<int const volatile&&>(static_cast<Fn const volatile&&>(cl));
    }
}

struct CopyThrows
{
    CopyThrows()
    {}
    CopyThrows(CopyThrows const&)
    {}
    CopyThrows(CopyThrows&&) noexcept
    {}
};

struct NoThrowCallable
{
    void operator()() noexcept
    {}
    void operator()(CopyThrows) noexcept
    {}
};

struct ThrowsCallable
{
    void operator()()
    {}
};

struct MemberObj
{
    int x;
};

TEST_CASE("invoke noexcept")
{
    {
        NoThrowCallable obj;
        ((void)obj); // suppress unused warning
        CopyThrows arg;
        ((void)arg); // suppress unused warning
        STATIC_REQUIRE(noexcept(phi::invoke(obj)));
        STATIC_REQUIRE(!noexcept(phi::invoke(obj, arg)));
        STATIC_REQUIRE(noexcept(phi::invoke(obj, phi::move(arg))));
    }
    {
        ThrowsCallable obj;
        ((void)obj); // suppress unused warning
        STATIC_REQUIRE(!noexcept(phi::invoke(obj)));
    }
    {
        MemberObj obj{42};
        ((void)obj); // suppress unused warning.
        STATIC_REQUIRE(noexcept(phi::invoke(&MemberObj::x, obj)));
    }
}

template <typename T, int N>
struct Array
{
    typedef T type[N];
};

struct Type
{
    Array<char, 1>::type& f1();
    Array<char, 2>::type& f2() const;
    Array<char, 1>::type& g1() &;
    Array<char, 2>::type& g2() const&;
    Array<char, 3>::type& g3() &&;
    Array<char, 4>::type& g4() const&&;
};

TEST_CASE("invoke")
{
    STATIC_REQUIRE(sizeof(phi::invoke(&Type::f1, phi::declval<Type>())) == 1);
    STATIC_REQUIRE(sizeof(phi::invoke(&Type::f2, phi::declval<Type const>())) == 2);
    STATIC_REQUIRE(sizeof(phi::invoke(&Type::g1, phi::declval<Type&>())) == 1);
    STATIC_REQUIRE(sizeof(phi::invoke(&Type::g2, phi::declval<Type const&>())) == 2);
    STATIC_REQUIRE(sizeof(phi::invoke(&Type::g3, phi::declval<Type&&>())) == 3);
    STATIC_REQUIRE(sizeof(phi::invoke(&Type::g4, phi::declval<Type const&&>())) == 4);
}

PHI_ATTRIBUTE_CONST int foo(int)
{
    return 42;
}

TEST_CASE("invoke basic test")
{
    //REQUIRE(phi::invoke(foo, 101) == 42);
}

#endif

PHI_GCC_SUPPRESS_WARNING_POP()
