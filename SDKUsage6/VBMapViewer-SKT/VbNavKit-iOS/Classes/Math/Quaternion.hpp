#pragma once
#include "Matrix.hpp"
#include <iostream>

template <typename T>
struct QuaternionT {
    T x;
    T y;
    T z;
    T w;
    
    QuaternionT();
    QuaternionT(T x, T y, T z, T w);
    
    QuaternionT<T> Slerp(T mu, const QuaternionT<T>& q) const;
    QuaternionT<T> Rotated(const QuaternionT<T>& b) const;
    QuaternionT<T> Scaled(T scale) const;
    T Dot(const QuaternionT<T>& q) const;
    Matrix3<T> ToMatrix() const;
    Vector4<T> ToVector() const;
    QuaternionT<T> operator-(const QuaternionT<T>& q) const;
    QuaternionT<T> operator+(const QuaternionT<T>& q) const;
    bool operator==(const QuaternionT<T>& q) const;
    bool operator!=(const QuaternionT<T>& q) const;
    
    void Normalize();
    void Rotate(const QuaternionT<T>& q);
    
    static QuaternionT<T> CreateFromVectors(const Vector3<T>& v0, const Vector3<T>& v1);
    static QuaternionT<T> CreateFromAxisAngle(const Vector3<T>& axis, float radians);
    
    static QuaternionT<T> CreateFromNegativeZtoPitchDegree(float fPitchDegree); //-Z를 Pitch Degree만큼 조정하는 쿼터니언을 반환
    static QuaternionT<T> CreateFromNegativeZtoCWYawDegreeOnXZ(float fYawDegreeCW); //-Z를 XZ 평면에서 시계 방향으로 Yaw Degree만큼 조정하는 쿼터니언을 반환
    static QuaternionT<T> CreateFromNegativeZtoCWYawAndPitchDegree(float fYawDegreeCW,float fPitchDegree); //-Z에서 yaw,pitch를 향하는 쿼터니언을 구함.

   
    //Additional function
    vec3            operator*(const vec3 vec);            //입력받은 벡터에 이 쿼터니언을 곱해서 새로운 벡터를 반환한다.
    QuaternionT<T>  operator*(const QuaternionT<T> qtn);   //입력받은 쿼터니언에 이 쿼터니언을 곱해서 새로운 쿼터니언을 반환한다.
    QuaternionT<T>  operator*(const float scalar);          //입력받은 쿼터니언에 이 실수를 곱해서 새로운 쿼터니언을 반환한다.
    QuaternionT<T>  operator/(const float scalar);          //입력받은 쿼터니언에 이 실수를 나눠서 새로운 쿼터니언을 반환한다.
    
    T               Magnitude();    //vector size
    QuaternionT<T>  Conjugation();  //Get Conjgation quaternion
    QuaternionT<T>  Inverse();      //Get Inverse quaternion
};

template <typename T>
inline QuaternionT<T>::QuaternionT() : x(0), y(0), z(0), w(1)
{
}

template <typename T>
inline QuaternionT<T>::QuaternionT(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
{
}

// Ken Shoemake's famous method.
template <typename T>
inline QuaternionT<T> QuaternionT<T>::Slerp(T t, const QuaternionT<T>& v1) const
{
    const T epsilon = 0.0005f;
    T dot = Dot(v1);
    
    if (dot > 1 - epsilon) {
        QuaternionT<T> result = v1 + (*this - v1).Scaled(t);
        result.Normalize();
        return result;
    }
    
    if (dot < 0)
        dot = 0;
    
    if (dot > 1)
        dot = 1;
    
    T theta0 = std::acos(dot);
    T theta = theta0 * t;
    
    QuaternionT<T> v2 = (v1 - Scaled(dot));
    v2.Normalize();
    
    QuaternionT<T> q = Scaled(std::cos(theta)) + v2.Scaled(std::sin(theta));
	q.Normalize();
	return q;
}

template <typename T>
inline QuaternionT<T> QuaternionT<T>::Rotated(const QuaternionT<T>& b) const
{
    QuaternionT<T> q;
    q.w = w * b.w - x * b.x - y * b.y - z * b.z;
    q.x = w * b.x + x * b.w + y * b.z - z * b.y;
    q.y = w * b.y + y * b.w + z * b.x - x * b.z;
    q.z = w * b.z + z * b.w + x * b.y - y * b.x;
    q.Normalize();
    return q;
}

template <typename T>
inline QuaternionT<T> QuaternionT<T>::Scaled(T s) const
{
    return QuaternionT<T>(x * s, y * s, z * s, w * s);
}

template <typename T>
inline T QuaternionT<T>::Dot(const QuaternionT<T>& q) const
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

template <typename T>
inline Matrix3<T> QuaternionT<T>::ToMatrix() const
{
    const T s = 2;
    T xs, ys, zs;
    T wx, wy, wz;
    T xx, xy, xz;
    T yy, yz, zz;
    xs = x * s;  ys = y * s;  zs = z * s;
    wx = w * xs; wy = w * ys; wz = w * zs;
    xx = x * xs; xy = x * ys; xz = x * zs;
    yy = y * ys; yz = y * zs; zz = z * zs;
    Matrix3<T> m;
    m.x.x = 1 - (yy + zz); m.y.x = xy - wz;  m.z.x = xz + wy;
    m.x.y = xy + wz; m.y.y = 1 - (xx + zz); m.z.y = yz - wx;
    m.x.z = xz - wy; m.y.z = yz + wx;  m.z.z = 1 - (xx + yy);
    return m;
}

template <typename T>
inline Vector4<T> QuaternionT<T>::ToVector() const
{
    return Vector4<T>(x, y, z, w);
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator-(const QuaternionT<T>& q) const
{
    return QuaternionT<T>(x - q.x, y - q.y, z - q.z, w - q.w);
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator+(const QuaternionT<T>& q) const
{
    return QuaternionT<T>(x + q.x, y + q.y, z + q.z, w + q.w);
}

template <typename T>
bool QuaternionT<T>::operator==(const QuaternionT<T>& q) const
{
    return x == q.x && y == q.y && z == q.z && w == q.w;
}

template <typename T>
bool QuaternionT<T>::operator!=(const QuaternionT<T>& q) const
{
    return !(*this == q);
}


//Additional function - 입력받은 벡터에 이 쿼터니언을 곱해서 새로운 벡터를 반환한다.
template <typename T>
vec3  QuaternionT<T>::operator*(const vec3 vec)
{
    vec3 qvec   = vec3(x,y,z);
    vec3 uv     = qvec.Cross(vec);
    vec3 uuv    = qvec.Cross(uv);
    
    uv = uv * 2.f * w;
    uuv = uuv * 2.f;
    
    vec3 result = uv;
    result += vec;
    result += uuv;
    
    return result;
}

template <typename T>
QuaternionT<T>  QuaternionT<T>::operator*(const QuaternionT<T> qtn)
{
    QuaternionT newqtn;
    
    newqtn.w = w * qtn.w - x * qtn.x - y * qtn.y - z * qtn.z;
    newqtn.x = w * qtn.x + x * qtn.w + y * qtn.z - z * qtn.y;
    newqtn.y = w * qtn.y + y * qtn.w + y * qtn.x - x * qtn.z;
    newqtn.z = w * qtn.z + z * qtn.w + x * qtn.y - y * qtn.x;
    
    return newqtn;
}

template <typename T>
QuaternionT<T>  QuaternionT<T>::operator*(const float scalar)
{
    QuaternionT newqtn;
    
    newqtn.w = w * scalar;
    newqtn.x = x * scalar;
    newqtn.y = y * scalar;
    newqtn.z = z * scalar;
    
    return newqtn;
}

template <typename T>
QuaternionT<T>  QuaternionT<T>::operator/(const float scalar)
{
    QuaternionT newqtn;
    if(scalar==0.f) return newqtn;
    
    newqtn.w = w / scalar;
    newqtn.x = x / scalar;
    newqtn.y = y / scalar;
    newqtn.z = z / scalar;
    
    return newqtn;
}


// Compute the quaternion that rotates from a to b, avoiding numerical instability.
// Taken from "The Shortest Arc Quaternion" by Stan Melax in "Game Programming Gems".
template <typename T>
inline QuaternionT<T> QuaternionT<T>::CreateFromVectors(const Vector3<T>& v0, const Vector3<T>& v1)
{
    if (v0 == -v1)
    {
        return QuaternionT<T>::CreateFromAxisAngle(vec3(0, 1, 0), Pi);
    }
    Vector3<T> c = v0.Cross(v1);
    T d = v0.Dot(v1);
    T s = std::sqrt((1 + d) * 2);

    QuaternionT<T> q;
    q.x = c.x / s;
    q.y = c.y / s;
    q.z = c.z / s;
    q.w = s / 2.0f;
    return q;
}

template <typename T>
inline QuaternionT<T>  QuaternionT<T>::CreateFromAxisAngle(const Vector3<T>& axis, float radians)
{
    QuaternionT<T> q;
    q.w = std::cos(radians / 2);
    q.x = q.y = q.z = std::sin(radians / 2);
    q.x *= axis.x;
    q.y *= axis.y;
    q.z *= axis.z;
    return q;
}

template <typename T>
inline QuaternionT<T>  QuaternionT<T>::CreateFromNegativeZtoPitchDegree(float fPitchDegree)
{
    vec3 pitch_dir;
    pitch_dir.x = 0.f;
    pitch_dir.y = sin(DEG2RAD(fPitchDegree));
    pitch_dir.z = -cos(DEG2RAD(fPitchDegree));
    
    QuaternionT<T> pitch_quat = CreateFromVectors(VEC3_UNITZ * (-1.f),pitch_dir);
    pitch_quat.Normalize();
    
    return pitch_quat;
}


template <typename T>
inline QuaternionT<T>  QuaternionT<T>::CreateFromNegativeZtoCWYawDegreeOnXZ(float fYawDegreeCW)
{
    
    if(fabs(fYawDegreeCW)>179.99f && fabs(fYawDegreeCW)<180.01f)
        return QuaternionT<T>::CreateFromAxisAngle(vec3(0, 1, 0), Pi);
    
    vec3 dir_yaw;
    
    //yaw는 y축을 중심으로 -z방향으로부터 시계 방향으로 XZ 평면 위에서의 회전을 정의한다.
    dir_yaw.x = sin(DEG2RAD(fYawDegreeCW));
    dir_yaw.y = 0.f;
    dir_yaw.z = -cos(DEG2RAD(fYawDegreeCW));
    dir_yaw.Normalize();
    
    //std::cout << "Dir yaw(" << dir_yaw.x <<", " << dir_yaw.z << std::endl;
    
    //수평 회전 성분
    QuaternionT<T>  yaw_quat = CreateFromVectors(VEC3_UNITZ * (-1.f),dir_yaw);
    yaw_quat.Normalize();
    
    return yaw_quat;
}

template <typename T>
inline QuaternionT<T>  QuaternionT<T>::CreateFromNegativeZtoCWYawAndPitchDegree(float fYawDegreeCW,float fPitchDegree)
{
    vec3 dir_yaw;
    
    //yaw는 y축을 중심으로 -z방향으로부터 시계 방향으로 XZ 평면 위에서의 회전을 정의한다.
    QuaternionT orbit_h = CreateFromNegativeZtoCWYawDegreeOnXZ(fYawDegreeCW);
    //////////////////////////////////위로 올릴 쿼터니언.
    QuaternionT orbit_v = CreateFromNegativeZtoPitchDegree(fPitchDegree);
    
    return orbit_h.Rotated(orbit_v);
}    
    
    
    
template <typename T>
inline void QuaternionT<T>::Normalize()
{
    *this = Scaled(1 / std::sqrt(Dot(*this)));
}

template <typename T>
inline void QuaternionT<T>::Rotate(const QuaternionT<T>& q2)
{
    QuaternionT<T> q;
    QuaternionT<T>& q1 = *this;
    
    q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    q.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
    q.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;
    
    q.Normalize();
    *this = q;
}

template <typename T>
inline    T   QuaternionT<T>::Magnitude()    //vector size
{
    return sqrt(x*x + y*y + z*z + w*w);
}

template <typename T>
inline  QuaternionT<T>  QuaternionT<T>::Conjugation()  //Get Conjugation quaternion
{  //q = w + ax + by + cz --> conjugation q' = w - ax -by - cz
    QuaternionT<T> q;
    q.w = w;
    q.x = -x;
    q.y = -y;
    q.z = -z;
    
    return q;
}

template <typename T>
inline  QuaternionT<T>  QuaternionT<T>::Inverse()  //Get Inverse quaternion
{ 
    T   length = Magnitude();
    QuaternionT<T>  q = Conjugation();
    q = q / length;
    return q;
}

typedef QuaternionT<float> Quaternion;
