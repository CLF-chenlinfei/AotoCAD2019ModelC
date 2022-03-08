#include "StdAfx.h"
#include "alg.h"
#include "name.h"
#include <vector>
#include <tchar.h>
#include <algorithm>
#include "CreateSome.h"

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
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
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


	if (p1.Layer != Layer_door && p1.Type == Solid && (abs(p1.volume - 1856) < 1 || abs(p1.volume - 300) < 1)) return true;
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

void CLs(const EntBox &p1, const EntBox &p2, int fx,int sx, const ACHAR* text)
{
	CreateLine(p1.center, p2.center, 120);
	CreateArrow(p1.center, fx, sx, 120, 120);
	CreateArrow(p2.center, fx, sx, 120, 120);
	CText(LineCenter(p1.center, p2.center), text, fx);

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
						{
							ls.push_back(adlist[ss].id);
							
						}
						if ((BoxIntersectBox2(ExpandBack(p2, 1, 3), adlist[ss], -0.5) && gsst(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
						{
							ls.push_back(adlist[ss].id);
							
						}
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
					if ((BoxIntersectBox2(ExpandBack(p1, 2, 3), adlist[ss], -0.5) && gsst(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
					{
						ls.push_back(adlist[ss].id);
						
					}
					if ((BoxIntersectBox2(ExpandBack(p2, 2, 3), adlist[ss], -0.5) && gsst(adlist[ss]) && adlist[ss].id != p1.id&&adlist[ss].id != p2.id))
					{
						ls.push_back(adlist[ss].id);
						
					}
				}

					std::sort(ls.begin(), ls.end());
					ls.erase(unique(ls.begin(), ls.end()), ls.end());
					
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
	bool p1yt = ((int)(p1.maxp.x - p1.minp.x) == 15 || (int)(p1.maxp.y - p1.minp.y) == 15) &&
		p1.Layer == Layer_wjls && (int)(p1.maxp.z - p1.minp.z) == 30;
	if (!p1yt)
		return 0;
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
		&& p1.Type == Solid && p1.Layer == Layer_door &&
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
						&& abs(drlist[i].DoorDt.center.z - drlist[j].DoorDt.center.z) < 1
						&& abs(drlist[i].DoorDt.maxp.z - drlist[j].DoorDt.maxp.z) < 50
						)
					{
						for (int s = 0; s < drlist[j].lspoint.size(); s++)
						{
							for (int ss = 0; ss < drlist[i].lspoint.size(); ss++)
							{
								if (round(drlist[j].lspoint[s].z) == round(drlist[i].lspoint[ss].z))break;
								if (ss == drlist[i].lspoint.size() - 1)
								{
									errovec.push_back(drlist[j].lspoint[s]);
								}
							}
						}
					}
				}
			}
			if (errovec.size() == 1)
			{
				CreateBox(errovec[0], 20, 110);
			}

			for (int i = 0; i < errovec.size(); i++)
			{
				CText(errovec[i], _T("    __����δ����!"),0);
				CreateArrow(errovec[i], 0, 0, 100, 100);
				CreateArrow(errovec[i], 1, 0, 100, 100);
				//Arrow3dXY(errovec[i], 22, 1);
				/*if (i != 0)
				{
					CreateLine(errovec[i - 1], errovec[i], 1);
				}*/
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
		for (int i = 0; i < door.size(); i++)
		{
			// �жϵ���z ��С���ֵλ����18���ڵ�
			// �����ŷ첻����
			int mfx = dec01(door[x]);
			int mfx1 = dec01(door[i]);
			// y ���� �����ŷ첻����
			if (abs(door[x].minp.z - door[i].maxp.z) < 18 &&
				mfx == mfx1 &&
				mfx == 1)
			{
				double cz = abs(door[x].maxp.y - door[i].maxp.y);
				double cz1 = abs(door[x].minp.y - door[i].minp.y);
				if ((cz<18 && cz>0.5) || (cz1<18 && cz1>0.5))
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
				if ((cz<18 && cz>0.5) || (cz1<18 && cz1>0.5))
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
				if ((cz < 18 && cz>1) || (cz1 < 18 && cz1>1))
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
				if ((cz < 18 && cz>1) || (cz1 < 18 && cz1>1))
				{
					CLs(door[x], door[i], mfx, 0, _T("_�����ŷ�δ����!"));
				}
			}
			// �Ű���С ���ֵ��𲻴� y ����y �����6-18 ����
			if (mfx == mfx1 && mfx == 0)
			{
				double bcz = abs(door[x].maxp.x - door[i].minp.x);
				double xcz = abs(door[x].minp.x - door[i].maxp.x);
				double p1z = door[x].center.z< door[i].maxp.z&&door[x].center.z > door[i].minp.z;
				double p2z = door[i].center.z< door[x].maxp.z&&door[i].center.z > door[x].minp.z;

				if (((bcz < 18 && bcz>5) || (xcz < 18 && xcz>5))&&(p1z||p2z))
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

				if (((bcz < 18 && bcz>5) || (xcz < 18 && xcz>5)) && (p1z || p2z))
				{
					CLs(door[x], door[i], mfx, 0, _T("_�ŷ����4!"));
				}
			}
				
		}
	}
}
