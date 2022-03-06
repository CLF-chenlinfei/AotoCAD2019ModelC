#include "StdAfx.h"
#include "alg.h"
#include "name.h"


// ���xy ���淽�� �����һ����ϱ�׼
bool dec_xy(EntBox &p1)
{
	// ���xy ���淽�� �����һ����ϱ�׼
	double xx = p1.maxp.x - p1.minp.x;
	double yy = p1.maxp.y - p1.minp.y;
	if (xx < 30 || yy < 30) return true;
	return false;
}
 //���������
bool DoubleEq(double n1, double n2, double chaz)
{
	if (abs(n1 - n2) < chaz) return true;
	return false;
}
// �����е�
AcGePoint3d LineCenter(const AcGePoint3d &p1, const AcGePoint3d &p2)
{
	double x = (p1.x + p2.x) / 2;
	double y = (p1.y + p2.y) / 2;
	double z = (p1.z + p2.z) / 2;
	return AcGePoint3d(x, y, z);
}
// ��λ���
bool EquaPoint(const AcGePoint3d &p1, const AcGePoint3d &p2)
{
	if (DoubleEq(p1.x, p2.x) &&
		DoubleEq(p1.y, p2.y) &&
		DoubleEq(p1.z, p2.z)) return true;
	return false;

}

// �������Ƚ� ǰ�ߴ󷵻�ture
bool Doublebj(double n1, double n2)
{
	if (n1 > n2) return true;
	return false;
}

// ���ߴ���ĳ��
bool bigfun2c(EntBox &p1, int wd)
{
	// ���ߴ���ĳ��
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y || x && z || y && z)return true;
	return false;

}

// ���ߴ���ĳ��
bool bigfun3c(EntBox &p1, int wd)
{
	// ���ߴ���ĳ��
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y&&z)return true;
	return false;

}

// 1�ߴ���ĳ��
bool bigfun1c(EntBox &p1, int wd)
{
	// ���ߴ���ĳ��
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x || y || z)return true;
	return false;

}

// ���Ű�� ����60*299 �İ��
bool gsst(EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && 
		p1.Layer != Layer_door && p1.Type == Solid)return true;
	return false;

}
// �����Ű� ����60*299 �İ��
bool gsdoor(EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && p1.Type == Solid)return true;
	return false;
}
// ���ڸ�������Ű��ж�
// �����Ű� ����60*299 �İ��
bool isdoor(EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer == Layer_door && p1.Type == Solid)return true;
	return false;
}
// ����ʵ��
EntBox ExspansionEnt(EntBox &p1, double greater)
{
	// ע�����δ���ͣ������ô�С�����
	EntBox newdata;
	newdata.maxp = AcGePoint3d(p1.maxp.x + greater, p1.maxp.y + greater, p1.maxp.z + greater);
	newdata.minp = AcGePoint3d(p1.minp.x - greater, p1.minp.y - greater, p1.minp.z - greater);
	newdata.center = p1.center;
	newdata.id = p1.id;
	newdata.Layer = p1.Layer;
	newdata.Type = p1.Type;
	newdata.volume = p1.volume;
	return newdata;
}

// �������ཻ
bool MandM(recmo &m1, recmo &m2, double pz)
{
	// �������ĵ�֮��ľ��� һ��С�ڵ��������б߳��͵�һ��
	// x���ľ���
	// ���ľ���Ϊ �߾�����ĵ� ��ֵ
	// ���߾�͵�һ��
	// ���ľ��� ���Ǿ͸�Զ pz С��0 ��ʴ ����0 ����
	double zx = abs(m1.x1 + m1.x11 - m2.x1 - m2.x11);
	double zy = abs(m1.y2 + m1.y22 - m2.y2 - m2.y22);
	// �߾����
	double rx = abs(m1.x1 - m1.x11) + abs(m2.x1 - m2.x11);
	double ry = abs(m1.y2 - m1.y22) + abs(m2.y2 - m2.y22);
	if (zx <= rx + pz && zy <= ry + pz)
		return true;
	else
		return false;
}
//
//// �����ཻ2 �����ȫ���
bool BoxIntersectBox2(EntBox &b1, EntBox &b2, double pz)
{
	// ������������p1
	recmo p1xy = { b1.minp.x, b1.minp.y, b1.maxp.x, b1.maxp.y };
	recmo p2xy = { b2.minp.x, b2.minp.y, b2.maxp.x, b2.maxp.y };
	// zy
	recmo p1zy = { b1.minp.z, b1.minp.y, b1.maxp.z, b1.maxp.y };
	recmo p2zy = { b2.minp.z, b2.minp.y, b2.maxp.z, b2.maxp.y };
	// zx
	recmo p1zx = { b1.minp.z, b1.minp.x, b1.maxp.z, b1.maxp.x };
	recmo p2zx = { b2.minp.z, b2.minp.x, b2.maxp.z, b2.maxp.x };

	if (MandM(p1xy, p2xy, pz) && 
		MandM(p1zx, p2zx, pz) && 
		MandM(p1zy, p2zy, pz))return true;
	return false;
	// ������������Ƿ��ཻ
}
//
bool pointdpoint(AcGePoint3d p1, AcGePoint3d p2)
{
	bool x = DoubleEq(p1.x, p2.x);
	bool y = DoubleEq(p1.y, p2.y);
	bool z = DoubleEq(p1.z, p2.z);
	if (x&&y&&z) return true;
	return false;
}
//
// �жϵ�λ�ص�
bool judeMore(EntBox &p1, EntBox &p2)
{
	// �ж�p1,p2 �ǿ�λ
	bool p1isHole = isHole(p1);
	bool p2isHole = isHole(p2);

	// �����ж������ĵ� ��� ͼ�� ͬʱ���Ϊ���
	if (pointdpoint(p1.center, p2.center) &&
		p1.id != p2.id&&
		p1.Layer == p2.Layer&&
		abs(p1.volume - p2.volume) < 0.1&&p1isHole&&p2isHole)
		return true;
	return false;
}
//// �ǵƲ�
bool isLamp(EntBox &p1)
{

	if (p1.Layer == Layer_wjls && p1.Type == Solid)
	{
		double p1Height = p1.maxp.z - p1.minp.z;
		double p1Depth = p1.maxp.y - p1.minp.y;
		double p1Width = p1.maxp.x - p1.minp.x;
		// Ӧ�÷�4�����
		if (abs(p1Height - 8.5) < 1E-6&&abs(p1Depth - 20) < 1E-6)return true;
		// x��ƽ��
		// y��ƽ��
		if (abs(p1Height - 8.5) < 1E-6&&abs(p1Width - 20) < 1E-6)return true;
		// z��ƽ��
		if (abs(p1Width - 8.5) < 1E-6&&abs(p1Depth - 20) < 1E-6)return true;
		// z������
		if (abs(p1Width - 20) < 1E-6&&abs(p1Depth - 8.5) < 1E-6)return true;
		return false;
	}
	return false;
}
//// �ǿ�λ
bool isHole(EntBox &p1)
{


	if (p1.Layer != Layer_door && p1.Type == Solid && (abs(p1.volume - 1856) < 1 || abs(p1.volume - 300) < 1)) return true;
	return false;

}
// ����ཻ
bool Intervene(EntBox &p1, EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -1.0) && gsst(p1) && gsst(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// �ţ�����ཻ
bool DoorBIntervene(EntBox &p1, EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -6.0) && gsdoor(p1) && gsdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// �Ű��ཻ
bool DoorIntervene(EntBox &p1, EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, 3.0) && isdoor(p1) && isdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id
		&& dec_xy(p1) && dec_xy(p2))
		return true;
	return false;
}