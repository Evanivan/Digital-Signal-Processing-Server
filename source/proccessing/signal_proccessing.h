#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <algorithm>

/**
 * @class DataProcessor
 * @brief Обработчик 3D-данных.
 */
class DataProcessor {
public:

    /**
     * @brief Конструктор обработчика данных.
     *
     * @param data Вектор данных для обработки.
     * @param x Размерность по оси X.
     * @param y Размерность по оси Y.
     * @param z Размерность по оси Z.
     */
    DataProcessor(std::vector<float> data, uint32_t x, uint32_t y, uint32_t z) 
        : x_(x), y_(y), z_(z) {
        data_.reserve(data.size());
        std::move(data.begin(),data.end(), data_.begin());
    }
    
    /**
     * @brief Применяет медианный фильтр к данным по указанной оси.
     *
     * @param axis Ось, по которой применяется фильтр (0 - X, 1 - Y, 2 - Z).
     * @param window_size Размер окна фильтра.
     */
    void apply_median_filter(int axis, int window_size);

    /**
     * @brief Применяет гауссовский фильтр к данным по указанной оси.
     *
     * @param axis Ось, по которой применяется фильтр (0 - X, 1 - Y, 2 - Z).
     * @param sigma Сигма гауссовского фильтра.
     */
    void apply_gaussian_filter(int axis, double sigma);

    /**
     * @brief Применяет скользящее среднее к данным.
     *
     * @param window_size Размер окна усреднения.
     */
    void apply_moving_average(int window_size);
    
    /**
     * @brief Возвращает обработанные данные.
     *
     * @return Ссылка на вектор обработанных данных.
     */
    const std::vector<float>& get_processed_data() const {
        return data_;
    }

    /**
     * @brief Вычисляет статистику обработанных данных.
     *
     * @param mean Среднее значение.
     * @param stddev Стандартное отклонение.
     */
    void calculate_stats(double& mean, double& stddev) const;

private:
    // 3D-обработка
    //________________________________
    /**
     * @brief Обрабатывает данные по указанной оси с помощью заданной операции.
     *
     * @param axis Ось, по которой применяется обработка (0 - X, 1 - Y, 2 - Z).
     * @param op Операция, применяемая к данным.
     */
    void process_axis(int axis, const std::function<void(std::vector<float>&)>& op);
    
    // Утилиты
    //________________________________
    /**
     * @brief Возвращает значение ядра гауссовского фильтра.
     *
     * @param x Позиция в ядре.
     * @param sigma Сигма гауссовского фильтра.
     * @return Значение ядра.
     */
    float gaussianKernel(float x, double sigma) const;

    /**
     * @brief Возвращает срез данных по указанной оси.
     *
     * @param axis Ось, по которой берется срез (0 - X, 1 - Y, 2 - Z).
     * @param idx1 Индекс начала среза.
     * @param idx2 Индекс конца среза.
     * @return Вектор среза данных.
     */
    std::vector<float> get_slice(int axis, uint32_t idx1, uint32_t idx2) const;

    /**
     * @brief Устанавливает срез данных по указанной оси.
     *
     * @param axis Ось, по которой устанавливается срез (0 - X, 1 - Y, 2 - Z).
     * @param idx1 Индекс начала среза.
     * @param idx2 Индекс конца среза.
     * @param slice Вектор среза данных для установки.
     */
    void set_slice(int axis, uint32_t idx1, uint32_t idx2, const std::vector<float>& slice);

    std::vector<float> data_;   // Вектор обработанных данных.
    uint32_t x_, y_, z_;        // Размерности данных.
};
