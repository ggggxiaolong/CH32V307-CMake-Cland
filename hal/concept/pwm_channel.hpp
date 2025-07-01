#pragma once
#include "core/math/real.hpp"

namespace ymd::hal {

class pwmIntf {
   public:
    virtual void set_duty(const real_t duty) = 0;
    virtual pwmIntf& operator=(const real_t duty) final {
        this->set_duty(duty);
        return *this;
    }
};

template <typename T>
class Countable {
   public:
    virtual volatile T& cnt() = 0;
    virtual volatile T& cvr() = 0;
    virtual volatile T& arr() = 0;
};
}  // namespace ymd::hal