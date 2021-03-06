
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */
 
#include "nckTransform.h"
#include "nckVec2.h"

_MATH_BEGIN

Math::Mat44 VecToMat(const Math::Vec3 & vec, VectorAxis axisIndex, VectorAxis upIndex, const Math::Vec3 & up){
    Math::Vec3 n = Math::Normalize(vec);

    if(Math::Dot(n,n) == 0.0f)
        n = Math::Vec3(0,0,1);

    Math::Vec3 proj = n * (Math::Dot(up,n) / Math::Dot(n,n));
    proj = Math::Normalize(up - proj);

    if(Math::Dot(proj,proj) == 0.0f )
        proj = Math::Vec3(0,1,0);

    Math::Vec3 right = Math::Normalize(Math::Cross(proj,n));

    if(axisIndex != upIndex){
        int right_index = 3 - (int)axisIndex - (int)upIndex;
        float neg = 0.0f;
        switch (axisIndex - upIndex){
                case 1:
                case -2:
                    neg = 1;break;
                case -1:
                case 2:
                    neg = -1;break;
                default:
                    break;
        }

        Math::Vec3 mR[3];
        mR[(int)upIndex] = proj;
        mR[(int)axisIndex] = n;
        mR[(int)right_index] = right * neg;

        float mF[] = {    mR[0].GetX()    ,mR[0].GetY()    ,mR[0].GetZ()    ,0.0f,
                        mR[1].GetX()    ,mR[1].GetY()    ,mR[1].GetZ()    ,0.0f,
                        mR[2].GetX()    ,mR[2].GetY()    ,mR[2].GetZ()    ,0.0f,
                        0.0f            ,0.0f            ,0.0f            ,1.0f};

        return Math::Mat44(mF);
    }

    return Math::Identity();
}

Math::Mat44 LookAt(const Math::Vec3 & target, const Math::Vec3 & position, const Math::Vec3 & up){
    Math::Vec3 f = Math::Normalize(target - position);
    Math::Vec3 s = Math::Cross(f, up);
    Math::Vec3 u = Math::Cross(Math::Normalize(s), f);

    const float m[16] = {
             s.GetX(),           u.GetX(),           -f.GetX(),          0,
             s.GetY(),           u.GetY(),           -f.GetY(),          0,
             s.GetZ(),           u.GetZ(),           -f.GetZ(),          0,
             position.GetX(),     position.GetY(),     position.GetZ(),    1};

     return Math::Mat44(m);
}

Math::Vec2 Project(const Math::Mat44 & projection, const Math::Mat44 & view,const Math::Vec3 & vec, const Math::Vec2 & vpSize) {
     const Math::Vec4 proj_pos = Math::Vec4(vec, 1.0) * view * projection;
     float x = (proj_pos.GetX() / proj_pos.GetW() / 2.0 + 0.5) * vpSize.GetX();
     float y = (-proj_pos.GetY() / proj_pos.GetW() / 2.0 + 0.5) * vpSize.GetY();
     return Math::Vec2(x, y);
}

Math::Vec3 UnProject(const Math::Mat44 & projection, const Math::Mat44 & view, const Math::Vec2 & point, const Math::Vec2 & vpSize, float winZ) {
    Math::Mat44 transformMat = (view * projection);
    Math::Mat44 invMat;

    Math::Invert(transformMat, &invMat);
        
    float x = (point.GetX() / vpSize.GetX()) * 2.0 - 1.0;
    float y = (point.GetY() / vpSize.GetY()) * 2.0 - 1.0;
    float z = 2.0 * winZ - 1.0;
    float w = 1.0;

    Math::Vec4 v = Math::Vec4(x, y, z, w) * invMat;
    w = 1.0 / v.GetW();
    return Math::Vec3(v) * w;
}

_MATH_END
