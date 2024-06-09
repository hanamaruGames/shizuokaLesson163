//-----------------------------------------------------------------------------
//
//  Direct3D�𗘗p���邽�߂̔ėp�R�c���C�u����         ver 2.8        2019.12.30
// 
//	�e��@���w�n�̃��C�u����
//
//
//																MyMath.h
//
//-----------------------------------------------------------------------------
#include "MyMath.h"


//-----------------------------------------------------------------------------
//  �����_���W�v�Z
//
//  ���_���璍���_�����邽�߂̃}�g���b�N�X���쐬����
//
//  ����		
//  const VECTOR3& vHear	���_�i�����̈ʒu���W�j
//  const VECTOR3& vLookat	�����_�i�ڕW�Ƃ���ʒu�̍��W�j
//
//	�߂�l  MATRIX4X4 mOut
//			�쐬���ꂽ�}�g���b�N�X
//
//-----------------------------------------------------------------------------
MATRIX4X4 GetLookatMatrix(const VECTOR3& vHear, const VECTOR3& vLookat)
{
	MATRIX4X4  matTemp, matTemp2, matRotX, matRotY, mOut;
	VECTOR3    vDist;
	float      mag_xz, mag_xyz;
	float      cos_x, sin_x, cos_y, sin_y;

	vDist = vLookat - vHear;
	mag_xyz = magnitude(vDist);
	vDist.y = 0.0f;
	mag_xz = magnitude(vDist);
	vDist = vLookat - vHear;

	if (1e-6f > mag_xyz) mag_xyz = 0.0001f;
	if (1e-6f > mag_xz)  mag_xz = 0.0001f;

	cos_y = vDist.z / mag_xz;
	sin_y = vDist.x / mag_xz;
	cos_x = mag_xz / mag_xyz;
	sin_x = -vDist.y / mag_xyz;

	// MAKE MATRIX
	matTemp = XMMatrixTranslation(vHear.x, vHear.y, vHear.z),

	matRotX = XMMatrixIdentity();
	matRotX._22 = cos_x;
	matRotX._23 = sin_x;
	matRotX._32 = -sin_x;
	matRotX._33 = cos_x;

	matRotY = XMMatrixIdentity();
	matRotY._11 = cos_y;
	matRotY._13 = -sin_y;
	matRotY._31 = sin_y;
	matRotY._33 = cos_y;

	matTemp2 = matRotX * matRotY;
	mOut     = matTemp2 * matTemp;

	return mOut;
}

//============================================================================
//   �}�g���b�N�X����p�x�����߂�
//    
//   �p�x�����߂���@�́A���}�g���b�N�X�̂y�����ɂP���i�߂��ʒu���A���ʒu���猩��
//   �w�x�y�̊e�����ɉ��x����Ă��邩�Ŕ��肷��
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//   �߂�l
//       VECTOR3 vOut
//============================================================================
VECTOR3 GetRotateVector(const MATRIX4X4& mat)
{
	MATRIX4X4	matObj, matObj2, matTemp;
	VECTOR3	vObj, vBase;
	VECTOR2 vP1, vP2;
	VECTOR3 vOut;

	matObj = mat;
	matObj._41 = 0.0f;
	matObj._42 = 0.0f;
	matObj._43 = 0.0f;

	matTemp = XMMatrixTranslation(0.0f, 0.0f, 1.0f),
	matObj2 = matTemp * matObj;


	// �p�x�̂w���������߂�
	vBase.x = 0.0f;
	vBase.y = 0.0f;
	vBase.z = 1.0f;
	vObj.x = 0.0f;
	vObj.y = matObj2._42;
	vObj.z = matObj2._43;
	vOut.x = Angle(vBase, vObj) * 180 / 3.14159f;
	vP1.x = vBase.y;
	vP1.y = vBase.z;
	vP2.x = vObj.y;
	vP2.y = vObj.z;
	if (crossZ(vP1, vP2) < 0.0f) vOut.x *= -1.0f;

	// �p�x�̂x���������߂�
	vBase.x = 0.0f;
	vBase.y = 0.0f;
	vBase.z = 1.0f;
	vObj.x = matObj2._41;
	vObj.y = 0.0f;
	vObj.z = matObj2._43;
	vOut.y = Angle(vBase, vObj) * 180 / 3.14159f;
	vP1.x = vBase.x;
	vP1.y = vBase.z;
	vP2.x = vObj.x;
	vP2.y = vObj.z;
	if (crossZ(vP1, vP2) >= 0.0f) vOut.y *= -1.0f;

	// �p�x�̂y���������߂�
	matTemp = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	matObj2 = matTemp * matObj;

	vBase.x = 0.0f;
	vBase.y = 1.0f;
	vBase.z = 0.0f;
	vObj.x = matObj2._41;
	vObj.y = matObj2._42;
	vObj.z = 0.0f;
	vOut.z = Angle(vBase, vObj) * 180 / 3.14159f;
	vP1.x = vBase.x;
	vP1.y = vBase.y;
	vP2.x = vObj.x;
	vP2.y = vObj.y;
	if (crossZ(vP1, vP2) < 0.0f) vOut.z *= -1.0f;

	return vOut;
}

//============================================================================
//   �}�g���b�N�X����p�x�����߂�
//    
//   �p�x�����߂���@�́A���}�g���b�N�X�̂y�����ɂP���i�߂��ʒu���A���ʒu���猩��
//   �w�x�y�̊e�����ɉ��x����Ă��邩�Ŕ��肷��
//    
//   const MATRIX4X4& mNow	�}�g���b�N�X
//   �߂�l
//   VECTOR3 angle
//============================================================================
VECTOR3 GetRotateVector2(const MATRIX4X4& mNow)
{
	VECTOR3     angle;
	MATRIX4X4	mNext;
	VECTOR2     vec2, w;
	VECTOR3     vec3;

	mNext = XMMatrixTranslation(0.0f, 0.0f, 1.0f) * mNow;
	vec3 = GetPositionVector(mNext) - GetPositionVector(mNow);

	// Y��]
	vec2 = VECTOR2(vec3.x, vec3.z);
	w = XMVector2AngleBetweenVectors(vec2, VECTOR2(0,1));
	angle.y = w.x;

	// X��]
	vec2 = VECTOR2(vec3.y, vec3.z);
	w = XMVector2AngleBetweenVectors(vec2, VECTOR2(0,1));
	angle.x = w.y;

	// Z��]
	mNext = XMMatrixTranslation(0.0f, 1.0f, 0.0f) * mNow;
	vec3 = GetPositionVector(mNext) - GetPositionVector(mNow);
	vec2 = VECTOR2(vec3.x, vec3.y);
	w = XMVector2AngleBetweenVectors(vec2, VECTOR2(0,1));
	angle.z = w.x;

	angle.x = angle.x * 180.0f / XM_PI;
	angle.y = angle.y * 180.0f / XM_PI;
	angle.z = angle.z * 180.0f / XM_PI;

	return angle;
}

//============================================================================
//   �}�g���b�N�X����p�x�����߂�							// -- 2019.7.19
//    
//    �p�x�����߂���@�́A�}�g���b�N�X���̊p�x�����݂̂���肾���Ċp�x��
//    ���߂Ă���
//    �x��]�}�g���b�N�X * �w��]�}�g���b�N�X * �y��]�}�g���b�N�X�̏ꍇ�́A
//    ���m�Ɋe���̊p�x�����܂�
//    
//   ����
//   const MATRIX4X4& mWorld	�}�g���b�N�X
//   �߂�l
//   VECTOR3 angle
//============================================================================
VECTOR3 GetRotateVector3(const MATRIX4X4& mWorld)
{
	VECTOR3     angle;
	double      threshold = 0.001;

	if (fabsf(mWorld._23 - 1.0f) < threshold)  // Mat(2,3) sin(x)=1�̂Ƃ�
	{
		angle.x = XM_PI / 2;
		angle.y = 0;
		angle.z = atan2f(mWorld._12, mWorld._11);
	}
	else if (fabsf(mWorld._23 + 1.0f) < threshold)  // Mat(2,3) sin(x)=-1�̂Ƃ�
	{
		angle.x = -XM_PI / 2;
		angle.y = 0;
		angle.z = atan2f(mWorld._12, mWorld._11);
	}
	else {
		angle.x = asinf(mWorld._23);
		angle.y = atan2f(-mWorld._13, mWorld._33);
		angle.z = atan2f(-mWorld._21, mWorld._22);
	}

	// ���W�A���p��x�ɕϊ�
	angle.x = XMConvertToDegrees(angle.x);
	angle.y = XMConvertToDegrees(angle.y);
	angle.z = XMConvertToDegrees(angle.z);

	return angle;
}

//============================================================================
//   �}�g���b�N�X����p�x�w�̍s������߂�
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//   �߂�l
//       MATRIX4X4 mOut
//============================================================================
MATRIX4X4 GetRotateMatrixX(const MATRIX4X4& mat)
{
	VECTOR3 vRot = GetRotateVector(mat);
	MATRIX4X4 mOut;
	mOut = XMMatrixRotationX(XMConvertToRadians(vRot.x));

	return mOut;
}
//============================================================================
//   �}�g���b�N�X����p�x�x�̍s������߂�
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//   �߂�l
//       MATRIX4X4 mOut
//============================================================================
MATRIX4X4 GetRotateMatrixY(const MATRIX4X4& mat)
{
	VECTOR3 vRot = GetRotateVector(mat);
	MATRIX4X4 mOut;
	mOut = XMMatrixRotationY(XMConvertToRadians(vRot.y));

	return mOut;
}
//============================================================================
//   �}�g���b�N�X����p�x�y�̍s������߂�
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//   �߂�l
//       MATRIX4X4 mOut
//============================================================================
MATRIX4X4 GetRotateMatrixZ(const MATRIX4X4& mat)
{
	VECTOR3 vRot = GetRotateVector(mat);
	MATRIX4X4 mOut;
	mOut = XMMatrixRotationZ(XMConvertToRadians(vRot.z));

	return mOut;
}
//============================================================================
//   �}�g���b�N�X����p�x�̍s������߂�
//    
//    �E�ʒu��������菜��
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//   �߂�l
//       MATRIX4X4 mOut
//============================================================================
MATRIX4X4 GetRotateMatrix(const MATRIX4X4& mat)
{
	MATRIX4X4 mOut;
	mOut = mat * XMMatrixTranslation(-mat._41, -mat._42, -mat._43);
	return mOut;
}
//============================================================================
//   �}�g���b�N�X����ʒu�����߂�
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//    
//   �߂�l
//       VECTOR3
//============================================================================
VECTOR3 GetPositionVector(const MATRIX4X4& mat)
{
	return VECTOR3(mat._41, mat._42, mat._43);
}
//============================================================================
//   �}�g���b�N�X����ʒu�s������߂�
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//    
//   �߂�l
//       D3DMATRIX  mOut
//============================================================================
MATRIX4X4 GetPositionMatrix(const MATRIX4X4& mat)
{
	MATRIX4X4 mOut;
	mOut = XMMatrixTranslation(mat._41, mat._42, mat._43);

	return mOut;
}

//============================================================================
//   ���}�g���b�N�X����^�[�Q�b�g���������߂̊p�x�����߂�               // -- 2018.8.16
//    
//   �����̃}�g���b�N�X�ƃ^�[�Q�b�g�ʒu�̃x�N�g������͂�
//   �������������Ă���p�x�ƃ^�[�Q�b�g�����Ƃ̊p�x�̍���������
//   ���̒l���O�`�R�U�O���̊p�x�ŕԂ�
//    
//   ����
//       const MATRIX4X4&  mWorld	  �����̃}�g���b�N�X
//       const VECTOR3&    vTarget  �^�[�Q�b�g�Ƃ���ʒu
//   �߂�l
//       VECTOR3    vOut     �^�[�Q�b�g���������߂̂O�`�R�U�O���̊p�x
//============================================================================
VECTOR3 GetTargetRotateVector(const MATRIX4X4& mWorld, const VECTOR3& vTarget)
{
	MATRIX4X4  mTemp;
	VECTOR3    vRa, vRb, vOut;

	mTemp = GetLookatMatrix(GetPositionVector(mWorld), vTarget);	// �^�[�Q�b�g�����������}�g���b�N�X
	vRa = GetRotateVector(mWorld);		// �����̌��݂̌���
	vRb = GetRotateVector(mTemp);		// �������猩���^�[�Q�b�g�̕���
	vOut = vRb - vRa;					// �������^�[�Q�b�g���������߂̊p�x

	// �쐬�����p�x�̐��K��
	// �O�̂��߂Ɋp�x���O�`�R�U�O���ɐ��K������
	if (vOut.x <  0)   vOut.x += 360;
	if (vOut.x >= 360) vOut.x -= 360;
	if (vOut.y <  0)   vOut.y += 360;
	if (vOut.y >= 360) vOut.y -= 360;
	if (vOut.z <  0)   vOut.z += 360;
	if (vOut.z >= 360) vOut.z -= 360;

	return vOut;
}

//============================================================================  // -- 2019.12.30
//   �}�g���b�N�X����g��k���������߂�
//    
//   const MATRIX4X4& mat	�}�g���b�N�X
//    
//   �߂�l
//       VECTOR3  vScale : ���݂̊g��k���� sx,sy,sz
//============================================================================
VECTOR3 GetScaleVector(const MATRIX4X4& mat)
{
	VECTOR3    vScale;

	vScale.x = sqrtf(mat._11*mat._11 + mat._21*mat._21 + mat._31*mat._31);
	vScale.y = sqrtf(mat._12*mat._12 + mat._22*mat._22 + mat._32*mat._32);
	vScale.z = sqrtf(mat._13*mat._13 + mat._23*mat._23 + mat._33*mat._33);

	return vScale;
}


//------------------------------------------------------------------------------
//
//�@�Q�����E�R�����x�N�g���p�ėp���C�u����
//
//�@�Q�����E�R�����x�N�g���p�̊e��ėp���C�u������ݒ肷��
//�@���l�̓���������w�l���C�u�����Ȃǂ����邪�A�����ł͑S�Ă����삷��
//
//------------------------------------------------------------------------------
//
// �l���O�̎��́A�e���������̍ŏ��l�ɂ���i�O�������j
//
FLOAT avoidZero(FLOAT inp)
{
	if (inp == 0.0f) {
		inp = FLT_EPSILON;
	}
	return inp;
}

//
// �Q�̂Q�c�x�N�g���̓��ς��v�Z����
//
FLOAT dot(const VECTOR2& vLhs, const VECTOR2& vRhs)
{
	return (vLhs.x*vRhs.x + vLhs.y*vRhs.y);
}
//
// �Q�̂R�c�x�N�g���̓��ς��v�Z����
//
FLOAT dot(const VECTOR3& vLhs, const VECTOR3& vRhs)
{
	return (vLhs.x*vRhs.x + vLhs.y*vRhs.y + vLhs.z*vRhs.z);
}
//
// �Q�̂R�c�x�N�g���̊O�ς��v�Z����
//
VECTOR3 cross(const VECTOR3& vLhs, const VECTOR3& vRhs)
{
	return VECTOR3(vLhs.y*vRhs.z - vLhs.z*vRhs.y,
		vLhs.z*vRhs.x - vLhs.x*vRhs.z,
		vLhs.x*vRhs.y - vLhs.y*vRhs.x);
}
//
// �Q�̂Q�c�x�N�g���̊O�ς��v�Z���Az�v�f��Ԃ�
// (z�v�f�̒l�����̏ꍇ�A�x�N�g��vRhs�̓x�N�g��vLhs���甽���v���ł���B)
//
FLOAT crossZ(const VECTOR2& vLhs, const VECTOR2& vRhs)
{
	return vLhs.x*vRhs.y - vLhs.y*vRhs.x;
}

//
// �Q�c�x�N�g���̒��������߂�
//
FLOAT magnitude(const VECTOR2& vLen)
{
	return sqrtf(vLen.x*vLen.x + vLen.y*vLen.y);
}

//
// �R�c�x�N�g���̒��������߂�
//
FLOAT magnitude(const VECTOR3& vLen)
{
	return sqrtf(vLen.x*vLen.x + vLen.y*vLen.y + vLen.z*vLen.z);
}

//
// �R�c�x�N�g���̒����̂Q������߂�
//
FLOAT magnitudeSQ(const VECTOR3& vLen)
{
	return vLen.x*vLen.x + vLen.y*vLen.y + vLen.z*vLen.z;
}
//
// �Q�c�x�N�g�����X�P�[�����O�����l�����߂�
//
VECTOR2 scaling(const VECTOR2& vVec, const FLOAT& Scale)
{
	return VECTOR2(vVec.x*Scale, vVec.y*Scale);
}
//
// �R�c�x�N�g�����X�P�[�����O�����l�����߂�
//
VECTOR3 scaling(const VECTOR3& vVec, const FLOAT& Scale)
{
	return VECTOR3(vVec.x*Scale, vVec.y*Scale, vVec.z*Scale);
}

//
// �Q�c�x�N�g���̐��K�������x�N�g�������߂�
//
VECTOR2 normalize(const VECTOR2& vVec)
{
	return scaling(vVec, 1.0f / avoidZero(magnitude(vVec)));
}
//
// �R�c�x�N�g���̐��K�������x�N�g�������߂�
//
VECTOR3 normalize(const VECTOR3& vVec)
{
	return scaling(vVec, 1.0f / avoidZero(magnitude(vVec)));
}

//
// �Q�̂R�c�x�N�g���̊p�x���v�Z����
//
FLOAT Angle(const VECTOR3& v1, const VECTOR3& v2)
{
	return acosf(dot(v1, v2) / avoidZero(magnitude(v1)*magnitude(v2)));
}

//
// �R�̂R�c�x�N�g���̊p�x�̘a���v�Z����
// �i���̊֐��̖߂�l���R�U�O���i�Q�΁j�̎��́A���_���R�̂R�c�x�N�g���̒��ɂ���j
//
FLOAT AddAngle(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3)
{
	FLOAT p12m, p23m, p31m;
	FLOAT w1, w2, w3;

	// �ڐG�_�����_�ɔ��ɋ߂��Ƃ��i�P�����ȉ��j�́A�R�c�x�N�g���̒��Ƃ���  2005.10.1
	if (((p1.x <= 0.001f && p1.x >= -0.001f) &&
		(p1.y <= 0.001f && p1.y >= -0.001f) &&
		(p1.z <= 0.001f && p1.z >= -0.001f)) ||
		((p2.x <= 0.001f && p2.x >= -0.001f) &&
		(p2.y <= 0.001f && p2.y >= -0.001f) &&
			(p2.z <= 0.001f && p2.z >= -0.001f)) ||
			((p3.x <= 0.001f && p3.x >= -0.001f) &&
		(p3.y <= 0.001f && p3.y >= -0.001f) &&
				(p3.z <= 0.001f && p3.z >= -0.001f))) {
		return (2.0f * 3.14159f);
	}

	{
		FLOAT p1m = magnitude(p1),	// �R�c�x�N�g�����P�̐�Βl�i�����j
			p2m = magnitude(p2),	// �R�c�x�N�g�����Q�̐�Βl�i�����j
			p3m = magnitude(p3);	// �R�c�x�N�g�����R�̐�Βl�i�����j
		p12m = p1m * p2m;
		p23m = p2m * p3m;
		p31m = p3m * p1m;
	}

	// �����@2005.9.25
	//return acosf(dot(p1, p2) / avoidZero(p12m)) +		// �R�c�x�N�g�����P�Ƃ��Q�̊p�x
	//       acosf(dot(p2, p3) / avoidZero(p23m)) +		// �R�c�x�N�g�����Q�Ƃ��R�̊p�x
	//       acosf(dot(p3, p1) / avoidZero(p31m));		// �R�c�x�N�g�����R�Ƃ��P�̊p�x

	w1 = dot(p1, p2) / avoidZero(p12m);		// �R�c�x�N�g�����P�Ƃ��Q�̂������l
	if (w1 < -1.0f) w1 = -1.0f;
	if (w1 >  1.0f) w1 = 1.0f;

	w2 = dot(p2, p3) / avoidZero(p23m);		// �R�c�x�N�g�����Q�Ƃ��R�̂������l
	if (w2 < -1.0f) w2 = -1.0f;
	if (w2 >  1.0f) w2 = 1.0f;

	w3 = dot(p3, p1) / avoidZero(p31m);		// �R�c�x�N�g�����R�Ƃ��P�̂������l
	if (w3 < -1.0f) w3 = -1.0f;
	if (w3 >  1.0f) w3 = 1.0f;

	return (acosf(w1) + acosf(w2) + acosf(w3));		// �R�c�x�N�g���̊p�x�̘a

}
//
// �S�̂R�c�x�N�g���̊p�x�̘a���v�Z����
//
FLOAT AddAngle(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3, const VECTOR3& p4)
{
	FLOAT p12m, p23m, p34m, p41m;
	FLOAT w1, w2, w3, w4;

	// �ڐG�_�����_�ɔ��ɋ߂��Ƃ��i�P�����ȉ��j�́A�S�c�x�N�g���̒��Ƃ���  2005.10.1
	if (((p1.x <= 0.001f && p1.x >= -0.001f) &&
		(p1.y <= 0.001f && p1.y >= -0.001f) &&
		(p1.z <= 0.001f && p1.z >= -0.001f)) ||
		((p2.x <= 0.001f && p2.x >= -0.001f) &&
		(p2.y <= 0.001f && p2.y >= -0.001f) &&
			(p2.z <= 0.001f && p2.z >= -0.001f)) ||
			((p3.x <= 0.001f && p3.x >= -0.001f) &&
		(p3.y <= 0.001f && p3.y >= -0.001f) &&
				(p3.z <= 0.001f && p3.z >= -0.001f)) ||
				((p4.x <= 0.001f && p4.x >= -0.001f) &&
		(p4.y <= 0.001f && p4.y >= -0.001f) &&
					(p4.z <= 0.001f && p4.z >= -0.001f))) {
		return (2.0f * 3.14159f);
	}
	{
		FLOAT p1m = magnitude(p1),
			p2m = magnitude(p2),
			p3m = magnitude(p3),
			p4m = magnitude(p4);
		p12m = p1m * p2m;
		p23m = p2m * p3m;
		p34m = p3m * p4m;
		p41m = p4m * p1m;
	}
	// �����@2005.9.25
	//return acosf(dot(p1, p2) / avoidZero(p12m)) +
	//       acosf(dot(p2, p3) / avoidZero(p23m)) +
	//       acosf(dot(p3, p4) / avoidZero(p34m)) +
	//       acosf(dot(p4, p1) / avoidZero(p41m));

	w1 = dot(p1, p2) / avoidZero(p12m);		// �S�c�x�N�g�����P�Ƃ��Q�̂������l
	if (w1 < -1.0f) w1 = -1.0f;
	if (w1 >  1.0f) w1 = 1.0f;

	w2 = dot(p2, p3) / avoidZero(p23m);		// �S�c�x�N�g�����Q�Ƃ��R�̂������l
	if (w2 < -1.0f) w2 = -1.0f;
	if (w2 >  1.0f) w2 = 1.0f;

	w3 = dot(p3, p4) / avoidZero(p34m);		// �S�c�x�N�g�����R�Ƃ��S�̂������l
	if (w3 < -1.0f) w3 = -1.0f;
	if (w3 >  1.0f) w3 = 1.0f;

	w4 = dot(p4, p1) / avoidZero(p41m);		// �S�c�x�N�g�����S�Ƃ��P�̂������l
	if (w4 < -1.0f) w4 = -1.0f;
	if (w4 >  1.0f) w4 = 1.0f;

	return (acosf(w1) + acosf(w2) + acosf(w3) + acosf(w4));		// �S�c�x�N�g���̊p�x�̘a}
}

//-----------------------------------------------------------------------------
//  �����̔���
//
//  int  min1  ��������l
//  int  max1  ���������l
//
//  �߂�l�@�@int  ����
//-----------------------------------------------------------------------------
int Random(int min1, int max1)
{
	static unsigned int cnt = 0;
	int range;

	if (cnt == 0) {
		srand((unsigned int)timeGetTime());
		cnt = 1;
	}
	range = max1 - min1 + 1;
	return((rand() % range) + min1);
}
