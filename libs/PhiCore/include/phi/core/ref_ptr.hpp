#ifndef INCG_PHI_CORE_REF_PTR_HPP
#define INCG_PHI_CORE_REF_PTR_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/algorithm/exchange.hpp"
#include "phi/algorithm/swap.hpp"
#include "phi/compiler_support/constexpr.hpp"
#include "phi/compiler_support/nodiscard.hpp"
#include "phi/core/assert.hpp"
#include "phi/core/forward.hpp"
#include "phi/core/nullptr_t.hpp"
#include "phi/core/types.hpp"
#include "phi/type_traits/enable_if.hpp"
#include "phi/type_traits/is_array.hpp"
#include "phi/type_traits/is_convertible.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

namespace detail
{
    class ref_ptr_control_block
    {
    public:
        PHI_NODISCARD boolean should_delete() const noexcept
        {
            return m_NumberOfRefPtrs == 0u;
        }

        PHI_NODISCARD phi::u32 get_ref_use_count() const noexcept
        {
            return m_NumberOfRefPtrs;
        }

        void increment_ref_count() noexcept
        {
            m_NumberOfRefPtrs += 1u;
        }

        void decrement_ref_count() noexcept
        {
            PHI_DBG_ASSERT(m_NumberOfRefPtrs != 0u, "Double delete");

            m_NumberOfRefPtrs -= 1u;
        }

    private:
        u32 m_NumberOfRefPtrs{1u};
    };
} // namespace detail

// forward declaration
template <typename TypeT>
class not_null_ref_ptr;

// Reference counted non-atomic smart pointer
template <typename TypeT>
class ref_ptr
{
public:
    using this_type       = ref_ptr<TypeT>;
    using not_null_type   = not_null_ref_ptr<TypeT>;
    using value_type      = TypeT;
    using reference       = TypeT&;
    using const_reference = const TypeT&;
    using pointer         = TypeT*;
    using const_pointer   = const TypeT*;

    constexpr ref_ptr() noexcept
        : m_Ptr{nullptr}
        , m_ControlBlock{nullptr}
    {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(nullptr_t) noexcept
        : m_Ptr{nullptr}
        , m_ControlBlock{nullptr}
    {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(TypeT* ptr)
        : m_Ptr{ptr}
        , m_ControlBlock{allocate_control_block()}
    {}

    constexpr ref_ptr(const ref_ptr& other) noexcept
        : m_Ptr{other.m_Ptr}
        , m_ControlBlock{other.m_ControlBlock}
    {
        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->increment_ref_count();
        }
    }

    constexpr ref_ptr(ref_ptr&& other) noexcept
        : m_Ptr{exchange(other.m_Ptr, nullptr)}
        , m_ControlBlock{exchange(other.m_ControlBlock, nullptr)}
    {}

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(const ref_ptr<OtherT>& other) noexcept
        : m_Ptr{other.m_Ptr}
        , m_ControlBlock{other.m_ControlBlock}
    {
        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->increment_ref_count();
        }
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(ref_ptr<OtherT>&& other) noexcept
        : m_Ptr{exchange(other.m_Ptr, nullptr)}
        , m_ControlBlock{exchange(other.m_ControlBlock, nullptr)}
    {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(const not_null_ref_ptr<TypeT>& other) noexcept
        : m_Ptr{other.m_Ptr}
        , m_ControlBlock{other.m_ControlBlock}
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "not_null_ref_ptr was null");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "not_null_ref_ptr was null");

        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->increment_ref_count();
        }
    }

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(not_null_ref_ptr<TypeT>&& other) noexcept
        : m_Ptr{exchange(other.m_Ptr, nullptr)}
        , m_ControlBlock{exchange(other.m_ControlBlock, nullptr)}
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "not_null_ref_ptr was null");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "not_null_ref_ptr was null");
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(const not_null_ref_ptr<OtherT>& other) noexcept
        : m_Ptr{other.m_Ptr}
        , m_ControlBlock{other.m_ControlBlock}
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "not_null_ref_ptr was null");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "not_null_ref_ptr was null");

        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->increment_ref_count();
        }
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr ref_ptr(not_null_ref_ptr<OtherT>&& other) noexcept
        : m_Ptr{exchange(other.m_Ptr, nullptr)}
        , m_ControlBlock{exchange(other.m_ControlBlock, nullptr)}
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "not_null_ref_ptr was null");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "ControlBlock was null");
    }

    PHI_CONSTEXPR_DESTRUCTOR ~ref_ptr() noexcept
    {
        clear();
    }

    constexpr ref_ptr& operator=(nullptr_t) noexcept
    {
        clear();

        return *this;
    }

    constexpr ref_ptr& operator=(TypeT* ptr)
    {
        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->decrement_ref_count();
            if (m_ControlBlock->should_delete())
            {
                delete m_Ptr;
                delete m_ControlBlock;
            }
        }

        m_Ptr = ptr;
        allocate_control_block();

        return *this;
    }

    constexpr ref_ptr& operator=(const ref_ptr& other) noexcept
    {
        ref_ptr<TypeT>(other).swap(*this);

        return *this;
    }

    constexpr ref_ptr& operator=(ref_ptr&& other) noexcept
    {
        ref_ptr<TypeT>(move(other)).swap(*this);

        return *this;
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    constexpr ref_ptr& operator=(const ref_ptr<OtherT>& other) noexcept
    {
        ref_ptr<TypeT>(other).swap(*this);

        return *this;
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    constexpr ref_ptr& operator=(ref_ptr<OtherT>&& other) noexcept
    {
        ref_ptr<TypeT>(move(other)).swap(*this);

        return *this;
    }

    constexpr ref_ptr& operator=(const not_null_ref_ptr<TypeT>& other) noexcept
    {
        ref_ptr<TypeT>(other).swap(*this);

        return *this;
    }

    constexpr ref_ptr& operator=(not_null_ref_ptr<TypeT>&& other) noexcept
    {
        ref_ptr<TypeT>(move(other)).swap(*this);

        return *this;
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    constexpr ref_ptr& operator=(const not_null_ref_ptr<OtherT>& other) noexcept
    {
        ref_ptr<TypeT>(other).swap(*this);

        return *this;
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::type> = 0>
    constexpr ref_ptr& operator=(not_null_ref_ptr<OtherT>&& other) noexcept
    {
        ref_ptr<TypeT>(move(other)).swap(*this);

        return *this;
    }

    constexpr void clear() noexcept
    {
        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->decrement_ref_count();
            if (m_ControlBlock->should_delete())
            {
                delete m_Ptr;
                delete m_ControlBlock;

                m_Ptr          = nullptr;
                m_ControlBlock = nullptr;
            }
        }
    }

    constexpr void reset(TypeT* ptr) noexcept
    {
        if (ptr == m_Ptr)
        {
            return;
        }

        decrement_count_and_delete_if_required();

        m_Ptr = ptr;
        if (ptr)
        {
            allocate_control_block();
        }
        else
        {
            m_ControlBlock = nullptr;
        }
    }

    constexpr void swap(ref_ptr& other) noexcept
    {
        phi::swap(m_Ptr, other.m_Ptr);
        phi::swap(m_ControlBlock, other.m_ControlBlock);
    }

    [[nodiscard]] constexpr TypeT* get() noexcept
    {
        return m_Ptr;
    }

    [[nodiscard]] constexpr const TypeT* get() const noexcept
    {
        return m_Ptr;
    }

    [[nodiscard]] constexpr usize use_count() const noexcept
    {
        if (m_ControlBlock != nullptr)
        {
            return m_ControlBlock->get_ref_use_count();
        }

        return 0u;
    }

    constexpr explicit operator bool() const noexcept
    {
        return get() != nullptr;
    }

    constexpr explicit operator boolean() const noexcept
    {
        return get() != nullptr;
    }

    constexpr explicit operator const TypeT*() const noexcept
    {
        return get();
    }

    constexpr explicit operator TypeT*() noexcept
    {
        return get();
    }

    TypeT* operator->()
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Cannot dereference a nullptr");

        return get();
    }

    const TypeT* operator->() const
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Cannot dereference a nullptr");

        return get();
    }

    TypeT& operator*()
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Cannot dereference a nullptr");

        return *get();
    }

    const TypeT& operator*() const
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Cannot dereference a nullptr");

        return *get();
    }

private:
    detail::ref_ptr_control_block* allocate_control_block()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        m_ControlBlock = new (std::nothrow) detail::ref_ptr_control_block();

        if (m_ControlBlock == nullptr)
        {
            delete m_Ptr;
            throw std::bad_alloc();
        }

        return m_ControlBlock;
    }

    void decrement_count_and_delete_if_required()
    {
        if (m_ControlBlock != nullptr)
        {
            m_ControlBlock->decrement_ref_count();
            if (m_ControlBlock->should_delete())
            {
                delete m_Ptr;
                delete m_ControlBlock;
            }
        }
    }

    TypeT*                         m_Ptr;
    detail::ref_ptr_control_block* m_ControlBlock;
};

template <typename LhsT, typename RhsT>
constexpr boolean operator==(const ref_ptr<LhsT>& lhs, const ref_ptr<RhsT>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

template <typename LhsT>
constexpr boolean operator==(const ref_ptr<LhsT>& lhs, nullptr_t) noexcept
{
    return lhs.get() == nullptr;
}

template <typename RhsT>
constexpr boolean operator==(nullptr_t, const ref_ptr<RhsT>& rhs) noexcept
{
    return rhs.get() == nullptr;
}

template <typename LhsT, typename RhsT>
constexpr boolean operator!=(const ref_ptr<LhsT>& lhs, const ref_ptr<RhsT>& rhs) noexcept
{
    return lhs.get() != rhs.get();
}

template <typename LhsT>
constexpr boolean operator!=(const ref_ptr<LhsT>& lhs, nullptr_t) noexcept
{
    return lhs.get() != nullptr;
}

template <typename RhsT>
constexpr boolean operator!=(nullptr_t, const ref_ptr<RhsT>& rhs) noexcept
{
    return rhs.get() != nullptr;
}

template <typename TypeT>
constexpr void swap(ref_ptr<TypeT>& lhs, ref_ptr<TypeT>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename TypeT>
class not_null_ref_ptr
{
public:
    using this_type       = not_null_ref_ptr<TypeT>;
    using value_type      = TypeT;
    using reference       = TypeT&;
    using const_reference = const TypeT&;
    using pointer         = TypeT*;
    using const_pointer   = const TypeT*;

    not_null_ref_ptr() = delete;

    not_null_ref_ptr(nullptr_t) = delete;

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr not_null_ref_ptr(TypeT* ptr) noexcept
        : m_Ptr{ptr}
        , m_ControlBlock{allocate_control_block()}
    {
        PHI_DBG_ASSERT(ptr != nullptr, "Trying to assign nullptr to not_null_ref_ptr");
    }

    constexpr not_null_ref_ptr(const not_null_ref_ptr& other) noexcept
        : m_Ptr{other.m_Ptr}
        , m_ControlBlock{other.m_ControlBlock}
    {
        PHI_DBG_ASSERT(other.get() != nullptr, "Trying to assign nullptr to not_null_ref_ptr");

        m_ControlBlock->increment_ref_count();
    }

    constexpr not_null_ref_ptr(not_null_ref_ptr&& other) noexcept
        : m_Ptr{exchange(other.m_Ptr, nullptr)}
        , m_ControlBlock{exchange(other.m_ControlBlock, nullptr)}
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Trying to assign nullptr to not_null_ref_ptr");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "ControllBlock was null");
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr not_null_ref_ptr(const not_null_ref_ptr<OtherT>& other) noexcept
        : m_Ptr{other.m_Ptr}
        , m_ControlBlock{other.m_ControlBlock}
    {
        PHI_DBG_ASSERT(other.get() != nullptr, "Trying to assign nullptr to not_null_ref_ptr");
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr not_null_ref_ptr(not_null_ref_ptr<OtherT>&& other) noexcept
        : m_Ptr{exchange(other.m_Ptr, nullptr)}
        , m_ControlBlock{exchange(other.m_ControlBlock, nullptr)}
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Trying to assign nullptr to not_null_ref_ptr");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "ControlBlock was null");
    }

    PHI_CONSTEXPR_DESTRUCTOR ~not_null_ref_ptr() noexcept
    {
        clear();
    }

    not_null_ref_ptr& operator=(nullptr_t) = delete;

    constexpr not_null_ref_ptr& operator=(TypeT* ptr)
    {
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "ControlBlock was null!");

        decrement_count_and_delete_if_required();

        m_Ptr = ptr;
        allocate_control_block();

        return *this;
    }

    constexpr not_null_ref_ptr& operator=(const not_null_ref_ptr& other) noexcept
    {
        not_null_ref_ptr<TypeT>(other).swap(*this);

        PHI_DBG_ASSERT(get() != nullptr, "Trying to assign nullptr to not_null_ref_ptr");

        return *this;
    }

    constexpr not_null_ref_ptr& operator=(not_null_ref_ptr&& other) noexcept
    {
        not_null_ref_ptr<TypeT>(move(other)).swap(*this);

        PHI_DBG_ASSERT(get() != nullptr, "Trying to assign nullptr to not_null_ref_ptr");

        return *this;
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    constexpr not_null_ref_ptr& operator=(const not_null_ref_ptr<OtherT>& other) noexcept
    {
        not_null_ref_ptr<TypeT>(other).swap(*this);

        PHI_DBG_ASSERT(get() != nullptr, "Trying to assign nullptr to not_null_ref_ptr");

        return *this;
    }

    template <typename OtherT, enable_if_t<is_convertible<OtherT*, TypeT*>::value> = 0>
    constexpr not_null_ref_ptr& operator=(not_null_ref_ptr<OtherT>&& other) noexcept
    {
        not_null_ref_ptr<TypeT>(move(other)).swap(*this);

        PHI_DBG_ASSERT(get() != nullptr, "Trying to assign nullptr to not_null_ref_ptr");

        return *this;
    }

    void reset(nullptr_t) = delete;

    constexpr void reset(TypeT* ptr) noexcept
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Trying to assign nullptr to not_null_ref_ptr");

        if (ptr == m_Ptr)
        {
            return;
        }

        decrement_count_and_delete_if_required();

        m_Ptr = ptr;
        allocate_control_block();
    }

    constexpr void swap(not_null_ref_ptr& other) noexcept
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Trying to assign nullptr to not_null_ref_ptr");
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "ControlBlock was null");
        PHI_DBG_ASSERT(other.m_Ptr != nullptr, "Trying to assign nullptr to not_null_ref_ptr");
        PHI_DBG_ASSERT(other.m_ControlBlock != nullptr, "ControlBlock was null");

        phi::swap(m_Ptr, other.m_Ptr);
        phi::swap(m_ControlBlock, other.m_ControlBlock);
    }

    [[nodiscard]] constexpr TypeT* get() noexcept
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Using not_null_ref_ptr after it was moved from");

        return m_Ptr;
    }

    [[nodiscard]] constexpr const TypeT* get() const noexcept
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Using not_null_ref_ptr after it was moved from");

        return m_Ptr;
    }

    [[nodiscard]] constexpr usize use_count() const noexcept
    {
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "ControlBlock was null");

        return m_ControlBlock->get_ref_use_count();
    }

    operator bool() = delete;

    operator boolean() = delete;

    constexpr explicit operator const TypeT*() const noexcept
    {
        return get();
    }

    constexpr explicit operator TypeT*() noexcept
    {
        return get();
    }

    TypeT* operator->()
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Using not_null_ref_ptr after it was moved from");

        return get();
    }

    const TypeT* operator->() const
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Using not_null_ref_ptr after it was moved from");

        return get();
    }

    TypeT& operator*()
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Using not_null_ref_ptr after it was moved from");

        return *get();
    }

    const TypeT& operator*() const
    {
        PHI_DBG_ASSERT(m_Ptr != nullptr, "Using not_null_ref_ptr after it was moved from");

        return *get();
    }

private:
    // Need to declare ref_ptr as a friend to access the control block
    friend ref_ptr<TypeT>;

    constexpr void clear() noexcept
    {
        // As clear is called from the destructor the control block can actually be null here if the ref_ptr was moved from.
        if (m_ControlBlock != nullptr)
        {
            decrement_count_and_delete_if_required();
        }

        // In debug builds explicitly set all values to nullptr
#if defined(PHI_DEBUG)
        m_ControlBlock = nullptr;
        m_Ptr          = nullptr;
#endif
    }

    detail::ref_ptr_control_block* allocate_control_block() noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        m_ControlBlock = new (std::nothrow) detail::ref_ptr_control_block();

        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "Failed to allocate control block");
        return m_ControlBlock;
    }

    void decrement_count_and_delete_if_required() noexcept
    {
        PHI_DBG_ASSERT(m_ControlBlock != nullptr, "Unexpected nullptr for control block");

        m_ControlBlock->decrement_ref_count();
        if (m_ControlBlock->should_delete())
        {
            delete m_Ptr;
            delete m_ControlBlock;
        }
    }

    TypeT*                         m_Ptr;
    detail::ref_ptr_control_block* m_ControlBlock;
};

template <typename LhsT, typename RhsT>
constexpr boolean operator==(const not_null_ref_ptr<LhsT>& lhs,
                             const not_null_ref_ptr<RhsT>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

template <typename LhsT>
boolean operator==(const not_null_ref_ptr<LhsT>& lhs, nullptr_t) = delete;

template <typename RhsT>
boolean operator==(nullptr_t, const not_null_ref_ptr<RhsT>& rhs) = delete;

template <typename LhsT, typename RhsT>
constexpr boolean operator!=(const not_null_ref_ptr<LhsT>& lhs,
                             const not_null_ref_ptr<RhsT>& rhs) noexcept
{
    return lhs.get() != rhs.get();
}

template <typename LhsT>
boolean operator!=(const not_null_ref_ptr<LhsT>& lhs, nullptr_t) = delete;

template <typename RhsT>
boolean operator!=(nullptr_t, const not_null_ref_ptr<RhsT>& rhs) = delete;

template <typename TypeT>
constexpr void swap(not_null_ref_ptr<TypeT>& lhs, not_null_ref_ptr<TypeT>& rhs) noexcept
{
    lhs.swap(rhs);
}

// make functions

template <typename TypeT, typename... ArgsT>
[[nodiscard]] enable_if_t<is_not_array<TypeT>::value, ref_ptr<TypeT>> make_ref(ArgsT&&... args)
{
    return ref_ptr<TypeT>(new TypeT(forward<ArgsT>(args)...));
}

template <typename TypeT, typename... ArgsT>
[[nodiscard]] enable_if_t<is_not_array<TypeT>::value, not_null_ref_ptr<TypeT>> make_not_null_ref(
        ArgsT&&... args)
{
    return not_null_ref_ptr<TypeT>(new TypeT(forward<ArgsT>(args)...));
}

DETAIL_PHI_END_NAMESPACE()

DETAIL_PHI_BEGIN_STD_NAMESPACE()

template <typename TypeT>
struct hash<phi::ref_ptr<TypeT>>
{
    PHI_NODISCARD constexpr size_t operator()(phi::ref_ptr<TypeT> pointer) const noexcept
    {
        return std::hash<TypeT*>()(pointer.get());
    }
};

template <typename TypeT>
struct hash<phi::not_null_ref_ptr<TypeT>>
{
    PHI_NODISCARD constexpr size_t operator()(phi::not_null_ref_ptr<TypeT> pointer) const noexcept
    {
        return std::hash<TypeT*>()(pointer.get());
    }
};

template <>
struct hash<phi::not_null_ref_ptr<phi::nullptr_t>>
{
    phi::size_t operator()(phi::not_null_ref_ptr<phi::nullptr_t> ptr) = delete;
};

DETAIL_PHI_END_STD_NAMESPACE()

#endif // INCG_PHI_CORE_REFPTR_HPP
