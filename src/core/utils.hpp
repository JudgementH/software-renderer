#pragma once

#include <Eigen/Eigen>
#include <string>
#include <iostream>

namespace math {
    const float PI = 3.141592653589793;

    inline float Radian2Degree(float randian) {
        return randian * 180 / PI;
    }

    inline float Degree2Radian(float degree) {
        return degree * PI / 180;
    }

    static Eigen::Matrix4f GetViewPortMatrix(int screenWidth, int screenHeight) {
        /**
         * @brief set the viewPortMatrix, which transform NDC space to scene space.
         * viewPortMatrix = [   w/2,    0   ,  0    ,   w/2,
         *                      0  ,    h/2 ,  0    ,   h/2,
         *                      0  ,    0   ,  1    ,   0,
         *                      0  ,    0   ,  0    ,   1]
         *
         */
        Eigen::Matrix4f ans;
        ans << screenWidth / 2.0f, 0, 0, screenWidth / 2.0f,
                0, screenHeight / 2.0f, 0, screenHeight / 2.0f,
                0, 0, 1, 0,
                0, 0, 0, 1;
        return ans;
    }

    template<typename T>
    T LinearLerp(T x1, T x2, const float &t) {
        return (1.0f - t) * x1 + t * x2;
    }

    inline float ProjectDistance(const Eigen::Vector4f& plane, const Eigen::Vector4f& p) {
        return plane.dot(p);
    }

}