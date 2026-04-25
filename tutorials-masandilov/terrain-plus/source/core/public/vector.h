#ifndef VECTOR_H
#define VECTOR_H

class CVector {
public:
    float x, y, z;

    CVector();
    CVector(float, float, float);
    CVector(const CVector &);

    CVector& operator=(const CVector &vec);
    CVector operator+(const CVector &vec) const;
    CVector operator-(const CVector &vec) const;
    CVector operator-() const;

    void operator+=(const CVector &vec);
    void operator-=(const CVector &vec);
    bool operator==(const CVector &vec) const;
    bool operator!=(const CVector &vec) const;

    float operator*(const CVector&vec) const;

    void set(float x, float y, float z);
    void set(const CVector &vec);

    void normalize();
    float magnitude() const;
    void scale(float);

    void crossProduct(const CVector &vec, CVector &res) const;
    CVector crossProduct(const CVector &vec) const;
private:

};

typedef CVector CPos;

#endif