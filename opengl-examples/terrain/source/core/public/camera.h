#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES
#include <cmath>

typedef struct {
    double x, y, z;
} Vector3D;

extern Vector3D vectorCross(Vector3D a, Vector3D b);
extern double vectorMagnitude(Vector3D a);
extern void vectorNormalize(Vector3D *a);

class Camera {
private:
    static constexpr double MAX_PITCH = 89.0 * M_PI / 180.0;
    static constexpr double MIN_PITCH = -89.0 * M_PI / 180.0;

    Vector3D pos;
    Vector3D vUp;        // Вверх
    Vector3D vView;      // То куда смотрит камера
    Vector3D vStrafe;    // В сторону

    double currentPitch;  // Текущий наклон камеры (чтобы нельзя было смотреть ровно вверх и вниз)

    void rotate_view(double angle, double x, double y, double z); // Вращение головой
    void update_strafe();
public:
    double mouseSensitivity = 1E-3;

    Camera(double pos_x, double pos_y, double pos_z, double view_x, double view_y, double view_z);

    void move_camera(double speed); // Движение вперёд назад
    void strafe(double speed);      // Движение в сторону
    void upDown(double speed);      // Движение вверх вниз

    void setViewByMouse(double deltaX, double deltaY); // Принимает отклонение в пикселях
    void look();    // Установка камеры
};

#endif