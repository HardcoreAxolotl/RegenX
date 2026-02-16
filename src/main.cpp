#include <iostream>
#include <RegenX/Core/RegenXMath.hpp>

int main(int argc, char **argv)
{
    regenx::math::matrix3<float> translationMatrix = regenx::math::matrix3<float>::translation({2.0f, 3.0f});
    regenx::math::matrix3<float> rotationMatrix = regenx::math::matrix3<float>::rotation(45.0f);
    regenx::math::matrix3<float> scalingMatrix = regenx::math::matrix3<float>::scaling({2.0f, 2.0f});

    return 0;
}
