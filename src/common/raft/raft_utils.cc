#include "raft_utils.h"

namespace raft_utils {
int
gpu_device_manager::random_choose() const {
    srand(time(NULL));
    return rand() % memory_load_.size();
}

int
gpu_device_manager::choose_with_load(size_t load) {
    std::lock_guard<std::mutex> lock(mtx_);
    int idx = -1;
    size_t min_load = std::numeric_limits<size_t>::max();
    for (size_t i = 0; i < memory_load_.size(); ++i) {
        if (min_load > memory_load_[i]) {
            idx = i;
            min_load = memory_load_[i];
        }
    }
    memory_load_[idx] += load;
    return idx;
}

gpu_device_manager::gpu_device_manager() {
    int device_counts;
    try {
        RAFT_CUDA_TRY(cudaGetDeviceCount(&device_counts));
    } catch (...) {
        LOG_KNOWHERE_FATAL_ << "cuda runtime exec failed.";
    }
    memory_load_.resize(device_counts);
    std::for_each(memory_load_.begin(), memory_load_.end(), [](size_t& x) { x = 0; });
}

gpu_device_manager&
gpu_device_manager::instance() {
    static gpu_device_manager mgr;
    return mgr;
}

}  // namespace raft_utils
