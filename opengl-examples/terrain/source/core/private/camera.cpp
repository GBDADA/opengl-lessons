#include "camera.h"
#include <GL/glu.h>

// Реализуем математику необходимую для камеры

Vector3D vectorCross(Vector3D a, Vector3D b) {
    Vector3D result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);

    return result;
}

double vectorMagnitude(Vector3D a) {
    return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

void vectorNormalize(Vector3D *a) {
    double len = vectorMagnitude(*a);

    a->x /= len;
    a->y /= len;
    a->z /= len;
}

// Делаем саму камеру

Camera::Camera(double pos_x, double pos_y, double pos_z, double view_x, double view_y, double view_z) {
    pos.x = pos_x;
    pos.y = pos_y;
    pos.z = pos_z;

    Vector3D deltaVector = {
        view_x - pos.x,
        view_y - pos.y,
        view_z - pos.z
    };
    vectorNormalize(&deltaVector);

    vView = {
        pos.x + deltaVector.x,
        pos.y + deltaVector.y,
        pos.z + deltaVector.z
    };

    vUp = {0, 1, 0};

    update_strafe();

    currentPitch = 0;
}

void Camera::rotate_view(double angle, double x, double y, double z) {
    vView = {
        vView.x - pos.x,
        vView.y - pos.y,
        vView.z - pos.z
    };

    double cosTheta = std::cos(angle);
    double sinTheta = std::sin(angle);

    Vector3D vNewView;
    // Поворачиваем вектор используя матрицу поворота
    vNewView.x = (cosTheta + (1 - cosTheta) * x * x) * vView.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

    vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y)	* vView.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

    vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z)	* vView.z;

    vView = {
        pos.x + vNewView.x,
        pos.y + vNewView.y,
        pos.z + vNewView.z
    };

    update_strafe();
}

void Camera::move_camera(double speed) {
    Vector3D deltaVector = {
        vView.x - pos.x,
        0,
        vView.z - pos.z
    };

    vectorNormalize(&deltaVector);

    pos = {
        pos.x + deltaVector.x * speed,
        pos.y + deltaVector.y * speed,
        pos.z + deltaVector.z * speed
    };

    vView = {
        vView.x + deltaVector.x * speed,
        vView.y + deltaVector.y * speed,
        vView.z + deltaVector.z * speed
    };
}

void Camera::strafe(double speed) {
    pos = {
        pos.x + vStrafe.x * speed,
        pos.y + vStrafe.y * speed,
        pos.z + vStrafe.z * speed
    };

    vView = {
        vView.x + vStrafe.x * speed,
        vView.y + vStrafe.y * speed,
        vView.z + vStrafe.z * speed
    };
}

void Camera::upDown(double speed) {
    pos.y += speed;
    vView.y += speed;
}

void Camera::setViewByMouse(double deltaX, double deltaY) {
    if (deltaX == 0 && deltaY == 0) return;

    deltaX *= mouseSensitivity;
    deltaY *= mouseSensitivity;

    if (currentPitch + deltaY > MAX_PITCH) {
        deltaY = MAX_PITCH - currentPitch;
        currentPitch = MAX_PITCH;
    } else if (currentPitch + deltaY < MIN_PITCH) {
        deltaY = MIN_PITCH - currentPitch;
        currentPitch = MIN_PITCH;
    } else {
        currentPitch += deltaY;
    }

    if (deltaY != 0) {
        rotate_view(deltaY, vStrafe.x, vStrafe.y, vStrafe.z);
    }

    if (deltaX != 0) {
        rotate_view(deltaX, 0, 1, 0);
    }
}

void Camera::look() {
    gluLookAt(
        pos.x, pos.y, pos.z,
        vView.x, vView.y, vView.z,
        vUp.x, vUp.y, vUp.z
    );
}

void Camera::update_strafe() {
    Vector3D deltaVector = {
        vView.x - pos.x,
        vView.y - pos.y,
        vView.z - pos.z
    };

    vStrafe = vectorCross(deltaVector, vUp);
    vectorNormalize(&vStrafe);
}






