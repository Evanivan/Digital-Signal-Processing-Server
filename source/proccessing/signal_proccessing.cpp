#include <algorithm>
#include <cmath>
#include <queue>
#include <numeric>
#include "signal_proccessing.h"

void DataProcessor::apply_median_filter(int axis, int window_size) {
    process_axis(axis, [window_size](std::vector<float>& slice) {
        std::vector<float> filtered(slice.size());
        const int half = window_size / 2;

        for (size_t i = 0; i < slice.size(); ++i) {
            int start = std::max(0, static_cast<int>(i) - half);
            int end = std::min(slice.size() - 1, i + half);
            
            std::vector<float> window(slice.begin() + start, slice.begin() + end + 1);
            std::nth_element(window.begin(), window.begin() + window.size()/2, window.end());
            filtered[i] = window[window.size()/2];
        }
        slice = std::move(filtered);
    });
}

void DataProcessor::apply_gaussian_filter(int axis, double sigma) {
    process_axis(axis, [sigma](std::vector<float>& slice) {
        const int kernel_size = static_cast<int>(sigma * 3) * 2 + 1;
        const int half = kernel_size / 2;
        std::vector<float> filtered(slice.size(), 0.0f);

        for (size_t i = 0; i < slice.size(); ++i) {
            float sum = 0.0f, weight_sum = 0.0f;
            
            for (int k = -half; k <= half; ++k) {
                const int pos = static_cast<int>(i) + k;
                if (pos < 0 || pos >= slice.size()) continue;
                
                const float weight = expf(-k*k / (2 * sigma * sigma));
                sum += slice[pos] * weight;
                weight_sum += weight;
            }
            filtered[i] = sum / weight_sum;
        }
        slice = std::move(filtered);
    });
}

void DataProcessor::process_axis(int axis, const std::function<void(std::vector<float>&)>& op) {
    if (axis == 0) {
        for (uint32_t y = 0; y < y_; ++y) {
            for (uint32_t z = 0; z < z_; ++z) {
                auto slice = get_slice(0, y, z);
                op(slice);
                set_slice(0, y, z, slice);
            }
        }
    } else {
        for (uint32_t x = 0; x < x_; ++x) {
            for (uint32_t z = 0; z < z_; ++z) {
                auto slice = get_slice(1, x, z);
                op(slice);
                set_slice(1, x, z, slice);
            }
        }
    }
}

std::vector<float> DataProcessor::get_slice(int axis, uint32_t idx1, uint32_t idx2) const {
    std::vector<float> slice;
    if (axis == 0) {
        slice.resize(x_);
        for (uint32_t x = 0; x < x_; ++x)
            slice[x] = data_[x + y_ * (idx1 + z_ * idx2)];
    } else {
        slice.resize(y_);
        for (uint32_t y = 0; y < y_; ++y)
            slice[y] = data_[idx1 + y_ * (y + z_ * idx2)];
    }
    return slice;
}

void DataProcessor::set_slice(int axis, uint32_t idx1, uint32_t idx2, const std::vector<float>& slice) {
    if (axis == 0) {
        for (uint32_t x = 0; x < x_; ++x)
            data_[x + y_ * (idx1 + z_ * idx2)] = slice[x];
    } else {
        for (uint32_t y = 0; y < y_; ++y)
            data_[idx1 + y_ * (y + z_ * idx2)] = slice[y];
    }
}

void DataProcessor::apply_moving_average(int window_size) {
    std::vector<float> result(data_.size());
    double sum = 0.0;
    size_t count = 0;

    for (size_t i = 0; i < data_.size(); ++i) {
        sum += data_[i];
        count++;
        
        if (i >= window_size) {
            sum -= data_[i - window_size];
            count--;
        }
        result[i] = sum / count;
    }
    data_ = std::move(result);
}

void DataProcessor::calculate_stats(double& mean, double& stddev) const {
    const double sum = std::accumulate(data_.begin(), data_.end(), 0.0);
    mean = sum / data_.size();
    
    double variance = 0.0;
    for (float val : data_)
        variance += (val - mean) * (val - mean);
    
    stddev = std::sqrt(variance / data_.size());
}