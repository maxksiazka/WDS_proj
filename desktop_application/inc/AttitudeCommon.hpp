#ifndef ATTITUDECOMMON_HPP_
#define ATTITUDECOMMON_HPP_

#include <mutex>
struct AttitudeData {
    double roll;
    double pitch;
};
class AttitudeBuffer {
  private:
    AttitudeData data_;
    std::mutex mutex_;
  public:
    void update_data(double roll, double pitch) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.roll = roll;
        data_.pitch = pitch;
    }
    AttitudeData get_data() {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_;
    }
};
#endif // ATTITUDECOMMON_HPP_
