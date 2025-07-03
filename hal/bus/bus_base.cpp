#include "bus_base.hpp"

#include "core/stream/ostream.hpp"
#include "core/system.hpp"
#include "hal/hal_result.hpp"

using namespace ymd;
using namespace ymd::hal;

void BusBase::Locker::lock(const LockRequest req) {
    sys::exception::disable_interrupt();
    on_interrupt_ = sys::exception::is_interrupt_acting();
    req_id_ = req.id();
    locked_ = true;
    sys::exception::enable_interrupt();
}

bool BusBase::Locker::is_owned_by(const LockRequest req) const {
    return (req_id_ == req.id()) and (sys::exception::is_interrupt_acting() == on_interrupt_);
}

hal::HalResult BusBase::begin(const LockRequest req) {
    if (not locker.is_locked()) {
        locker.lock(req);
        return lead(req);
    } else if (locker.is_owned_by(req)) {
        locker.lock(req);
        return lead(req);
    } else {
        return hal::HalResult::OccupiedByOther;
    }
}

void BusBase::end() {
    this->trail();
    locker.unlock();
}