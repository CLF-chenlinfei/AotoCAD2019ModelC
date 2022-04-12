#include "StdAfx.h"
#include "alg.h"
#include "name.h"
#include <vector>
#include <tchar.h>
#include <algorithm>
#include "CreateSome.h"

#include "brbrep.h"

#include "brbftrav.h"

#include "brbetrav.h"

#include "brface.h"

#include "bredge.h"



// �����
bool maxbord(const EntBox &p1)
{
	double x = abs(p1.maxp.x - p1.minp.x) < 2800;
	double y = abs(p1.maxp.y - p1.minp.y) < 2800;
	double z = abs(p1.maxp.z - p1.minp.z) < 2800;
	if (x&&y&&z) return true;
	return false;
}

// �����ཻ2 �����ȫ���
bool BoxIntersectBox(const EntBox &b1, const EntBox &b2, double pz)
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

	if (MandM(p1xy, p2xy, pz) && MandM(p1zx, p2zx, pz) && MandM(p1zy, p2zy, pz))return true;
	return false;
	// ������������Ƿ��ཻ
}
//�����Χ������
bool PointBoxTangency(const AcGePoint3d &p1, const EntBox &box, double expand)
{
	//��λ�Ƿ����������
	//��������
	if (
		((p1.x >= box.minp.x&&p1.x <= box.maxp.x&&p1.z <= box.maxp.z&&p1.z >= box.minp.z) && (abs(p1.y - box.maxp.y) < expand || abs(p1.y - box.minp.y) < expand)) ||
		((p1.y >= box.minp.y&&p1.y <= box.maxp.y&&p1.z <= box.maxp.z&&p1.z >= box.minp.z) && (abs(p1.x - box.maxp.x) < expand || abs(p1.x - box.minp.x) < expand)) ||
		((p1.x >= box.minp.x&&p1.x <= box.maxp.x&&p1.y <= box.maxp.y&&p1.y >= box.minp.y) && (abs(p1.z - box.maxp.z) < expand || abs(p1.z - box.minp.z) < expand))
		)return true;
	return false;
}

// ��������
bool BoxTangencyBox(const EntBox &b1, const EntBox &b2)
{
	bool p1b = false;
	bool p2b = false;
	bool p3b = false;
	bool p4b = false;

	if (PointBoxTangency(b1.minp, b2))
	{
		p1b = true;
	}
	if (PointBoxTangency(b1.maxp, b2))
	{
		p2b = true;
	}
	if (PointBoxTangency(b2.minp, b1))
	{
		p3b = true;
	}
	if (PointBoxTangency(b2.maxp, b1))
	{
		p4b = true;
	}

	if (p1b || p2b || p3b || p4b) return true;
	return false;
}
// ָ������2
bool FiltrationEnt(const EntBox &p1, int d1, int d2)
{
	// ���߾���ӽ� С��0.1 ����ture else 0
	double s1 = abs(p1.maxp.x - p1.minp.x);
	double s2 = abs(p1.maxp.y - p1.minp.y);

	if ((abs(s1 - d1) < 0.1&& abs(s2 - d2) < 0.1)
		|| (abs(s2 - d1) < 0.1&& abs(s1 - d2) < 0.1))
		return true;
	return false;


}
//���ڰ�Χ����
bool PointInBoxTangency(const AcGePoint3d &p1, const EntBox &box)
{

	if (
		box.minp.x < p1.x&&p1.x < box.maxp.x&&
		box.minp.y < p1.y&&p1.y < box.maxp.y&&
		box.minp.z < p1.z&&p1.z < box.maxp.z
		)return true;
	return false;

}
// ����������
bool PointInRec(double x, double y, recmo rec)
{
	if (rec.x1<x&&rec.x11>x&&rec.y2<y&&rec.y22>y) return true;
	return false;
}
// ���xy ���淽�� �����һ����ϱ�׼
bool dec_xy(const EntBox &p1)
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
bool bigfun2c(const EntBox &p1, int wd)
{
	// ���ߴ���ĳ��
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y || x && z || y && z)return true;
	return false;

}
// �����ֿ�λ
bool isLskwf(const EntBox &p1)
{
	
	if ((
		p1.volume == 450 ||
		p1.volume == 500 ||
		p1.volume == 300) &&
		p1.Type == Solid &&
		p1.Layer == Layer_door) return true;
	return false;
}
// ���ߴ���ĳ��
bool bigfun3c(const EntBox &p1, int wd)
{
	// ���ߴ���ĳ��
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y&&z)return true;
	return false;

}

// ����һ�ж�
int isShy(const EntBox &p1)
{
	if (round(p1.volume) == 1856)
	{
		if (round(p1.maxp.z - p1.minp.z) == 29)
		{
			// 1 Ϊ��
			return 1;
		}
		if (round(p1.maxp.x - p1.minp.x) == 29)
		{
			// 1 Ϊ��
			return 2;
		}
		if (round(p1.maxp.y - p1.minp.y) == 29)
		{
			// 1 Ϊ��
			return 3;
		}
		// ����
		return 0;
	}
	// ������
	return 0;
}

// 1�ߴ���ĳ��
bool bigfun1c(const EntBox &p1, int wd)
{
	// ���ߴ���ĳ��
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x || y || z)return true;
	return false;

}

// ���Ű�� ����60*299 �İ��
bool gsst(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 249);
	bool a2 = bigfun2c(p1, 47);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && 
		p1.Layer != Layer_door && p1.Type == Solid)return true;
	return false;
}

// ���Ű�� ����60*299 �İ��
bool gsst11(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 180);
	bool a2 = bigfun2c(p1, 47);
	bool a3 = bigfun3c(p1, 8);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls &&
		p1.Layer != Layer_door && p1.Type == Solid)return true;
	return false;
}
// �����Ű� ����60*299 �İ��
bool gsdoor(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && p1.Type == Solid)return true;
	return false;
}
// ���ڸ�������Ű��ж�
// �����Ű� ����60*299 �İ��
bool isdoor(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer == Layer_door && p1.Type == Solid)return true;
	return false;
}
// ����ʵ��
EntBox ExspansionEnt(const EntBox &p1, double greater)
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
bool BoxIntersectBox2(const EntBox &b1, const EntBox &b2, double pz)
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
bool pointdpoint(const AcGePoint3d p1, const AcGePoint3d p2)
{
	bool x = DoubleEq(p1.x, p2.x);
	bool y = DoubleEq(p1.y, p2.y);
	bool z = DoubleEq(p1.z, p2.z);
	if (x&&y&&z) return true;
	return false;
}
//
// �жϵ�λ�ص�
bool judeMore(const EntBox &p1, const EntBox &p2)
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
bool isLamp(const EntBox &p1)
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
bool isHole(const EntBox &p1)
{


	if (p1.Layer != Layer_door && 
		p1.Type == Solid && (abs(p1.volume - 1856) < 1 || 
			abs(p1.volume - 300) < 1)) return true;
	return false;

}
// ����ཻ
bool Intervene(const EntBox &p1, const EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -1.0) && gsst(p1) && gsst(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// �ţ�����ཻ
bool DoorBIntervene(const EntBox &p1, const EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -6.0) && gsdoor(p1) && gsdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// �Ű��ཻ
bool DoorIntervene(const EntBox &p1, const EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, 3.0) && isdoor(p1) && isdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id
		&& dec_xy(p1) && dec_xy(p2))
		return true;
	return false;
}
// �౳��
bool isbb(const EntBox &p1)
{
	bool a1 = bigfun1c(p1, 399);
	bool a2 = bigfun2c(p1, 200);
	bool a3 = bigfun3c(p1, 8);
	if (a1&&a2&&a3)return true;
	return false;

}
// ��鷽�� 1 y ���� 2 x ����
int decXYZ(const EntBox &p1)
{
	if (round(p1.maxp.x - p1.minp.x) == 18 || round(p1.maxp.x - p1.minp.x) == 9)return 1;
	if (round(p1.maxp.y - p1.minp.y) == 18 || round(p1.maxp.y - p1.minp.y) == 9)return 2;
	if (round(p1.maxp.z - p1.minp.z) == 18 || round(p1.maxp.z - p1.minp.z) == 9)return 3;
	return 0;
}

// 0 x ���� 1 Y����
// ����2 ���� ������
int dec01(const EntBox &p1)
{
	if (p1.maxp.x - p1.minp.x < p1.maxp.y - p1.minp.y) return 1;
	if (p1.maxp.z - p1.minp.z < p1.maxp.x - p1.minp.x&&
		p1.maxp.z - p1.minp.z < p1.maxp.y - p1.minp.y&&
		p1.maxp.z - p1.minp.z < 30
		)
	{
		return 2;
	}
	return 0;
	
}

// �������
bool MinM(const recmo &m1, const recmo &m2, double pz)
{
	// ���� С�� ���� �Ҳ������Ҳ�� ��������ͬʱ����
	// �����ж��� ������
	// ��m1 ����m2 ǰ��һ���� ����
	if (m1.x1 < m1.x11&&m2.x1 < m2.x11)
	{
		// 1�������
		if (m1.x1 <= m2.x1&&
			m1.y2 <= m2.y2&&
			m1.x11 >= m2.x11&&
			m1.y22 >= m2.y22)
			return true;
		return false;

	}
	return false;
}

void CLs(const EntBox &p1, const EntBox &p2, int fx,int sx, const ACHAR* text)
{
	AcGePoint3d center = AcGePoint3d((p1.maxp.x + p1.minp.x) / 2,
		(p1.maxp.y + p1.minp.y) / 2, 
		(p1.maxp.z + p1.minp.z) / 2);
	AcGePoint3d center2 = AcGePoint3d((p2.maxp.x + p2.minp.x) / 2,
		(p2.maxp.y + p2.minp.y) / 2,
		(p2.maxp.z + p2.minp.z) / 2);
	CreateLine(center, p2.center, 90);
	CreateArrow(center, fx, sx, 120, 90);
	CreateArrow(center2, fx, sx, 120, 90);
	CText(LineCenter(center, center2), text, fx);

}

// �������� ����ƽ��X�� �ֱ���Z ��X ���
EntBox ExpandBack(const EntBox &p1, int xs, int more)
{
	EntBox pp;
	pp = p1;
	if (xs==2)
	{
		pp.maxp = AcGePoint3d(p1.maxp.x+more, p1.maxp.y, p1.maxp.z+more);
		pp.minp = AcGePoint3d(p1.minp.x-more, p1.minp.y, p1.minp.z-more);
	}
	else
	{
		pp.maxp = AcGePoint3d(p1.maxp.x, p1.maxp.y+more, p1.maxp.z+more);
		pp.minp = AcGePoint3d(p1.minp.x, p1.minp.y-more, p1.minp.z-more);
	}
	return pp;
}

bool MoreBack(const EntBox &p1, const EntBox &p2, std::vector<EntBox> &adlist)
{
	// �ҵ����鱳���ص�
	//std::vector<EntBox> MydataR;
	if (isbb(p1) && isbb(p2))
	{
		//x ����18
		if (decXYZ(p1) == 1 && decXYZ(p2) == 1)
		{
			recmo reccc;
				reccc.x1 = p2.minp.y;
				reccc.y2 = p2.minp.z;
				reccc.x11 = p2.maxp.y;
				reccc.y22 = p2.maxp.z;
				if (PointInRec(p1.center.y, p1.center.z, reccc) && p1.id != p2.id&&
					abs(p1.center.x - p2.center.x) < 100)
				{
					std::vector<AcDbObjectId> ls;
					for (int ss = 0; ss < adlist.size(); ss++)
					{
						if ((BoxIntersectBox2(ExpandBack(p1, 1, 3), adlist[ss], -0.5) && gsst(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
							ls.push_back(adlist[ss].id);
						if ((BoxIntersectBox2(ExpandBack(p2, 1, 3), adlist[ss], -0.5) && gsst(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
							ls.push_back(adlist[ss].id);
					}
					std::sort(ls.begin(), ls.end());
					
					ls.erase(unique(ls.begin(), ls.end()), ls.end());
					
					if (ls.size() / 2 < 3)return true;
				}
		}
		// y ����18
		if (decXYZ(p1) == 2 && decXYZ(p2) == 2)
		{
			recmo reccc;
			reccc.x1 = p2.minp.x;
			reccc.y2 = p2.minp.z;
			reccc.x11 = p2.maxp.x;
			reccc.y22 = p2.maxp.z;
			if (PointInRec(p1.center.x, p1.center.z, reccc) && p1.id != p2.id&&
				abs(p1.center.y - p2.center.y) < 100)
			{
				std::vector<AcDbObjectId> ls;
				for (int ss = 0; ss < adlist.size(); ss++)
				{
					if ((BoxIntersectBox2(ExpandBack(p1, 2, 3), adlist[ss], -0.5) && gsst11(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
						ls.push_back(adlist[ss].id);
					if ((BoxIntersectBox2(ExpandBack(p2, 2, 3), adlist[ss], -0.5) && gsst11(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
						ls.push_back(adlist[ss].id);
				}

					std::sort(ls.begin(), ls.end());
					ls.erase(unique(ls.begin(), ls.end()), ls.end());
					//acutPrintf(_T("\n ������:%d"), ls.size());
					if (ls.size() / 2 < 3)return true;
			}

		}
	}
	
	return false;

}

// �����ͨλ��
int TestYt2(const EntBox &p1, const EntBox &p2)
{
	// �ж�p1 ����ͨ p2  ������ͨ
	/*bool p1yt = ((int)(p1.maxp.x - p1.minp.x) == 15 || (int)(p1.maxp.y - p1.minp.y) == 15) &&
		p1.Layer == Layer_wjls && (int)(p1.maxp.z - p1.minp.z) == 30;
	if (!p1yt)
		return 0;*/
	bool p2box = p2.id != p1.id&&gsst(p2);
	if (!p2box)
		return 0;
	AcGePoint3d yap;
	AcGePoint3d yip;
	bool a1 = false;
	bool a2 = false;
	// �ж��¸˷���
	if (p1.maxp.x - p1.minp.x > 100)
	{
		yap.x = p1.maxp.x + 7;
		yip.x = p1.minp.x - 7;
		yap.y = p1.maxp.y;
		yip.y = p1.minp.y;
	}
	else
	{
		yap.x = p1.maxp.x;
		yip.x = p1.minp.x;
		yap.y = p1.maxp.y + 7;
		yip.y = p1.minp.y - 7;
	}
	yap.z = p1.maxp.z;
	yip.z = p1.minp.z;
	if (PointInBoxTangency(yap, p2))return 1;// �жϴ��
	
	if (PointInBoxTangency(yip, p2))return 2;// �ж�С��
	return 0;
	
}
// �ж������Ž�
bool isMj(const EntBox &p1)
{
	if (((p1.volume > 16537 && p1.volume < 16538) || (p1.volume > 17149 && p1.volume < 17151))
		&& p1.Type == Solid &&
		((int)(p1.maxp.x - p1.minp.x - 0.1) == 13 || (int)(p1.maxp.y - p1.minp.y - 0.1) == 13 || (int)(p1.maxp.z - p1.minp.z - 0.1) == 13)
		)return true;
	return false;
}

TEntBox CreaterMJ(const EntBox &mj)
{
	TEntBox newMj;
	newMj.sx.id = mj.id;
	newMj.sx.Layer = mj.Layer;
	newMj.sx.Type = mj.Type;
	newMj.sx.center = mj.center;

	newMj.zy.id = mj.id;
	newMj.zy.Layer = mj.Layer;
	newMj.zy.Type = mj.Type;
	newMj.zy.center = mj.center;
	// �ж��Ƿ���X����Ž�
	if ((int)(mj.maxp.x - mj.minp.x - 0.1) != 13)
	{
		// ��������ʵ�� x�� y ֵ����� z�Ǹ߶�
		// 35 �����µĸ߶�
		//
		newMj.fx = 0;
		// ������
		newMj.sx.minp.z = mj.center.z - 35;
		newMj.sx.maxp.z = mj.center.z + 35;
		newMj.sx.minp.y = mj.center.y - 75;
		newMj.sx.maxp.y = mj.center.y + 75;
		newMj.sx.minp.x = mj.center.x - 4;
		newMj.sx.maxp.x = mj.center.x + 4;
		// ������
		newMj.zy.minp.z = mj.center.z - 4;
		newMj.zy.maxp.z = mj.center.z + 4;
		newMj.zy.minp.y = mj.center.y - 75;
		newMj.zy.maxp.y = mj.center.y + 75;
		newMj.zy.minp.x = mj.center.x - 35;
		newMj.zy.maxp.x = mj.center.x + 35;

	}
	else
	{
		// ��y��ƽ��
		newMj.fx = 1;
		// ������
		newMj.sx.minp.z = mj.center.z - 35;
		newMj.sx.maxp.z = mj.center.z + 35;
		newMj.sx.minp.y = mj.center.y - 4;
		newMj.sx.maxp.y = mj.center.y + 4;
		newMj.sx.minp.x = mj.center.x - 75;
		newMj.sx.maxp.x = mj.center.x + 75;
		// ������
		newMj.zy.minp.z = mj.center.z - 4;
		newMj.zy.maxp.z = mj.center.z + 4;
		newMj.zy.minp.y = mj.center.y - 35;
		newMj.zy.maxp.y = mj.center.y + 35;
		newMj.zy.minp.x = mj.center.x - 75;
		newMj.zy.maxp.x = mj.center.x + 75;

	}
	return newMj;

}

int TestMj(const TEntBox &newMj, const EntBox &p2)
{

	if (p2.Layer != Layer_door && p2.Type == Solid && p2.Layer != Layer_wjls && maxbord(p2)
		&& BoxIntersectBox2(newMj.sx, p2, 0.0) && !EquaPoint(newMj.sx.center, p2.center))
		return 1;
	if (p2.Layer != Layer_door && p2.Type == Solid && p2.Layer != Layer_wjls && maxbord(p2)
		&& BoxIntersectBox2(newMj.zy, p2, 0.0) && !EquaPoint(newMj.sx.center, p2.center))
		return 2;

	
	return 0;
	
}

// 9mm��δ��ۼ�⣬ע��װ�κ�
int dec9mmBB(const EntBox &bb9, const EntBox &adlist)
{
	// �ҵ� 9mm ��� �ж����Ĵ�����С���Ƿ���һ�������κΰ���ཻ
	
	int a = 2;// ��λ�ƴ�С
	//bb9mm = (round(ent[i].maxp.x - ent[i].minp.x) == 9 || round(ent[i].maxp.z - ent[i].minp.z) == 9 || round(ent[i].maxp.y - ent[i].minp.y) == 9)
	AcGePoint3d maxp;
	AcGePoint3d minp;
	if (round(bb9.maxp.x - bb9.minp.x) == 9)
	{
		// y ��
		maxp.x = bb9.center.x;
		maxp.y = bb9.maxp.y;
		maxp.z = bb9.maxp.z + a;

		minp.x = bb9.center.x;
		minp.y = bb9.minp.y;
		minp.z = bb9.minp.z - a;
	}
	else
	{
		// x ��
		maxp.x = bb9.maxp.x;
		maxp.y = bb9.center.y;
		maxp.z = bb9.maxp.z + a;

		minp.x = bb9.minp.x;
		minp.y = bb9.center.y;
		minp.z = bb9.minp.z - a;
	}

	if (bb9.id != adlist.id&&PointInBoxTangency(maxp, adlist))
		return 1; //���

	if (bb9.id != adlist.id&&PointInBoxTangency(minp, adlist))
		return 2; // С��
	return 0;
}

// �жϰ��
/* 
���ɵ�
�˿���18 ������������Ӵ�����1234
�˿���18 ���п�λ���� 1234
�˿�����18�� �п�λ������ i++
��18����2��
*/
int JudgeBord(const EntBox &p1)
{
	if (p1.Layer == Layer_door || p1.Layer == Layer_wjls || p1.Type != Solid)
		return 0;
	if (bigfun2c(p1,47)&&bigfun1c(p1, 188)&&p1.maxp.z>79)
	{
		if (round(p1.maxp.x - p1.minp.x) == 18 ||
			round(p1.maxp.x - p1.minp.x) == 25 ||
			round(p1.maxp.x - p1.minp.x) == 36)
			return 1;
			// 1x ��Ϊ18��
		if (round(p1.maxp.y - p1.minp.y) == 18 ||
			round(p1.maxp.y - p1.minp.y) == 25 ||
			round(p1.maxp.y - p1.minp.y) == 36)
			return 2;
			// 2y ��Ϊ18��
		if (round(p1.maxp.z - p1.minp.z) == 18 ||
			round(p1.maxp.z - p1.minp.z) == 25 ||
			round(p1.maxp.z - p1.minp.z) == 36)
			return 3;
			// 3z ��Ϊ18��
		return 0;
	}
	return 0;
}

void TestMjLs(std::vector<EntBox> &door, std::vector<AcGePoint3d> &ls, std::vector<MyMj> &mj)
{
	// �ж�ͬһ�����Ž� ��ֵͳһ
	if (!mj.empty())
	{
		for (int i = 0; i < door.size(); i++)
		{
			
			double value = 0;
			AcGePoint3d pp;
			for (int j = 0; j < mj.size(); j++)
			{
				if (PointInBoxTangency(mj[j].p1.sx.center, door[i]))
				{
					if (mj[j].minValue != 0 && abs(value - mj[j].minValue) > 2
						&& value != 0)
					{
						CreateArrow(mj[j].p1.sx.center, mj[j].fx, 0, 110, 100);
						CreateLine(pp, mj[j].p1.sx.center, 1);
						CText(mj[j].p1.sx.center, _T("�������ָ�ֵ�Ž�"), mj[j].fx);
						break;
					}
					value = mj[j].minValue;
					pp = mj[j].p1.sx.center;

				}
			}
		}
	}
	// �������λ��
	if (!ls.empty())
	{
		
		std::vector<DrLs> drlist; // �����������б�
		for (int i = 0; i < door.size(); i++)
		{
			DrLs drls1;
			for (int j = 0; j < ls.size(); j++)
			{
				if (PointInBoxTangency(ls[j], door[i]))
				{
					drls1.lspoint.push_back(ls[j]);
				}
			}
			// �����źϳ�
			if (!drls1.lspoint.empty())
			{
				drls1.DoorDt = door[i];
				drlist.push_back(drls1);
			}
		}

		// �������λ�ô���
		if (drlist.size()>1)
		{
			std::vector<AcGePoint3d> errovec;
			for (int i = 0; i < drlist.size(); i++)
			{
				for (int j = 0; j < drlist.size(); j++)
				{
					if (i != j
						&& abs(drlist[i].DoorDt.minp.z - drlist[j].DoorDt.minp.z) < 9
						/*&& abs(drlist[i].DoorDt.center.z - drlist[j].DoorDt.center.z) < 1*/
						&& abs(drlist[i].DoorDt.maxp.z - drlist[j].DoorDt.maxp.z) < 50
						)		
					{
						std::vector<double> p11;
						std::vector<double> p12;
						//z ��߶ȼ��� �����Ƚ�
						
						for (int s = 0; s < drlist[j].lspoint.size(); s++)
						{
							p11.push_back((int)(drlist[j].lspoint[s].z));
						}
						for (int s = 0; s < drlist[i].lspoint.size(); s++)
						{
							p12.push_back((int)(drlist[i].lspoint[s].z));
						}
			
						for (int s = 0; s < p11.size(); s++)
						{
							for (int ss = 0; ss < p12.size(); ss++)
							{
								if (abs(p11[s] - p12[ss]) < 1)break;
								if (ss==p12.size()-1)
								{
									
									CreateArrow(drlist[i].lspoint[s], 0, 0, 100, 100);
									errovec.push_back(drlist[i].lspoint[s]);
								}
							}
						}
					}
				}
			}
			// �㼯�ľ���
			if (!errovec.empty())
			{
				double zx=0;
				double zy=0;
				double zz=0;
				for (int s1 = 0; s1 < errovec.size(); s1++)
				{
					zx += errovec[s1].x;
					zy += errovec[s1].y;
					zz += errovec[s1].z;
				}
				//+100�����ָ߶�
				CText(AcGePoint3d(zx/errovec.size(),zy/errovec.size(),zz/errovec.size()+60),
					_T("���ָ߶Ȳ�ͬ!"), dec01(drlist[0].DoorDt));
			}
		}
	}
	// ���ϳɺ��Ű�λ�� �ֱ������²����ŷ�  ���Ҳ����ŷ�
	// ��������54 ������

	for (int x = 0; x < door.size(); x++)
	{
		
		// �������ж�54 ���������߶�1600 �Ļ�׼��������
		for (int s = 0; s < ls.size(); s++)
		{
			double z1 = round(abs(door[x].maxp.z - ls[s].z));//����
			double z2 = round(abs(door[x].minp.z - ls[s].z));//����

			if (PointInBoxTangency(ls[s], door[x]))
			{
				if (z1 == 54 && door[x].minp.z > 1600)
				{
					CreateArrow(ls[s], dec01(door[x]), 0, 120, 100);
					CText(door[x].center, _T("��������,�Ű巽����!"),dec01(door[x]));
				}
				if (z2 == 54 && door[x].minp.z < 1600)
				{
					CreateArrow(ls[s], dec01(door[x]), 0, 120, 100);
					CText(door[x].center, _T("��������,�Ű巽����!"), dec01(door[x]));
				}
			}
		}
		//���������ŷ�δ����!
		double mcz = 0.5;
		//CreateLine(door[x].maxp, door[x].minp, 2);
		for (int i = 0; i < door.size(); i++)
		{
			// �жϵ���z ��С���ֵλ����18���ڵ�
			// �����ŷ첻����
			//CreateLine(door[i].maxp, door[i].minp, 1);
			int mfx = dec01(door[x]);
			int mfx1 = dec01(door[i]);
			// y ���� �����ŷ첻����
			if (abs(door[x].minp.z - door[i].maxp.z) < 18 &&
				mfx == mfx1 &&
				mfx == 1)
			{
				double cz = abs(door[x].maxp.y - door[i].maxp.y);
				double cz1 = abs(door[x].minp.y - door[i].minp.y);
				if ((cz<18 && cz>mcz) || (cz1<18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 1, _T("_�����ŷ�δ����!"));
				}
			}
			// x ���������ŷ첻����
			if (abs(door[x].minp.z - door[i].maxp.z) < 18 &&
				mfx == mfx1 &&
				mfx == 0)
			{
				double cz = abs(door[x].maxp.x - door[i].maxp.x);
				double cz1 = abs(door[x].minp.x - door[i].minp.x);
				if ((cz<18 && cz>mcz) || (cz1<18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 1, _T("_�����ŷ�δ����!"));
				}
			}
			// �ж� x ���� �Ű����� �ŷ첻����
			if (abs(door[x].minp.x - door[i].maxp.x) < 18 &&
				mfx == mfx1 &&
				mfx == 0)
			{
				// С��z��߶Ȳ�ֵ
				double cz = abs(door[x].maxp.z - door[i].maxp.z);
				double cz1 = abs(door[x].minp.z - door[i].minp.z);
				if ((cz < 18 && cz>mcz) || (cz1 < 18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 0, _T("_�����ŷ�δ����!"));
				}
			}
			// �ж� y ���� �Ű����� �ŷ첻����
			if (abs(door[x].minp.y - door[i].maxp.y) < 18 &&
				mfx == mfx1 &&
				mfx == 1)
			{
				// С��z��߶Ȳ�ֵ
				double cz = abs(door[x].maxp.z - door[i].maxp.z);
				double cz1 = abs(door[x].minp.z - door[i].minp.z);
				if ((cz < 18 && cz>mcz) || (cz1 < 18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 0, _T("_�����ŷ�δ����!"));
				}
			}
			// �Ű���С ���ֵ��𲻴� y ����y �����6-18 ����
			if (mfx == mfx1 && mfx == 0)
			{
				double bcz = abs(door[x].maxp.x - door[i].minp.x);
				double xcz = abs(door[x].minp.x - door[i].maxp.x);
				double ycha = abs(door[x].maxp.y - door[i].maxp.y);
				double p1z = door[x].center.z< door[i].maxp.z&&door[x].center.z > door[i].minp.z;
				double p2z = door[i].center.z< door[x].maxp.z&&door[i].center.z > door[x].minp.z;

				if (((bcz < 18 && bcz>5) || (xcz < 18 && xcz>5))&&(p1z||p2z)&&ycha<20)
				{
					CLs(door[x], door[i], mfx, 0, _T("_�ŷ����4!"));
				}
			}
			if (mfx == mfx1 && mfx == 1)
			{
				double bcz = abs(door[x].maxp.y - door[i].minp.y);
				double xcz = abs(door[x].minp.y - door[i].maxp.y);
				double p1z = door[x].center.z< door[i].maxp.z&&door[x].center.z > door[i].minp.z;
				double p2z = door[i].center.z< door[x].maxp.z&&door[i].center.z > door[x].minp.z;
				double xcha = abs(door[x].maxp.y - door[i].maxp.y);
				if (((bcz < 18 && bcz>5) || (xcz < 18 && xcz>5)) && (p1z || p2z)&&xcha<20)
				{
					CLs(door[x], door[i], mfx, 0, _T("_�ŷ����4!"));
				}
			}
				
		}
	}
}

// ��ʱ�������һ;
// �ж�����1�ߴ���ĳ��
bool judeRegion1(const rec3d &m1, double b1)
{
	bool k = false;
	if (m1.p1.x == m1.p2.x)
	{
		// yz����
		double ymay = (m1.p1.y > m1.p2.y) ? m1.p1.y : m1.p2.y;
		double ymin = (m1.p1.y < m1.p2.y) ? m1.p1.y : m1.p2.y;
		//
		double zmay = (m1.p1.z > m1.p2.z) ? m1.p1.z : m1.p2.z;
		double zmin = (m1.p1.z < m1.p2.z) ? m1.p1.z : m1.p2.z;
		if (ymay - ymin > b1 || zmay - zmin > b1) k = true;

	}
	if (m1.p1.y == m1.p2.y)
	{
		// xz����
		double xmax = (m1.p1.x > m1.p2.x) ? m1.p1.x : m1.p2.x;
		double xmin = (m1.p1.x < m1.p2.x) ? m1.p1.x : m1.p2.x;
		//
		double zmax = (m1.p1.z > m1.p2.z) ? m1.p1.z : m1.p2.z;
		double zmin = (m1.p1.z < m1.p2.z) ? m1.p1.z : m1.p2.z;
		if (xmax - xmin > b1 || zmax - zmin > b1) k = true;

	}
	if (m1.p1.z == m1.p2.z)
	{
		// xy����
		double xmax = (m1.p1.x > m1.p2.x) ? m1.p1.x : m1.p2.x;
		double xmin = (m1.p1.x < m1.p2.x) ? m1.p1.x : m1.p2.x;
		//
		double ymax = (m1.p1.y > m1.p2.y) ? m1.p1.y : m1.p2.y;
		double ymin = (m1.p1.y < m1.p2.y) ? m1.p1.y : m1.p2.y;
		if (xmax - xmin > b1 || ymax - ymin > b1) k = true;


	}
	return k;
}

// ���ཻ�ж�
bool mxj(const rec3d &r1, const rec3d &r2, int fx)
{
	// Ҫ�ִ�С�� r1 С��
	// p1 ��������ʱ �������С��
	// Fb �ǰ����߾��� ��������
	double FB = 2;

	recmo m1;
	recmo m2;
	
	if (fx == 1) // ����λ�� 1��ʱ��λ����ߵ�Ϊ z
	{
		m1 = { r1.p1.x, r1.p1.y ,r1.p2.x, r1.p2.y };
		m2 = { r2.p1.x - FB, r2.p1.y - FB ,r2.p2.x + FB, r2.p2.y + FB };
	}
	if (fx == 2)// 2��ʱ��λ����ߵ�Ϊ y
	{
		m1 = { r1.p1.x, r1.p1.z ,r1.p2.x, r1.p2.z };
		m2 = { r2.p1.x - FB, r2.p1.z - FB ,r2.p2.x + FB, r2.p2.z + FB };
	}
	if (fx == 3)// 3��ʱ��λ����ߵ�Ϊ x
	{
		m1 = { r1.p1.y, r1.p1.z ,r1.p2.y, r1.p2.z };
		m2 = { r2.p1.y - FB, r2.p1.z - FB ,r2.p2.y + FB, r2.p2.z + FB };
	}
	//
	
	if (MandM(m1, m2, 2)&&abs(r1.dd-r2.dd)<FB&&r1.zx==r2.zx&&r1.zx==fx&&r1.id!=r2.id)
		return true;
	return false;



}

bool newmim(const rec3d &r1, const rec3d &r2, int fx)
{
	// Ҫ�ִ�С�� r1 С��
	// p1 ��������ʱ �������С��
	// Fb �ǰ����߾��� ��������
	double FB = 2;

	recmo m1;
	recmo m2;

	if (fx == 1) // ����λ�� 1��ʱ��λ����ߵ�Ϊ z
	{
		m1 = { r1.p1.x, r1.p1.y ,r1.p2.x, r1.p2.y };
		m2 = { r2.p1.x - FB, r2.p1.y - FB ,r2.p2.x + FB, r2.p2.y + FB };
	}
	if (fx == 2)// 2��ʱ��λ����ߵ�Ϊ y
	{
		m1 = { r1.p1.x, r1.p1.z ,r1.p2.x, r1.p2.z };
		m2 = { r2.p1.x - FB, r2.p1.z - FB ,r2.p2.x + FB, r2.p2.z + FB };
	}
	if (fx == 3)// 3��ʱ��λ����ߵ�Ϊ x
	{
		m1 = { r1.p1.y, r1.p1.z ,r1.p2.y, r1.p2.z };
		m2 = { r2.p1.y - FB, r2.p1.z - FB ,r2.p2.y + FB, r2.p2.z + FB };
	}
	bool s2 = MandM(m1, m2,-3);
	// �е��Ƿ���������
	double center_x = (m1.x1 + m1.x11)/2;
	double center_y = (m1.y2 + m1.y22)/2;
	if (s2&&
		r1.id!=r2.id
		&&r1.zx == r2.zx&&r1.zx == fx && abs(r1.dd - r2.dd) < FB)
	
		return true;
	return false;
}

// �޳���
bool tcm(const rec3d &r1, int fx)
{
	recmo m1;

	if (fx == 1) // ����λ�� 1��ʱ��λ����ߵ�Ϊ z
	{
		m1 = { r1.p1.x, r1.p1.y ,r1.p2.x, r1.p2.y };
		
	}
	if (fx == 2)// 2��ʱ��λ����ߵ�Ϊ y
	{
		m1 = { r1.p1.x, r1.p1.z ,r1.p2.x, r1.p2.z };
		
	}
	if (fx == 3)// 3��ʱ��λ����ߵ�Ϊ x
	{
		m1 = { r1.p1.y, r1.p1.z ,r1.p2.y, r1.p2.z };
	}
	double w1 = abs(m1.x1 - m1.x11);
	double w2 = abs(m1.y2 - m1.y22);
	if (w1 < 70 && w2 < 70 && r1.Layer == Layer_skt) return true;
	return false;
}
// �޳�ʵ��
bool tcbox(const EntBox &p1)
{
	if (p1.Layer==Layer_skt)
	{
		bool x1 = abs(p1.maxp.x - p1.minp.x) < 70;
		bool x2 = abs(p1.maxp.y - p1.minp.y) < 70;
		bool x3 = abs(p1.maxp.z - p1.minp.z) < 70;
		if (x1&&x2&&x3) return true;
	}
	return false;

}

// ��������һʵ��
EntBox creatorE(const rec3d &p1)
{
	EntBox np;
	np.id = p1.id;
	// ��С��xy �����ǣ� С��xy���
	np.minp.z = p1.p1.z - 10;
	np.minp.x = p1.p1.x + 4;
	np.minp.y = p1.p1.y + 4;
	np.maxp.z = p1.p2.z + 10;
	np.maxp.x = p1.p2.x - 4;
	np.maxp.y = p1.p2.y - 4;
	return np;
}

void TestShyBord(std::vector<RecBox> &marray, std::vector<EntBox> &shym, 
	std::vector<rec3d> &shym2, std::vector<EntBox> &shyh)
{
	std::vector<rec3d> pz18m; // ��������ײ��18�漯��
	std::vector<rec3d> km18array; // �����п�λ��18�漯��
	std::vector<rec3d> shys; // �����п�λ��18�漯��

	
	
	for (int i = 0; i < marray.size(); i++)
	{
		/*CreateLine(marray[i].m18a.p1, marray[i].m18a.p2, 2);
		CreateLine(marray[i].m18b.p1, marray[i].m18b.p2, 2);
		CreateLine(marray[i].m18c.p1, marray[i].m18c.p2, 2);
		CreateLine(marray[i].m18d.p1, marray[i].m18.p2, 2);*/

		// �����п�λ��18�ļ���
		for (int j = 0; j < shym2.size(); j++)
		{
			if (mxj(shym2[j], marray[i].m18a, marray[i].m18a.zx))
				km18array.push_back(marray[i].m18a);
			if (mxj(shym2[j], marray[i].m18b, marray[i].m18b.zx))
				km18array.push_back(marray[i].m18b);
			if (mxj(shym2[j], marray[i].m18c, marray[i].m18c.zx))
				km18array.push_back(marray[i].m18c);
			if (mxj(shym2[j], marray[i].m18d, marray[i].m18d.zx))
				km18array.push_back(marray[i].m18d);
		}

		// ��������ײ�ļ���
		for (int j = 0; j < marray.size(); j++)
		{
			// �ָ�4 ��
			if (marray[j].me18a.Layer!=Layer_beib)
			{
				if (newmim(marray[i].m18a, marray[j].me18a, marray[i].m18a.zx))
					pz18m.push_back(marray[i].m18a);

				if (newmim(marray[i].m18a, marray[j].me18b, marray[i].m18a.zx))
					pz18m.push_back(marray[i].m18a);
				// �ָ�
				if (newmim(marray[i].m18b, marray[j].me18a, marray[i].m18b.zx))
					pz18m.push_back(marray[i].m18b);
				if (newmim(marray[i].m18b, marray[j].me18b, marray[i].m18b.zx))
					pz18m.push_back(marray[i].m18b);
				// �ָ�
				if (newmim(marray[i].m18c, marray[j].me18a, marray[i].m18c.zx))
					pz18m.push_back(marray[i].m18c);
				if (newmim(marray[i].m18c, marray[j].me18b, marray[i].m18c.zx))
					pz18m.push_back(marray[i].m18c);
				// �ָ�
				if (newmim(marray[i].m18d, marray[j].me18a, marray[i].m18d.zx))
					pz18m.push_back(marray[i].m18d);
				if (newmim(marray[i].m18d, marray[j].me18b, marray[i].m18d.zx))
					pz18m.push_back(marray[i].m18d);
			}
		}
	}

	// ����һ����ϵ�� 
	// ����©�׵�����һ��
	double pzxs = 3.5;
	//acutPrintf(_T("\n��ײ������:%d"), pz18m.size());
	for (int i = 0; i < pz18m.size(); i++)
	{
		//CreateLine(pz18m[i].p1, pz18m[i].p2, 12);
		EntBox entp;
		entp.minp = pz18m[i].p1;
		entp.maxp = pz18m[i].p2;
		entp.Layer = pz18m[i].Layer;
		for (int j = 0; j < shym.size(); j++)
		{
			//CreateLine(shym[j].minp, shym[j].maxp, 2);
			// ����һ����
			if (BoxIntersectBox2(ExspansionEnt(shym[j], pzxs),entp)|| tcbox(entp)) break;
			if (j==shym.size()-1)
			{
				CreateLine(pz18m[i].p1, pz18m[i].p2, 2);
				CreateBox(LineCenter(pz18m[i].p1, pz18m[i].p2), 30, 2);
			}
		}
	}
	// ����λ��ײ

	// ȥ��
	std::vector<AcDbObjectId> id_list;
	std::vector<AcDbObjectId> erid_list;
	for (int i = 0; i < km18array.size(); i++)
	{
		
		if (abs(km18array[i].p1.z - km18array[i].p2.z) < 1)
		{
			
			if (std::find(id_list.begin(), id_list.end(), km18array[i].id) != id_list.end())
			{
				int a = 1;
			}
			else
			{
				id_list.push_back(km18array[i].id);
				/*acutPrintf(_T("\n %.2f"), km18array[i].p1.x - km18array[i].p2.x);
				acutPrintf(_T("\n %d"), km18array[i].id);*/
				//CreateLine(km18array[i].p1, km18array[i].p2, 1);
				EntBox np = creatorE(km18array[i]);
				
				// ���ײ��λ�� �����ǽ������������box, ��ѯ�����ཻ�ĺ�����
				int sa = 0;
				for (int j = 0; j < shyh.size(); j++)
				{
					if (std::find(id_list.begin(), id_list.end(), km18array[i].id) != erid_list.end())
					{
						int a = 1;
					}
					if (BoxIntersectBox2(np, shyh[j]))
					{
						//CreateBox(shyh[j].center, 30, 6);
						CreateArrow(shyh[j].center, 0, 0, 110, 40);
						CreateArrow(shyh[j].center, 1, 0, 110, 40);
						/*CText(shyh[j].center, _T("ײ����!"), 0);*/
						sa = 1;
					}
					
				}
				if (sa == 1)
				{
					CText(np.minp, _T("ײ����!"), 0);
					CText(np.minp, _T("ײ����!"), 1);
				}
			}
			
		}
		
		//km18array[i].id
		//if (abs(km18array[i].p1.z - km18array[i].p2.z)<1)
		//{

		//	// �жϷ���
		//	acutPrintf(_T("\n %.2f"), km18array[i].p1.x - km18array[i].p2.x);
		//	CreateLine(km18array[i].p1, km18array[i].p2, 1);
		//}
		
	}
	
	

}

// 0 ����6����
// 1 Բ�����
// 2 ˫��Բ��

int isbox(AcDbObjectId &id)
{
	AcDbEntity *pEnt = NULL;

	// ��ȡʵ��
	if (acdbOpenObject(pEnt, id, AcDb::kForRead) == Acad::eOk)
	{
		//AcBrBrep brepEnt;

		//brepEnt.set(*pEnt);

		//acutPrintf(_T("st"));

		////����һ����ı�����

		//AcBr::ErrorStatus returnValue = AcBr::eOk;

		//AcBrBrepEdgeTraverser  brepEdgeTrav;

		//returnValue = brepEdgeTrav.setBrep(brepEnt);
		AcBrBrep brepEnt;

		brepEnt.set(*pEnt);

		//����һ����ı�����

		AcBr::ErrorStatus returnValue = AcBr::eOk;

		AcBrBrepFaceTraverser  brepFaceTrav;

		returnValue = brepFaceTrav.setBrep(brepEnt);

		int m = 0;
		int cy = 0;
		//int nu = 0;

		//int n = 0;
		//double center;
		double rr = 0;


		while ((!brepFaceTrav.done()) && (returnValue == AcBr::eOk))

		{

			AcBrFace brFace;

			AcBr::ErrorStatus es = brepFaceTrav.getFace(brFace);

			if (es == AcBr::eOk)

			{

				AcGeSurface *pAcGeSurface;

				brFace.getSurface(pAcGeSurface);

				AcGeExternalBoundedSurface *ebSurf = (AcGeExternalBoundedSurface*)pAcGeSurface;

				if (ebSurf != NULL)

				{
					if (ebSurf->isCylinder())
					{
						cy += 1;
						cy += 1;
						AcGeCylinder *pCylinder = (AcGeCylinder*)ebSurf;
						/*if (rr != 0 && abs(rr - pCylinder->radius()) > 10)
						{
							CreateArrow(pCylinder->origin(), 1, 0, 1, 110);
							CText(pCylinder->origin(), _T("��Բ���� R �뾶��һ��!"), 0);
						}*/
						rr = pCylinder->radius();
						//n += 1;
					}
					if (ebSurf->isPlane())
					{
						m += 1;
					}
					/*if (ebSurf->isNurbs())
					{
						nu += 1;
					}*/
					/*AcGeCylinder *pCylinder = (AcGeCylinder*)ebSurf;

					acutPrintf(TEXT("\nԲ�������Ϣ:"));

					acutPrintf(TEXT("\n�뾶:%.2f"), pCylinder->radius());

					AcGePoint3d center = pCylinder->origin();

					acutPrintf(TEXT("\n���ĵ㣺(%.2f,%2.f,%.2f)"), center.x, center.y, center.z);

					AcGeVector3d axis = pCylinder->axisOfSymmetry();

					acutPrintf(TEXT("\n��������:(%.2f,%2.f,%.2f)"), axis.x, axis.y, axis.z);
*/
				}

				delete pAcGeSurface;

			}

			returnValue = brepFaceTrav.next();

		}

		pEnt->close();
		if (m == 6 && cy == 0)
			return 0;
		if (m == 6 && cy == 1)
			return 1;
		if (m == 6 && cy == 2)
			return 2;
	}
	//acutPrintf(_T("\n �򲻿�����2?"));
	return 999;
}
// �ж��Ƿ���Բ���� ���Ҽ�鵥���R �뾶��ֵС��10
RBord getRBord(AcDbObjectId &id)
{
	AcDbEntity *pEnt = NULL;
	RBord bb;
	// ��ȡʵ��
	if (acdbOpenObject(pEnt, id, AcDb::kForRead) == Acad::eOk)
	{
		
		AcBrBrep brepEnt;

		brepEnt.set(*pEnt);

		AcBr::ErrorStatus returnValue = AcBr::eOk;

		AcBrBrepFaceTraverser  brepFaceTrav;

		returnValue = brepFaceTrav.setBrep(brepEnt);

		

		while ((!brepFaceTrav.done()) && (returnValue == AcBr::eOk))

		{

			AcBrFace brFace;

			AcBr::ErrorStatus es = brepFaceTrav.getFace(brFace);

			if (es == AcBr::eOk)

			{

				AcGeSurface *pAcGeSurface;

				brFace.getSurface(pAcGeSurface);

				AcGeExternalBoundedSurface *ebSurf = (AcGeExternalBoundedSurface*)pAcGeSurface;

				if (ebSurf != NULL&& ebSurf->isCylinder())

				{
					AcGeCylinder *pCylinder = (AcGeCylinder*)ebSurf;
					bb.center = pCylinder->origin();
					bb.r = pCylinder->radius();
					/*if (ebSurf->isNurbs())
					{
						nu += 1;
					}*/
					/*AcGeCylinder *pCylinder = (AcGeCylinder*)ebSurf;

					acutPrintf(TEXT("\nԲ�������Ϣ:"));

					acutPrintf(TEXT("\n�뾶:%.2f"), pCylinder->radius());

					AcGePoint3d center = pCylinder->origin();

					acutPrintf(TEXT("\n���ĵ㣺(%.2f,%2.f,%.2f)"), center.x, center.y, center.z);

					AcGeVector3d axis = pCylinder->axisOfSymmetry();

					acutPrintf(TEXT("\n��������:(%.2f,%2.f,%.2f)"), axis.x, axis.y, axis.z);
*/
				}

				delete pAcGeSurface;

			}

			returnValue = brepFaceTrav.next();

		}


		//acutPrintf(_T("\n m:%d cy:%d nu:%d"), m, cy, nu);
		pEnt->close();
		return bb;
	}
	acutPrintf(_T("\n �򲻿�����1?"));
	return bb;
}


void testbref(AcDbObjectId &id)
{
	AcDbEntity *pEnt = NULL;

	// ��ȡʵ��
	if (acdbOpenObject(pEnt, id, AcDb::kForRead) == Acad::eOk)
	{
		//AcBrBrep brepEnt;

		//brepEnt.set(*pEnt);

		//acutPrintf(_T("st"));

		////����һ����ı�����

		//AcBr::ErrorStatus returnValue = AcBr::eOk;

		//AcBrBrepEdgeTraverser  brepEdgeTrav;

		//returnValue = brepEdgeTrav.setBrep(brepEnt);
		AcBrBrep brepEnt;

		brepEnt.set(*pEnt);

		//����һ����ı�����

		AcBr::ErrorStatus returnValue = AcBr::eOk;

		AcBrBrepFaceTraverser  brepFaceTrav;

		returnValue = brepFaceTrav.setBrep(brepEnt);
		
		int m = 0;
		int cy = 0;
		int nu = 0;
		
		while ((!brepFaceTrav.done()) && (returnValue == AcBr::eOk))

		{

			AcBrFace brFace;

			AcBr::ErrorStatus es = brepFaceTrav.getFace(brFace);

			if (es == AcBr::eOk)

			{

				AcGeSurface *pAcGeSurface;

				brFace.getSurface(pAcGeSurface);

				AcGeExternalBoundedSurface *ebSurf = (AcGeExternalBoundedSurface*)pAcGeSurface;

				if (ebSurf != NULL )

				{
					if (ebSurf->isCylinder())
					{
						cy+= 1;
						AcGeCylinder *pCylinder = (AcGeCylinder*)ebSurf;

						//acutPrintf(TEXT("\nԲ�������Ϣ:"));

						acutPrintf(TEXT("\n Բ���뾶�뾶:%.2f, %d"), pCylinder->radius(),id);

						/*AcGePoint3d center = pCylinder->origin();

						acutPrintf(TEXT("\n���ĵ㣺(%.2f,%2.f,%.2f)"), center.x, center.y, center.z);

						AcGeVector3d axis = pCylinder->axisOfSymmetry();

						acutPrintf(TEXT("\n��������:(%.2f,%2.f,%.2f)"), axis.x, axis.y, axis.z);*/

					}
					if (ebSurf->isPlane())
					{
						m += 1;
					}
					if (ebSurf->isNurbs())
					{
						nu += 1;
					}
		
				}

				delete pAcGeSurface;

			}

			returnValue = brepFaceTrav.next();

		}


		/*if (returnValue == AcBr::eOk)

		{

			while ((!brepEdgeTrav.done()) && (returnValue == AcBr::eOk))

			{

				AcBrEdge brEdge;

				brepEdgeTrav.getEdge(brEdge);

				AcGeCurve3d *pGeCurve = NULL;

				if (brEdge.getCurve(pGeCurve) == AcBr::eOk)

				{

					AcGeCurve3d *pNativeCurve = NULL;

					Adesk::Boolean bRet = ((AcGeExternalCurve3d*)pGeCurve)->isNativeCurve(pNativeCurve);

					int numSample = 180;

					AcGePoint3dArray points;

					pNativeCurve->getSamplePoints(numSample, points);

					if (pNativeCurve->isClosed() && points.length() > 0)

					{
						points.append(points[0]);
						acutPrintf(_T("/n sssst:%d "),points.length());
						Arrow3dXY(points[0], 3, 122);
					}

					delete pNativeCurve;

					delete pGeCurve;
				}

				returnValue = brepEdgeTrav.next();

			}



		}*/
		acutPrintf(_T("\n m:%d cy:%d nu:%d"), m, cy, nu);
		pEnt->close();
	}
	//����һ��BREP����
}

// ��������
bool testhclc(const EntBox &p1)
{
	std::string huoc = "72IMOSXD01_IM_SHELF_INT";
	
	// p1�ǻ���
	if (p1.Layer == huoc && gsst(p1))
	{
		return true;
	}
	return false;

}

// ��������һ��ͬһ�����ںϳ�һ���µ�box