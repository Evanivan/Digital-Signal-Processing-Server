#include <gtest/gtest.h>
#include "../source/proccessing/signal_proccessing.h"

TEST(DataProcessorTest, MedianFilter) {
    std::vector<float> input = {5.0f, 3.0f, 2.0f, 4.0f, 1.0f};
    DataProcessor dp(input, 5, 1, 1);
    
    dp.apply_median_filter(0, 3);
    const auto& result = dp.get_processed_data();
    
    ASSERT_FLOAT_EQ(result[2], 3.0f);
}

TEST(DataProcessorTest, StatsCalculation) {
    std::vector<float> data = {1.0f, 2.0f, 3.0f, 4.0f};
    DataProcessor dp(data, 2, 2, 1);
    
    double mean, stddev;
    dp.calculate_stats(mean, stddev);
    
    ASSERT_NEAR(mean, 2.5, 0.001);
    ASSERT_NEAR(stddev, 1.11803, 0.001);
}

TEST(DataProcessorTest, InvalidData) {
    std::vector<float> data(4, 0.0f);
    EXPECT_THROW(
        DataProcessor dp(data, 0, 2, 1),
        std::invalid_argument
    );
}
