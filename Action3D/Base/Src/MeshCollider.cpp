#include "MeshCollider.h"
#include "ResourceManager.h"

MeshCollider::MeshCollider()
{
    parent = nullptr;
}

MeshCollider::MeshCollider(Object3D* object)
{
    parent = object;
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::MakeFromMesh(CFbxMesh* mesh)
{
    // CFbxMesh����f�[�^���擾����
    // �������b�V���ɕʂ�Ă��邪�A�܂Ƃ߂ĂP�ɂ���
    int vTop = 0;
    for (DWORD m = 0; m < mesh->m_dwMeshNum; m++) {
        CFbxMeshArray* arr = &mesh->m_pMeshArray[m];
        int vNum = arr->m_dwVerticesNum;
        vertices.reserve(vertices.size() + vNum);
        for (int v = 0; v < vNum; v++) {
            if (mesh->m_nMeshType == 1)
                vertices.push_back(arr->m_vStaticVerticesNormal[v].Pos);
            else
                vertices.push_back(arr->m_vSkinVerticesNormal[v].Pos);
        }
        int iNum = arr->m_dwIndicesNum / 3;
        polygons.reserve(polygons.size() + iNum);
        for (int i = 0; i < iNum; i++) {
            PolygonInfo inf;
            inf.indices[0] = arr->m_nIndices[i * 3 + 0] + vTop;
            inf.indices[1] = arr->m_nIndices[i * 3 + 1] + vTop;
            inf.indices[2] = arr->m_nIndices[i * 3 + 2] + vTop;
            VECTOR3 v0 = vertices[inf.indices[0]];
            VECTOR3 v1 = vertices[inf.indices[1]];
            VECTOR3 v2 = vertices[inf.indices[2]];
            inf.normal = XMVector3Normalize(XMVector3Cross(v1 - v0, v2 - v0));
            inf.mesh = m;
            polygons.push_back(inf);
        }
        vTop += vNum;
    }
    bBox.min = vertices[0];
    bBox.max = vertices[0];
    bBall.center = vertices[0];
    for (int v = 1; v < vertices.size(); v++) {
        if (bBox.min.x > vertices[v].x) bBox.min.x = vertices[v].x;
        if (bBox.max.x < vertices[v].x) bBox.max.x = vertices[v].x;
        if (bBox.min.y > vertices[v].y) bBox.min.y = vertices[v].y;
        if (bBox.max.y < vertices[v].y) bBox.max.y = vertices[v].y;
        if (bBox.min.z > vertices[v].z) bBox.min.z = vertices[v].z;
        if (bBox.max.z < vertices[v].z) bBox.max.z = vertices[v].z;
        bBall.center += vertices[v];
    }
    bBall.center /= vertices.size(); // ���������S�ɂȂ�
    bBall.radius = 0.0f; // �܂��͋����̓��̍ő�l�����߂�
    for (int v = 1; v < vertices.size(); v++) {
        float lenSq = (vertices[v] - bBall.center).LengthSquare();
        if (bBall.radius < lenSq)
            bBall.radius = lenSq;
    }
    bBall.radius = sqrtf(bBall.radius);
}

void MeshCollider::MakeFromFile(std::string fileName)
{
    CFbxMesh* mesh = new CFbxMesh();
    mesh->Load(fileName.c_str());
    MakeFromMesh(mesh);
}

bool MeshCollider::CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* hitOut)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invFrom = from * invTrans;
    VECTOR3 invTo = to * invTrans;

    // �o�E���f�B���O�{�[���Ŕ���
    VECTOR3 center = (invTo - invFrom) / 2.0f;
    float radius = (invTo - invFrom).Length() / 2.0f;
    float radiusSq = (radius + bBall.radius) * (radius + bBall.radius);
    if ((center - bBall.center).LengthSquare() > radiusSq) {
        return false;
    }
    // �o�E���f�B���O�{�b�N�X�Ŕ���
    if (bBox.min.x > invFrom.x && bBox.min.x > invTo.x) return false;
    if (bBox.max.x < invFrom.x && bBox.max.x < invTo.x) return false;
    if (bBox.min.y > invFrom.y && bBox.min.y > invTo.y) return false;
    if (bBox.max.y < invFrom.y && bBox.max.y < invTo.y) return false;
    if (bBox.min.z > invFrom.z && bBox.min.z > invTo.z) return false;
    if (bBox.max.z < invFrom.z && bBox.max.z < to.z) return false;

    float maxLengthSq = (to - from).LengthSquare();
    float minLengthSq = maxLengthSq;
    CollInfo minColl;
    for (const PolygonInfo& pol : polygons) {
        CollInfo coll;
        if (checkPolygonToLine(pol, invFrom, invTo, &coll)) {
            float lenSq = (coll.hitPosition - invFrom).LengthSquare();
            if (minLengthSq > lenSq) {
                minColl = coll;
                minLengthSq = lenSq;
            }
        }
    }
    if (minLengthSq < maxLengthSq) { // 1�ȏ㌩�����Ă���
        if (hitOut != nullptr) {
            hitOut->hitPosition = minColl.hitPosition * trans;
            hitOut->triangle[0] = minColl.triangle[0] * trans;
            hitOut->triangle[1] = minColl.triangle[1] * trans;
            hitOut->triangle[2] = minColl.triangle[2] * trans;
            VECTOR4 n = VECTOR4(minColl.normal);
            n.w = 0.0f;
            hitOut->normal = XMVector4Transform(n, trans);
        }
        return true;
    }
    return false;
}

bool MeshCollider::CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invCenter = center * invTrans;

    // �o�E���f�B���O�{�[��
    if ((invCenter - bBall.center).LengthSquare() > (bBall.radius + radius) * (bBall.radius + radius)) return false;

    // �o�E���f�B���O�{�b�N�X
    if (bBox.min.x > invCenter.x+radius) return false;
    if (bBox.max.x < invCenter.x-radius) return false;
    if (bBox.min.y > invCenter.y+radius) return false;
    if (bBox.max.y < invCenter.y-radius) return false;
    if (bBox.min.z > invCenter.z+radius) return false;
    if (bBox.max.z < invCenter.z-radius) return false;

    // ���̒��S����A���ʂɉ��������������߂�
    float minLengthSq = radius*radius;
    CollInfo minColl;
    bool found = false;
    for (const PolygonInfo& pol : polygons) {
        CollInfo coll;
        if (checkPolygonToSphere(pol, invCenter, radius, &coll)) {
            float lenSq = (coll.hitPosition - invCenter).LengthSquare();
            if (lenSq < minLengthSq) {
                minColl = coll;
                found = true;
            }
        }
    }
    if (!found) return false;

    if (hitOut != nullptr) {
        hitOut->hitPosition = minColl.hitPosition * trans;
        hitOut->triangle[0] = minColl.triangle[0] * trans;
        hitOut->triangle[1] = minColl.triangle[1] * trans;
        hitOut->triangle[2] = minColl.triangle[2] * trans;
        VECTOR4 n = VECTOR4(minColl.normal);
        n.w = 0.0f;
        hitOut->normal = XMVector4Transform(n, trans);
    }
    return true;
}

std::list<MeshCollider::CollInfo> MeshCollider::CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center, float radius)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invCenter = center * invTrans;
    std::list<CollInfo> meshes;

    // �o�E���f�B���O�{�[��
    if ((invCenter - bBall.center).LengthSquare() > (bBall.radius + radius) * (bBall.radius + radius)) return meshes;

    // �o�E���f�B���O�{�b�N�X
    if (bBox.min.x > invCenter.x + radius) return meshes;
    if (bBox.max.x < invCenter.x - radius) return meshes;
    if (bBox.min.y > invCenter.y + radius) return meshes;
    if (bBox.max.y < invCenter.y - radius) return meshes;
    if (bBox.min.z > invCenter.z + radius) return meshes;
    if (bBox.max.z < invCenter.z - radius) return meshes;

    // ���̒��S����A���ʂɉ��������������߂�
    for (const PolygonInfo& pol : polygons) {
        CollInfo coll;
        if (checkPolygonToSphere(pol, invCenter, radius, &coll)) {
            coll.hitPosition = coll.hitPosition * trans;
            VECTOR4 n = VECTOR4(coll.normal);
            n.w = 0.0f;
            coll.normal = XMVector4Transform(n, trans);
            meshes.push_back(coll);
        }
    }
    return meshes;
}

//bool MeshCollider::CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, MeshCollider::CollInfo* info)
//{
//    return false;
//}

//std::list<MeshCollider::CollInfo> MeshCollider::CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius)
//{
//    return std::list<MeshCollider::CollInfo>();
//}

//bool MeshCollider::CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info)
//{
//    // triangle�̂R�̃G�b�W����_�������A���݂��𒲂ׂ��OK
//    VECTOR3 pos[4];
//    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
//    for (int i = 0; i < 3; i++) {
//        pos[i] = vertexes[i];
//    }
//    pos[3] = pos[0];
//    float minX = min(min(pos[0].x, pos[1].x), pos[2].x);
//    float maxX = max(max(pos[0].x, pos[1].x), pos[2].x);
//    float minY = min(min(pos[0].y, pos[1].y), pos[2].y);
//    float maxY = max(max(pos[0].y, pos[1].y), pos[2].y);
//    float minZ = min(min(pos[0].z, pos[1].z), pos[2].z);
//    float maxZ = max(max(pos[0].z, pos[1].z), pos[2].z);
//
//    // �o�E���f�B���O�{�b�N�X�Ŕ���
//    if (bBox.min.x > maxX) return false;
//    if (bBox.max.x < minX) return false;
//    if (bBox.min.y > maxY) return false;
//    if (bBox.max.y < minY) return false;
//    if (bBox.min.z > maxZ) return false;
//    if (bBox.max.z < minZ) return false;
//
//    for (int i = 0; i < 3; i++) {
//        float maxLengthSq = (pos[i + 1] - pos[i]).LengthSquare();
//        float minLengthSq = maxLengthSq;
//        for (const PolygonInfo& pol : polygons) {
//            CollInfo coll;
//            if (checkPolygonToLine(pol, pos[i], pos[i + 1], &coll)) {
//                return true;
//            }
//        }
//    }
//    return false;
//}

bool MeshCollider::CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to)
{
    MATRIX4X4 invTrans = XMMatrixInverse(nullptr, trans);
    VECTOR3 invFrom = from * invTrans;
    VECTOR3 invTo = to * invTrans;

    // �o�E���f�B���O�{�[���Ŕ���
    VECTOR3 center = (invTo - invFrom) / 2.0f;
    float radius = (invTo - invFrom).Length() / 2.0f;
    float radiusSq = (radius + bBall.radius) * (radius + bBall.radius);
    if ((center - bBall.center).LengthSquare() > radiusSq) {
        return false;
    }
    // �o�E���f�B���O�{�b�N�X�Ŕ���
    if (bBox.min.x > invFrom.x && bBox.min.x > invTo.x) return false;
    if (bBox.max.x < invFrom.x && bBox.max.x < invTo.x) return false;
    if (bBox.min.y > invFrom.y && bBox.min.y > invTo.y) return false;
    if (bBox.max.y < invFrom.y && bBox.max.y < invTo.y) return false;
    if (bBox.min.z > invFrom.z && bBox.min.z > invTo.z) return false;
    if (bBox.max.z < invFrom.z && bBox.max.z < to.z) return false;

    return true;
}

bool MeshCollider::checkPolygonToLine(const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to, CollInfo* hit)
{
    // �@����0���ƁA�����_�Ȃ̂ŁA��_�͋��߂Ȃ�
    if (info.normal.LengthSquare() == 0.0f)
        return false;
    // �|���S�����܂ޖʂƂ̌��������邩�����߂�
    VECTOR3 v0 = vertices[info.indices[0]];
    VECTOR3 v1 = vertices[info.indices[1]];
    VECTOR3 v2 = vertices[info.indices[2]];
    float nFrom = Dot(from - v0, info.normal);
    float nTo = Dot(to - v0, info.normal);
    if (nFrom * nTo > 0.0f) // �x�N�g�������������Ȃ̂Ō������Ă��Ȃ�
        return false;
    nFrom = fabsf(nFrom);
    nTo = fabsf(nTo);
    // ��_�̍��W�����߂�
    VECTOR3 pos = from + (to - from) * nFrom / (nFrom + nTo);
    // �O�p�`�̒����𒲂ׂ�i�O�σx�N�g�����@���Ɠ��������j
    float n = Dot(XMVector3Cross(v1 - v0, pos - v0), info.normal);
    if (n < 0.0f)
        return false;
    n = Dot(XMVector3Cross(v2 - v1, pos - v1), info.normal);
    if (n < 0.0f)
        return false;
    n = Dot(XMVector3Cross(v0 - v2, pos - v2), info.normal);
    if (n < 0.0f)
        return false;
    hit->hitPosition = pos;
    hit->normal = info.normal;
    hit->triangle[0] = v0;
    hit->triangle[1] = v1;
    hit->triangle[2] = v2;
    return true;
}

bool MeshCollider::checkPolygonToSphere(const PolygonInfo& info, const VECTOR3& center, float radius, CollInfo* hit)
{
#define EDGE_HIT 1 // �|���S���̊O���̋���������ꏊ���v�Z����ꍇ

    // �@����0���ƁA�����_
    if (info.normal.LengthSquare() == 0.0f)
        return false;
    // ���S����|���S�����܂ޕ��ʂւ̐����̒��������߂�
    VECTOR3 v[3];
    v[0] = vertices[info.indices[0]];
    v[1] = vertices[info.indices[1]];
    v[2] = vertices[info.indices[2]];
    float len = Dot(center - v[0], info.normal);
    if (len < 0 || len > radius) return false; // �����̒��������a���傫��

    VECTOR3 pos = center - info.normal * len; // �����Ƃ̌�_�i��������_�j
    if (Dot(center - pos, info.normal) < 0) // �@�������Ȃ瓖����Ȃ�
        return false;

    // ��_���|���S���̓��������ׂ�i�O�ς̌������@���Ɠ����ɂȂ�͂��j
#if EDGE_HIT>0
    int hitIdx[3];
    int hitNum = 0;
#endif
    for (int vi = 0; vi < 3; vi++) {
        VECTOR3& v0 = v[vi];
        VECTOR3& v1 = v[(vi + 1) % 3];
        float n = Dot(XMVector3Cross(v1 - v0, pos - v0), info.normal);
        if (n < 0.0f) { // �ӂ̏�Ɍ�_������ƁA�O�ς�0�Ȃ̂ŁAn��0�ɂȂ�
#if EDGE_HIT>0
            hitIdx[hitNum++] = vi;
#else
            return false;
#endif
        }
    }
    if (hitNum == 0) {
        if (hit != nullptr) {
            hit->hitPosition = pos;
            hit->normal = info.normal;
            hit->triangle[0] = v[0];
            hit->triangle[1] = v[1];
            hit->triangle[2] = v[2];
        }
        return true;
    }
#if EDGE_HIT>0
    else if (hitNum==1) { // �P�����O���Ȃ̂ŁA���̃G�b�W�Ɋ񂹂�
        int vi = hitIdx[0];
        VECTOR3 edge = XMVector3Normalize(v[(vi + 1) % 3] - v[vi]); // ���������G�b�W�̌���
        float len = Dot(center - v[vi], edge);
        VECTOR3 hitPos = edge * len + v[vi];
        if ((hitPos-center).LengthSquare() > radius*radius)
            return false;
        if (hit != nullptr) {
            hit->hitPosition = hitPos;
            hit->normal = info.normal;
            hit->triangle[0] = v[0];
            hit->triangle[1] = v[1];
            hit->triangle[2] = v[2];
        }
        return true;
    } else { // �Q�̊O���Ȃ̂ŁA���̒��_�ɓ�����
        // 0-1��1-2�ɓ������Ă����1�A1-2��2-0�ɓ������Ă����2�A2-0��0-1�ɓ������Ă����0�̒��_���g���Ƃ����v�Z
        int n = hitIdx[0] + hitIdx[1];
        n = 2 - (n % 3);

        float lenSq = (center - v[n]).LengthSquare();
        if (lenSq > radius * radius)
            return false;
        if (hit != nullptr) {
            hit->hitPosition = v[n];
            hit->normal = info.normal;
            hit->triangle[0] = v[0];
            hit->triangle[1] = v[1];
            hit->triangle[2] = v[2];
        }
        return true;
    }
#endif
}
