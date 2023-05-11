#pragma once

// std
#include <cstdint>
#include <algorithm>
// 3rd party
#include <boost/beast/core/span.hpp>
#include <auxiliary/math/bbox2op.hpp>

namespace c_m = callisto::math;

namespace auxml
{

class darknet_box_provider
{
    float* _data_ptr;

    uint32_t _classes_count;

public:
    // construct and destruct
    inline darknet_box_provider(float* data_ptr, uint32_t classes_count);

    // methods
    c_m::bbox2<float> get_bbox() const noexcept;

    double iou(const darknet_box_provider& provider) const noexcept;

    double iom(const darknet_box_provider& provider) const noexcept;

    // getters and setters
    inline float& x_c() noexcept;

    inline const float& x_c() const noexcept;

    inline float& y_c() noexcept;

    inline const float& y_c() const noexcept;

    inline float& width() noexcept;

    inline const float& width() const noexcept;

    inline float& height() noexcept;

    inline const float& height() const noexcept;

    inline float& object_confidence() noexcept;

    inline const float& object_confidence() const noexcept;

    inline boost::beast::span<float> class_probabilities() noexcept;

    inline boost::beast::span<const float> class_probabilities() const noexcept;
};

#pragma region construct_and_destruct

inline darknet_box_provider::darknet_box_provider(float* data_ptr, uint32_t classes_count)
{
    _data_ptr      = data_ptr;
    _classes_count = classes_count;
}

#pragma endregion

#pragma region methods

c_m::bbox2<float> darknet_box_provider::get_bbox() const noexcept
{
    return c_m::bbox2f(
        x_c() - width() / 2.0f,
        y_c() - height() / 2.0f,
        x_c() + width() / 2.0f,
        y_c() + height() / 2.0f
    );
}

double darknet_box_provider::iou(const darknet_box_provider& provider) const noexcept
{
    auto box1 = get_bbox();
    auto box2 = provider.get_bbox();

    return c_m::bbox2op_can::iou(box1, box2);
}

double darknet_box_provider::iom(const darknet_box_provider& provider) const noexcept
{
    auto box1 = get_bbox();
    auto box2 = provider.get_bbox();

    return c_m::bbox2op_can::iom(box1, box2);
}

#pragma endregion

#pragma region getters_and_setters

inline float& darknet_box_provider::x_c() noexcept { return _data_ptr[0]; }

inline const float& darknet_box_provider::x_c() const noexcept { return _data_ptr[0]; }

inline float& darknet_box_provider::y_c() noexcept { return _data_ptr[1]; }

inline const float& darknet_box_provider::y_c() const noexcept { return _data_ptr[1]; }

inline float& darknet_box_provider::width() noexcept { return _data_ptr[2]; }

inline const float& darknet_box_provider::width() const noexcept { return _data_ptr[2]; }

inline float& darknet_box_provider::height() noexcept { return _data_ptr[3]; }

inline const float& darknet_box_provider::height() const noexcept { return _data_ptr[3]; }

inline float& darknet_box_provider::object_confidence() noexcept { return _data_ptr[4]; }

inline const float& darknet_box_provider::object_confidence() const noexcept
{
    return _data_ptr[4];
}

inline boost::beast::span<float> darknet_box_provider::class_probabilities() noexcept
{
    return boost::beast::span<float>(_data_ptr + 5, _classes_count);
}

inline boost::beast::span<const float> darknet_box_provider::class_probabilities() const noexcept
{
    return boost::beast::span<const float>(_data_ptr + 5, _classes_count);
}

#pragma endregion

} // namespace auxml