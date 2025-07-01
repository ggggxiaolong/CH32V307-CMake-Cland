#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

#include "core/constants/enums.hpp"
#include "core/math/uint24_t.hpp"

namespace ymd {

template <typename T>
struct Regcopy final : public T {
   private:
    T &owner_;

   public:
    using value_type = typename T::value_type;

    constexpr Regcopy(T &owner) : owner_(owner) {
        T &self = this;
        self.as_ref() = owner_.as_val();
    }

    constexpr void apply() const {
        const T &self = *this;
        owner_.as_ref() = self.as_val();
    }
};

template <typename T>
Regcopy(T) -> Regcopy<T>;

struct __RegBase {
    template <typename T>
    friend struct Regcopy;
};

// D = T：第二个模板类型参数，默认值为 T
template <typename T, typename D = T>
struct alignas(size_t(T)) __RegC_t : public __RegBase {
   protected:
    using TReg = T;
    __RegC_t() = default;

   public:
    __RegC_t(const std::span<const uint8_t> pbuf) { *(reinterpret_cast<T *>(this)) = *reinterpret_cast<const T *>(pbuf.data()); }
    using value_type = T;

    constexpr std::span<const uint8_t> as_bytes() const { return std::span(reinterpret_cast<const uint8_t *>(this), sizeof(TReg)); }

    constexpr const T &as_ref() const { return (reinterpret_cast<const T &>(this)); }

    constexpr T as_val() const { return (std::bit_cast<T>(*this)); }
    constexpr std::span<const T, 1> as_span() const { return std::span(reinterpret_cast<const T *>(*this), 1); }
};

template <typename T, typename D = T>
struct __Reg_t : public __RegC_t<T, D> {
   protected:
    constexpr __Reg_t<T> &operator=(const __Reg_t<T, D> &) = default;
    constexpr __Reg_t<T> &operator=(__Reg_t<T, D> &&) = default;
    constexpr __Reg_t(const T &data) { *this = data; };
    constexpr __Reg_t(T &&data) { *this = data; }

   public:
    using value_type = T;

    // 表示继承基类__RegC_t<T>的构造函数
    using __RegC_t<T>::__RegC_t;
    using __RegC_t<T>::as_bytes;
    // 将基类__RegC_t<T>中的成员as_bytes引入当前类的作用域中，使其可以直接访问
    using TReg = __RegC_t<T>::TReg;

    constexpr __Reg_t<T> &operator=(const T data) {
        *reinterpret_cast<T *>(this) = data;
        return *this;
    }

    constexpr std::span<uint8_t> as_bytes() { return std::span<uint8_t>(reinterpret_cast<uint8_t *>(this), sizeof(TReg)); }
    constexpr std::span<const uint8_t> as_bytes() const { return std::span<const uint8_t>(reinterpret_cast<const uint8_t *>(this), sizeof(TReg)); }

    constexpr __Reg_t<T> &set_bits(const T data) {
        static_cast<T &>(*this) = static_cast<T>(*this | static_cast<T>(data));
        return *this;
    }

    constexpr __Reg_t<T> &clear_bits(const T data) {
        static_cast<T &>(*this) = static_cast<T>(*this & ~static_cast<T>(~data));
        return *this;
    }

    constexpr __Reg_t<T> &reconf_bits(const T data) {
        static_cast<T &>(*this) = static_cast<T>(data);
        return *this;
    }

    constexpr T &as_ref() { return (reinterpret_cast<T &>(*this)); }
    constexpr const T &as_ref() const { return reinterpret_cast<const T &>(this); }
    // constexpr T as_ref() const { return reinterpret_cast<const T &>(*this); }

    constexpr std::span<T, 1> as_span() { return std::span(reinterpret_cast<T *>(this), 1); }
};

#define DEF_REG_TEMPLATE(name, T, as_fn)                              \
    template <typename D = T>                                         \
    struct name : public __Reg_t<T, D> {                              \
        using __Reg_t<T, D>::__Reg_t;                                 \
        using __Reg_t<T, D>::operator=;                               \
        constexpr T as_fn() const { return std::bit_cast<T>(*this); } \
    };

#define DEF_REGC_TEMPLATE(name, T, as_fn)              \
    template <typename D = T>                          \
    struct name : public __RegC_t<T, D> {              \
        using __RegC_t<T, D>::__RegC_t;                \
        using __RegC_t<T, D>::operator=;               \
        constexpr T as_fn() const { return T(*this); } \
    };

DEF_REG_TEMPLATE(Reg8, uint8_t, as_u8)
DEF_REG_TEMPLATE(Reg16, uint16_t, as_u16)
DEF_REG_TEMPLATE(Reg24, uint24_t, as_u24)
DEF_REG_TEMPLATE(Reg32, uint32_t, as_u32)
DEF_REG_TEMPLATE(Reg64, uint64_t, as_u64)

DEF_REG_TEMPLATE(Reg8i, int8_t, as_i8)
DEF_REG_TEMPLATE(Reg16i, int16_t, as_i16)
DEF_REG_TEMPLATE(Reg32i, int32_t, as_i32)
DEF_REG_TEMPLATE(Reg64i, int64_t, as_i64)

DEF_REGC_TEMPLATE(RegC8, uint8_t, as_u8)
DEF_REGC_TEMPLATE(RegC16, uint16_t, as_u16)
DEF_REGC_TEMPLATE(RegC24, uint24_t, as_u24)
DEF_REGC_TEMPLATE(RegC32, uint32_t, as_u32)
DEF_REGC_TEMPLATE(RegC64, uint64_t, as_u64)

DEF_REGC_TEMPLATE(RegC8i, int8_t, as_i8)
DEF_REGC_TEMPLATE(RegC16i, int16_t, as_i16)
DEF_REGC_TEMPLATE(RegC32i, int32_t, as_i32)
DEF_REGC_TEMPLATE(RegC64i, int64_t, as_i64)

#undef DEF_REG_TEMPLATE
#undef DEF_REGC_TEMPLATE

#define CHECK_R32(type)                                                                            \
    static_assert(sizeof(type) == 4, "type must be 32 bits");                                      \
    static_assert(std::has_unique_object_representations_v<type>, "type must be unique bitfield"); \
    // 用于判断某个类型的所有可能值是否具有唯一的对象表示（即：相同的值始终以相同的二进制形式存储在内存中

#define CHECK_R16(type)                                       \
    static_assert(sizeof(type) == 2, "type must be 32 bits"); \
    static_assert(std::has_unique_object_representations_v<type>, "type must be unique bitfield");

#define CHECK_R8(type)                                        \
    static_assert(sizeof(type) == 1, "type must be 32 bits"); \
    static_assert(std::has_unique_object_representations_v<type>, "type must be unique bitfield");

// decay_t 是 C++ 标准库中的一个类型别名模板，定义于 <type_traits> 头文件中。
// 功能解释如下：
// 它用于去除类型的“顶层”修饰，如数组到指针的退化、函数到函数指针的转换，以及移除 const、volatile 和引用等。
// 常用于泛型编程中，使模板参数更灵活地匹配各种类型。
#define DEF_R32(name) \
    name{};           \
    CHECK_R32(std::decay_t<decltype(name)>);

#define DEF_R16(name) \
    name{};           \
    CHECK_R16(std::decay_t<decltype(name)>);

#define DEF_R8(name) \
    name{};          \
    CHECK_R8(std::decay_t<decltype(name)>);

#define REG16I_QUICK_DEF(addr, type, name) \
    struct type : public Ref16i<> {        \
        scexpr uint8_t address = addr;     \
        int16_t data;                      \
    } DEF_R16(name)

#define REG16_QUICK_DEF(addr, type, name) \
    struct type : public Ref16<> {        \
        scexpr RegAddress address = addr; \
        int16_t data;                     \
    } DEF_R16(name);

#define REG8_QUICK_DEF(addr, type, name)  \
    struct type : public Ref8<> {         \
        scexpr RegAddress address = addr; \
        int8_t data;                      \
    } DEF_R16(name);
}  // namespace ymd