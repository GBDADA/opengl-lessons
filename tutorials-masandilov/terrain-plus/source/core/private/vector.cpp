#include "vector.h"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <assert.h>

// Вспомогательная функция
inline bool areSame(float a, float b) {
    return std::fabs(a - b) < FLT_EPSILON;
}

CVector::CVector() { set(0,0,0); };
CVector::CVector(float x, float y, float z) { set(x, y ,z); };
CVector::CVector(const CVector &vec) { set(vec.x, vec.y, vec.z); };

CVector& CVector::operator=(const CVector &vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

CVector CVector::operator+(const CVector &vec) const {
    return CVector(x + vec.x, y + vec.y, z + vec.z);
}

CVector CVector::operator-(const CVector &vec) const {
    return CVector(x - vec.x, y + vec.y, z + vec.z);
}

CVector CVector::operator-() const {
    return CVector(-x, -y, -z);
}

void CVector::operator+=(const CVector &vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

void CVector::operator-=(const CVector &vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}

bool CVector::operator==(const CVector &vec) const {
    return areSame(x, vec.x) && areSame(y, vec.y) && areSame(z, vec.z);
}

bool CVector::operator!=(const CVector &vec) const {
    return !(*this == vec);
}

float CVector::operator*(const CVector &vec) const {
    return x*vec.x + y*vec.y + z*vec.z;
}

void CVector::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void CVector::set(const CVector &vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

void CVector::normalize() {
    float len = magnitude();
    assert(len != 0);
    x /= len;
    y /= len;
    z /= len;
}

float CVector::magnitude() const {
    return std::sqrt(x*x + y*y + z*z);
}

void CVector::scale(float a) {
    x *= a;
    y *= a;
    z *= a;
}

void CVector::crossProduct(const CVector &vec, CVector &res) const {
    res.x = y * vec.z - z * vec.y;
    res.y = z * vec.x - x * vec.z;
    res.z = x * vec.y - y * vec.x;
}

CVector CVector::crossProduct(const CVector &vec) const {
    return CVector(
        y * vec.z - z * vec.y,
        z * vec.x - x * vec.z,
        x * vec.y - y * vec.x
    );
}