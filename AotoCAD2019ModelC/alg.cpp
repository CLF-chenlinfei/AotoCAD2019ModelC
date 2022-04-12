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



// 最大板件
bool maxbord(const EntBox &p1)
{
	double x = abs(p1.maxp.x - p1.minp.x) < 2800;
	double y = abs(p1.maxp.y - p1.minp.y) < 2800;
	double z = abs(p1.maxp.z - p1.minp.z) < 2800;
	if (x&&y&&z) return true;
	return false;
}

// 盒子相交2 这个是全面的
bool BoxIntersectBox(const EntBox &b1, const EntBox &b2, double pz)
{
	// 创建三个面域p1
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
	// 检查三个面域是否相交
}
//点与包围盒相切
bool PointBoxTangency(const AcGePoint3d &p1, const EntBox &box, double expand)
{
	//点位是否与盒子相切
	//分正反面
	if (
		((p1.x >= box.minp.x&&p1.x <= box.maxp.x&&p1.z <= box.maxp.z&&p1.z >= box.minp.z) && (abs(p1.y - box.maxp.y) < expand || abs(p1.y - box.minp.y) < expand)) ||
		((p1.y >= box.minp.y&&p1.y <= box.maxp.y&&p1.z <= box.maxp.z&&p1.z >= box.minp.z) && (abs(p1.x - box.maxp.x) < expand || abs(p1.x - box.minp.x) < expand)) ||
		((p1.x >= box.minp.x&&p1.x <= box.maxp.x&&p1.y <= box.maxp.y&&p1.y >= box.minp.y) && (abs(p1.z - box.maxp.z) < expand || abs(p1.z - box.minp.z) < expand))
		)return true;
	return false;
}

// 盒子相切
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
// 指定长宽2
bool FiltrationEnt(const EntBox &p1, int d1, int d2)
{
	// 两边距离接近 小于0.1 返回ture else 0
	double s1 = abs(p1.maxp.x - p1.minp.x);
	double s2 = abs(p1.maxp.y - p1.minp.y);

	if ((abs(s1 - d1) < 0.1&& abs(s2 - d2) < 0.1)
		|| (abs(s2 - d1) < 0.1&& abs(s1 - d2) < 0.1))
		return true;
	return false;


}
//点在包围盒内
bool PointInBoxTangency(const AcGePoint3d &p1, const EntBox &box)
{

	if (
		box.minp.x < p1.x&&p1.x < box.maxp.x&&
		box.minp.y < p1.y&&p1.y < box.maxp.y&&
		box.minp.z < p1.z&&p1.z < box.maxp.z
		)return true;
	return false;

}
// 点在面域内
bool PointInRec(double x, double y, recmo rec)
{
	if (rec.x1<x&&rec.x11>x&&rec.y2<y&&rec.y22>y) return true;
	return false;
}
// 检查xy 顶面方向 板件有一项符合标准
bool dec_xy(const EntBox &p1)
{
	// 检查xy 顶面方向 板件有一项符合标准
	double xx = p1.maxp.x - p1.minp.x;
	double yy = p1.maxp.y - p1.minp.y;
	if (xx < 30 || yy < 30) return true;
	return false;
}
 //浮点数相等
bool DoubleEq(double n1, double n2, double chaz)
{
	if (abs(n1 - n2) < chaz) return true;
	return false;
}
// 两点中点
AcGePoint3d LineCenter(const AcGePoint3d &p1, const AcGePoint3d &p2)
{
	double x = (p1.x + p2.x) / 2;
	double y = (p1.y + p2.y) / 2;
	double z = (p1.z + p2.z) / 2;
	return AcGePoint3d(x, y, z);
}
// 点位相等
bool EquaPoint(const AcGePoint3d &p1, const AcGePoint3d &p2)
{
	if (DoubleEq(p1.x, p2.x) &&
		DoubleEq(p1.y, p2.y) &&
		DoubleEq(p1.z, p2.z)) return true;
	return false;

}

// 浮点数比较 前者大返回ture
bool Doublebj(double n1, double n2)
{
	if (n1 > n2) return true;
	return false;
}

// 两边大于某数
bool bigfun2c(const EntBox &p1, int wd)
{
	// 两边大于某数
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y || x && z || y && z)return true;
	return false;

}
// 是拉手孔位
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
// 三边大于某数
bool bigfun3c(const EntBox &p1, int wd)
{
	// 两边大于某数
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y&&z)return true;
	return false;

}

// 三合一判断
int isShy(const EntBox &p1)
{
	if (round(p1.volume) == 1856)
	{
		if (round(p1.maxp.z - p1.minp.z) == 29)
		{
			// 1 为竖
			return 1;
		}
		if (round(p1.maxp.x - p1.minp.x) == 29)
		{
			// 1 为竖
			return 2;
		}
		if (round(p1.maxp.y - p1.minp.y) == 29)
		{
			// 1 为竖
			return 3;
		}
		// 横着
		return 0;
	}
	// 并不是
	return 0;
}

// 1边大于某数
bool bigfun1c(const EntBox &p1, int wd)
{
	// 两边大于某数
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x || y || z)return true;
	return false;

}

// 非门板的 大于60*299 的板件
bool gsst(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 249);
	bool a2 = bigfun2c(p1, 47);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && 
		p1.Layer != Layer_door && p1.Type == Solid)return true;
	return false;
}

// 非门板的 大于60*299 的板件
bool gsst11(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 180);
	bool a2 = bigfun2c(p1, 47);
	bool a3 = bigfun3c(p1, 8);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls &&
		p1.Layer != Layer_door && p1.Type == Solid)return true;
	return false;
}
// 包含门板 大于60*299 的板件
bool gsdoor(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && p1.Type == Solid)return true;
	return false;
}
// 用于干涉检测的门板判断
// 包含门板 大于60*299 的板件
bool isdoor(const EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer == Layer_door && p1.Type == Solid)return true;
	return false;
}
// 膨胀实体
EntBox ExspansionEnt(const EntBox &p1, double greater)
{
	// 注意体积未膨胀，可以用大小点计算
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

// 面与面相交
bool MandM(recmo &m1, recmo &m2, double pz)
{
	// 矩形中心点之间的距离 一定小于等于两举行边长和的一半
	// x中心距离
	// 中心距离为 边距的中心的 差值
	// 两边距和的一半
	// 中心距变大 他们就更远 pz 小于0 腐蚀 大于0 膨胀
	double zx = abs(m1.x1 + m1.x11 - m2.x1 - m2.x11);
	double zy = abs(m1.y2 + m1.y22 - m2.y2 - m2.y22);
	// 边距相加
	double rx = abs(m1.x1 - m1.x11) + abs(m2.x1 - m2.x11);
	double ry = abs(m1.y2 - m1.y22) + abs(m2.y2 - m2.y22);
	if (zx <= rx + pz && zy <= ry + pz)
		return true;
	else
		return false;
}
//
//// 盒子相交2 这个是全面的
bool BoxIntersectBox2(const EntBox &b1, const EntBox &b2, double pz)
{
	// 创建三个面域p1
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
	// 检查三个面域是否相交
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
// 判断点位重叠
bool judeMore(const EntBox &p1, const EntBox &p2)
{
	// 判断p1,p2 是孔位
	bool p1isHole = isHole(p1);
	bool p2isHole = isHole(p2);

	// 函数判断了中心点 面积 图层 同时相等为相等
	if (pointdpoint(p1.center, p2.center) &&
		p1.id != p2.id&&
		p1.Layer == p2.Layer&&
		abs(p1.volume - p2.volume) < 0.1&&p1isHole&&p2isHole)
		return true;
	return false;
}
//// 是灯槽
bool isLamp(const EntBox &p1)
{

	if (p1.Layer == Layer_wjls && p1.Type == Solid)
	{
		double p1Height = p1.maxp.z - p1.minp.z;
		double p1Depth = p1.maxp.y - p1.minp.y;
		double p1Width = p1.maxp.x - p1.minp.x;
		// 应该分4钟情况
		if (abs(p1Height - 8.5) < 1E-6&&abs(p1Depth - 20) < 1E-6)return true;
		// x轴平行
		// y轴平行
		if (abs(p1Height - 8.5) < 1E-6&&abs(p1Width - 20) < 1E-6)return true;
		// z轴平行
		if (abs(p1Width - 8.5) < 1E-6&&abs(p1Depth - 20) < 1E-6)return true;
		// z轴竖行
		if (abs(p1Width - 20) < 1E-6&&abs(p1Depth - 8.5) < 1E-6)return true;
		return false;
	}
	return false;
}
//// 是孔位
bool isHole(const EntBox &p1)
{


	if (p1.Layer != Layer_door && 
		p1.Type == Solid && (abs(p1.volume - 1856) < 1 || 
			abs(p1.volume - 300) < 1)) return true;
	return false;

}
// 板件相交
bool Intervene(const EntBox &p1, const EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -1.0) && gsst(p1) && gsst(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// 门，板件相交
bool DoorBIntervene(const EntBox &p1, const EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -6.0) && gsdoor(p1) && gsdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// 门板相交
bool DoorIntervene(const EntBox &p1, const EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, 3.0) && isdoor(p1) && isdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id
		&& dec_xy(p1) && dec_xy(p2))
		return true;
	return false;
}
// 多背板
bool isbb(const EntBox &p1)
{
	bool a1 = bigfun1c(p1, 399);
	bool a2 = bigfun2c(p1, 200);
	bool a3 = bigfun3c(p1, 8);
	if (a1&&a2&&a3)return true;
	return false;

}
// 检查方向 1 y 方向 2 x 方向
int decXYZ(const EntBox &p1)
{
	if (round(p1.maxp.x - p1.minp.x) == 18 || round(p1.maxp.x - p1.minp.x) == 9)return 1;
	if (round(p1.maxp.y - p1.minp.y) == 18 || round(p1.maxp.y - p1.minp.y) == 9)return 2;
	if (round(p1.maxp.z - p1.minp.z) == 18 || round(p1.maxp.z - p1.minp.z) == 9)return 3;
	return 0;
}

// 0 x 方向 1 Y方向
// 返回2 就是 顶面门
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

// 面包含面
bool MinM(const recmo &m1, const recmo &m2, double pz)
{
	// 左侧点 小于 左侧点 右侧点大于右侧点 是两个点同时满足
	// 首先判断面 的左右
	// 若m1 包含m2 前面一个是 大面
	if (m1.x1 < m1.x11&&m2.x1 < m2.x11)
	{
		// 1点在左侧
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

// 背板膨胀 当他平行X轴 分别是Z 和X 变大
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
	// 找到两块背板重叠
	//std::vector<EntBox> MydataR;
	if (isbb(p1) && isbb(p2))
	{
		//x 方向18
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
		// y 方向18
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
					//acutPrintf(_T("\n 数组量:%d"), ls.size());
					if (ls.size() / 2 < 3)return true;
			}

		}
	}
	
	return false;

}

// 检查衣通位置
int TestYt2(const EntBox &p1, const EntBox &p2)
{
	// 判断p1 是衣通 p2  不是衣通
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
	// 判断衣杆方向
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
	if (PointInBoxTangency(yap, p2))return 1;// 判断大点
	
	if (PointInBoxTangency(yip, p2))return 2;// 判断小点
	return 0;
	
}
// 判断生成门铰
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
	// 判断是否是X轴的门铰
	if ((int)(mj.maxp.x - mj.minp.x - 0.1) != 13)
	{
		// 创建两个实体 x面 y 值是深度 z是高度
		// 35 是上下的高度
		//
		newMj.fx = 0;
		// 上下面
		newMj.sx.minp.z = mj.center.z - 35;
		newMj.sx.maxp.z = mj.center.z + 35;
		newMj.sx.minp.y = mj.center.y - 75;
		newMj.sx.maxp.y = mj.center.y + 75;
		newMj.sx.minp.x = mj.center.x - 4;
		newMj.sx.maxp.x = mj.center.x + 4;
		// 左右面
		newMj.zy.minp.z = mj.center.z - 4;
		newMj.zy.maxp.z = mj.center.z + 4;
		newMj.zy.minp.y = mj.center.y - 75;
		newMj.zy.maxp.y = mj.center.y + 75;
		newMj.zy.minp.x = mj.center.x - 35;
		newMj.zy.maxp.x = mj.center.x + 35;

	}
	else
	{
		// 与y轴平行
		newMj.fx = 1;
		// 上下面
		newMj.sx.minp.z = mj.center.z - 35;
		newMj.sx.maxp.z = mj.center.z + 35;
		newMj.sx.minp.y = mj.center.y - 4;
		newMj.sx.maxp.y = mj.center.y + 4;
		newMj.sx.minp.x = mj.center.x - 75;
		newMj.sx.maxp.x = mj.center.x + 75;
		// 左右面
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

// 9mm板未入槽检测，注意装饰盒
int dec9mmBB(const EntBox &bb9, const EntBox &adlist)
{
	// 找到 9mm 板件 判断他的大点或者小点是否有一个不与任何板件相交
	
	int a = 2;// 点位移大小
	//bb9mm = (round(ent[i].maxp.x - ent[i].minp.x) == 9 || round(ent[i].maxp.z - ent[i].minp.z) == 9 || round(ent[i].maxp.y - ent[i].minp.y) == 9)
	AcGePoint3d maxp;
	AcGePoint3d minp;
	if (round(bb9.maxp.x - bb9.minp.x) == 9)
	{
		// y 轴
		maxp.x = bb9.center.x;
		maxp.y = bb9.maxp.y;
		maxp.z = bb9.maxp.z + a;

		minp.x = bb9.center.x;
		minp.y = bb9.minp.y;
		minp.z = bb9.minp.z - a;
	}
	else
	{
		// x 轴
		maxp.x = bb9.maxp.x;
		maxp.y = bb9.center.y;
		maxp.z = bb9.maxp.z + a;

		minp.x = bb9.minp.x;
		minp.y = bb9.center.y;
		minp.z = bb9.minp.z - a;
	}

	if (bb9.id != adlist.id&&PointInBoxTangency(maxp, adlist))
		return 1; //大点

	if (bb9.id != adlist.id&&PointInBoxTangency(minp, adlist))
		return 2; // 小点
	return 0;
}

// 判断板件
/* 
生成的
此块板件18 面与其他板件接触的面1234
此块板件18 面有孔位的面 1234
此块板件非18面 有孔位的数量 i++
非18面有2个
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
			// 1x 面为18面
		if (round(p1.maxp.y - p1.minp.y) == 18 ||
			round(p1.maxp.y - p1.minp.y) == 25 ||
			round(p1.maxp.y - p1.minp.y) == 36)
			return 2;
			// 2y 面为18面
		if (round(p1.maxp.z - p1.minp.z) == 18 ||
			round(p1.maxp.z - p1.minp.z) == 25 ||
			round(p1.maxp.z - p1.minp.z) == 36)
			return 3;
			// 3z 面为18面
		return 0;
	}
	return 0;
}

void TestMjLs(std::vector<EntBox> &door, std::vector<AcGePoint3d> &ls, std::vector<MyMj> &mj)
{
	// 判断同一门内门铰 盖值统一
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
						CText(mj[j].p1.sx.center, _T("此门两种盖值门铰"), mj[j].fx);
						break;
					}
					value = mj[j].minValue;
					pp = mj[j].p1.sx.center;

				}
			}
		}
	}
	// 检查拉手位置
	if (!ls.empty())
	{
		
		std::vector<DrLs> drlist; // 生成拉手门列表
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
			// 拉手门合成
			if (!drls1.lspoint.empty())
			{
				drls1.DoorDt = door[i];
				drlist.push_back(drls1);
			}
		}

		// 检查拉手位置错误
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
						//z 轴高度集合 用来比较
						
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
			// 点集的均数
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
				//+100是文字高度
				CText(AcGePoint3d(zx/errovec.size(),zy/errovec.size(),zz/errovec.size()+60),
					_T("拉手高度不同!"), dec01(drlist[0].DoorDt));
			}
		}
	}
	// 检查合成后门板位置 分别是上下不对门缝  左右不对门缝
	// 还有上下54 的拉手

	for (int x = 0; x < door.size(); x++)
	{
		
		// 这里来判断54 拉手里地面高度1600 的基准向上向下
		for (int s = 0; s < ls.size(); s++)
		{
			double z1 = round(abs(door[x].maxp.z - ls[s].z));//在上
			double z2 = round(abs(door[x].minp.z - ls[s].z));//在下

			if (PointInBoxTangency(ls[s], door[x]))
			{
				if (z1 == 54 && door[x].minp.z > 1600)
				{
					CreateArrow(ls[s], dec01(door[x]), 0, 120, 100);
					CText(door[x].center, _T("特殊拉手,门板方向下!"),dec01(door[x]));
				}
				if (z2 == 54 && door[x].minp.z < 1600)
				{
					CreateArrow(ls[s], dec01(door[x]), 0, 120, 100);
					CText(door[x].center, _T("特殊拉手,门板方向上!"), dec01(door[x]));
				}
			}
		}
		//上下左右门缝未对齐!
		double mcz = 0.5;
		//CreateLine(door[x].maxp, door[x].minp, 2);
		for (int i = 0; i < door.size(); i++)
		{
			// 判断的是z 大小点差值位置在18以内的
			// 上下门缝不对齐
			//CreateLine(door[i].maxp, door[i].minp, 1);
			int mfx = dec01(door[x]);
			int mfx1 = dec01(door[i]);
			// y 方向 上下门缝不对齐
			if (abs(door[x].minp.z - door[i].maxp.z) < 18 &&
				mfx == mfx1 &&
				mfx == 1)
			{
				double cz = abs(door[x].maxp.y - door[i].maxp.y);
				double cz1 = abs(door[x].minp.y - door[i].minp.y);
				if ((cz<18 && cz>mcz) || (cz1<18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 1, _T("_上下门缝未对齐!"));
				}
			}
			// x 方向上下门缝不对齐
			if (abs(door[x].minp.z - door[i].maxp.z) < 18 &&
				mfx == mfx1 &&
				mfx == 0)
			{
				double cz = abs(door[x].maxp.x - door[i].maxp.x);
				double cz1 = abs(door[x].minp.x - door[i].minp.x);
				if ((cz<18 && cz>mcz) || (cz1<18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 1, _T("_上下门缝未对齐!"));
				}
			}
			// 判断 x 方向 门板左右 门缝不对齐
			if (abs(door[x].minp.x - door[i].maxp.x) < 18 &&
				mfx == mfx1 &&
				mfx == 0)
			{
				// 小点z轴高度差值
				double cz = abs(door[x].maxp.z - door[i].maxp.z);
				double cz1 = abs(door[x].minp.z - door[i].minp.z);
				if ((cz < 18 && cz>mcz) || (cz1 < 18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 0, _T("_左右门缝未对齐!"));
				}
			}
			// 判断 y 方向 门板左右 门缝不对齐
			if (abs(door[x].minp.y - door[i].maxp.y) < 18 &&
				mfx == mfx1 &&
				mfx == 1)
			{
				// 小点z轴高度差值
				double cz = abs(door[x].maxp.z - door[i].maxp.z);
				double cz1 = abs(door[x].minp.z - door[i].minp.z);
				if ((cz < 18 && cz>mcz) || (cz1 < 18 && cz1>mcz))
				{
					CLs(door[x], door[i], mfx, 0, _T("_左右门缝未对齐!"));
				}
			}
			// 门板最小 最大值差别不大， y 或者y 距离差6-18 区间
			if (mfx == mfx1 && mfx == 0)
			{
				double bcz = abs(door[x].maxp.x - door[i].minp.x);
				double xcz = abs(door[x].minp.x - door[i].maxp.x);
				double ycha = abs(door[x].maxp.y - door[i].maxp.y);
				double p1z = door[x].center.z< door[i].maxp.z&&door[x].center.z > door[i].minp.z;
				double p2z = door[i].center.z< door[x].maxp.z&&door[i].center.z > door[x].minp.z;

				if (((bcz < 18 && bcz>5) || (xcz < 18 && xcz>5))&&(p1z||p2z)&&ycha<20)
				{
					CLs(door[x], door[i], mfx, 0, _T("_门缝大于4!"));
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
					CLs(door[x], door[i], mfx, 0, _T("_门缝大于4!"));
				}
			}
				
		}
	}
}

// 临时检查三合一;
// 判断面域1边大于某数
bool judeRegion1(const rec3d &m1, double b1)
{
	bool k = false;
	if (m1.p1.x == m1.p2.x)
	{
		// yz面域
		double ymay = (m1.p1.y > m1.p2.y) ? m1.p1.y : m1.p2.y;
		double ymin = (m1.p1.y < m1.p2.y) ? m1.p1.y : m1.p2.y;
		//
		double zmay = (m1.p1.z > m1.p2.z) ? m1.p1.z : m1.p2.z;
		double zmin = (m1.p1.z < m1.p2.z) ? m1.p1.z : m1.p2.z;
		if (ymay - ymin > b1 || zmay - zmin > b1) k = true;

	}
	if (m1.p1.y == m1.p2.y)
	{
		// xz面域
		double xmax = (m1.p1.x > m1.p2.x) ? m1.p1.x : m1.p2.x;
		double xmin = (m1.p1.x < m1.p2.x) ? m1.p1.x : m1.p2.x;
		//
		double zmax = (m1.p1.z > m1.p2.z) ? m1.p1.z : m1.p2.z;
		double zmin = (m1.p1.z < m1.p2.z) ? m1.p1.z : m1.p2.z;
		if (xmax - xmin > b1 || zmax - zmin > b1) k = true;

	}
	if (m1.p1.z == m1.p2.z)
	{
		// xy面域
		double xmax = (m1.p1.x > m1.p2.x) ? m1.p1.x : m1.p2.x;
		double xmin = (m1.p1.x < m1.p2.x) ? m1.p1.x : m1.p2.x;
		//
		double ymax = (m1.p1.y > m1.p2.y) ? m1.p1.y : m1.p2.y;
		double ymin = (m1.p1.y < m1.p2.y) ? m1.p1.y : m1.p2.y;
		if (xmax - xmin > b1 || ymax - ymin > b1) k = true;


	}
	return k;
}

// 面相交判断
bool mxj(const rec3d &r1, const rec3d &r2, int fx)
{
	// 要分大小面 r1 小面
	// p1 创建数据时 传入的是小点
	// Fb 是板件封边距离 扩大面域
	double FB = 2;

	recmo m1;
	recmo m2;
	
	if (fx == 1) // 三面位置 1的时候位置左边点为 z
	{
		m1 = { r1.p1.x, r1.p1.y ,r1.p2.x, r1.p2.y };
		m2 = { r2.p1.x - FB, r2.p1.y - FB ,r2.p2.x + FB, r2.p2.y + FB };
	}
	if (fx == 2)// 2的时候位置左边点为 y
	{
		m1 = { r1.p1.x, r1.p1.z ,r1.p2.x, r1.p2.z };
		m2 = { r2.p1.x - FB, r2.p1.z - FB ,r2.p2.x + FB, r2.p2.z + FB };
	}
	if (fx == 3)// 3的时候位置左边点为 x
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
	// 要分大小面 r1 小面
	// p1 创建数据时 传入的是小点
	// Fb 是板件封边距离 扩大面域
	double FB = 2;

	recmo m1;
	recmo m2;

	if (fx == 1) // 三面位置 1的时候位置左边点为 z
	{
		m1 = { r1.p1.x, r1.p1.y ,r1.p2.x, r1.p2.y };
		m2 = { r2.p1.x - FB, r2.p1.y - FB ,r2.p2.x + FB, r2.p2.y + FB };
	}
	if (fx == 2)// 2的时候位置左边点为 y
	{
		m1 = { r1.p1.x, r1.p1.z ,r1.p2.x, r1.p2.z };
		m2 = { r2.p1.x - FB, r2.p1.z - FB ,r2.p2.x + FB, r2.p2.z + FB };
	}
	if (fx == 3)// 3的时候位置左边点为 x
	{
		m1 = { r1.p1.y, r1.p1.z ,r1.p2.y, r1.p2.z };
		m2 = { r2.p1.y - FB, r2.p1.z - FB ,r2.p2.y + FB, r2.p2.z + FB };
	}
	bool s2 = MandM(m1, m2,-3);
	// 中点是否在面域内
	double center_x = (m1.x1 + m1.x11)/2;
	double center_y = (m1.y2 + m1.y22)/2;
	if (s2&&
		r1.id!=r2.id
		&&r1.zx == r2.zx&&r1.zx == fx && abs(r1.dd - r2.dd) < FB)
	
		return true;
	return false;
}

// 剔除面
bool tcm(const rec3d &r1, int fx)
{
	recmo m1;

	if (fx == 1) // 三面位置 1的时候位置左边点为 z
	{
		m1 = { r1.p1.x, r1.p1.y ,r1.p2.x, r1.p2.y };
		
	}
	if (fx == 2)// 2的时候位置左边点为 y
	{
		m1 = { r1.p1.x, r1.p1.z ,r1.p2.x, r1.p2.z };
		
	}
	if (fx == 3)// 3的时候位置左边点为 x
	{
		m1 = { r1.p1.y, r1.p1.z ,r1.p2.y, r1.p2.z };
	}
	double w1 = abs(m1.x1 - m1.x11);
	double w2 = abs(m1.y2 - m1.y22);
	if (w1 < 70 && w2 < 70 && r1.Layer == Layer_skt) return true;
	return false;
}
// 剔除实体
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

// 创建三合一实体
EntBox creatorE(const rec3d &p1)
{
	EntBox np;
	np.id = p1.id;
	// 大小点xy 规律是？ 小点xy变大
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
	std::vector<rec3d> pz18m; // 所有有碰撞的18面集合
	std::vector<rec3d> km18array; // 所有有孔位的18面集合
	std::vector<rec3d> shys; // 所有有孔位的18面集合

	
	
	for (int i = 0; i < marray.size(); i++)
	{
		/*CreateLine(marray[i].m18a.p1, marray[i].m18a.p2, 2);
		CreateLine(marray[i].m18b.p1, marray[i].m18b.p2, 2);
		CreateLine(marray[i].m18c.p1, marray[i].m18c.p2, 2);
		CreateLine(marray[i].m18d.p1, marray[i].m18.p2, 2);*/

		// 生成有孔位面18的集合
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

		// 生成有碰撞的集合
		for (int j = 0; j < marray.size(); j++)
		{
			// 分隔4 次
			if (marray[j].me18a.Layer!=Layer_beib)
			{
				if (newmim(marray[i].m18a, marray[j].me18a, marray[i].m18a.zx))
					pz18m.push_back(marray[i].m18a);

				if (newmim(marray[i].m18a, marray[j].me18b, marray[i].m18a.zx))
					pz18m.push_back(marray[i].m18a);
				// 分隔
				if (newmim(marray[i].m18b, marray[j].me18a, marray[i].m18b.zx))
					pz18m.push_back(marray[i].m18b);
				if (newmim(marray[i].m18b, marray[j].me18b, marray[i].m18b.zx))
					pz18m.push_back(marray[i].m18b);
				// 分隔
				if (newmim(marray[i].m18c, marray[j].me18a, marray[i].m18c.zx))
					pz18m.push_back(marray[i].m18c);
				if (newmim(marray[i].m18c, marray[j].me18b, marray[i].m18c.zx))
					pz18m.push_back(marray[i].m18c);
				// 分隔
				if (newmim(marray[i].m18d, marray[j].me18a, marray[i].m18d.zx))
					pz18m.push_back(marray[i].m18d);
				if (newmim(marray[i].m18d, marray[j].me18b, marray[i].m18d.zx))
					pz18m.push_back(marray[i].m18d);
			}
		}
	}

	// 三合一膨胀系数 
	// 画出漏孔的三合一面
	double pzxs = 3.5;
	//acutPrintf(_T("\n碰撞面数量:%d"), pz18m.size());
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
			// 三合一膨胀
			if (BoxIntersectBox2(ExspansionEnt(shym[j], pzxs),entp)|| tcbox(entp)) break;
			if (j==shym.size()-1)
			{
				CreateLine(pz18m[i].p1, pz18m[i].p2, 2);
				CreateBox(LineCenter(pz18m[i].p1, pz18m[i].p2), 30, 2);
			}
		}
	}
	// 检查孔位碰撞

	// 去重
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
				
				// 检查撞孔位， 方法是建立竖连接组的box, 查询与它相交的横连接
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
						/*CText(shyh[j].center, _T("撞孔了!"), 0);*/
						sa = 1;
					}
					
				}
				if (sa == 1)
				{
					CText(np.minp, _T("撞孔了!"), 0);
					CText(np.minp, _T("撞孔了!"), 1);
				}
			}
			
		}
		
		//km18array[i].id
		//if (abs(km18array[i].p1.z - km18array[i].p2.z)<1)
		//{

		//	// 判断方向
		//	acutPrintf(_T("\n %.2f"), km18array[i].p1.x - km18array[i].p2.x);
		//	CreateLine(km18array[i].p1, km18array[i].p2, 1);
		//}
		
	}
	
	

}

// 0 正规6面体
// 1 圆弧板件
// 2 双面圆弧

int isbox(AcDbObjectId &id)
{
	AcDbEntity *pEnt = NULL;

	// 获取实体
	if (acdbOpenObject(pEnt, id, AcDb::kForRead) == Acad::eOk)
	{
		//AcBrBrep brepEnt;

		//brepEnt.set(*pEnt);

		//acutPrintf(_T("st"));

		////创建一个面的遍历器

		//AcBr::ErrorStatus returnValue = AcBr::eOk;

		//AcBrBrepEdgeTraverser  brepEdgeTrav;

		//returnValue = brepEdgeTrav.setBrep(brepEnt);
		AcBrBrep brepEnt;

		brepEnt.set(*pEnt);

		//创建一个面的遍历器

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
							CText(pCylinder->origin(), _T("此圆弧板 R 半径不一致!"), 0);
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

					acutPrintf(TEXT("\n圆柱面的信息:"));

					acutPrintf(TEXT("\n半径:%.2f"), pCylinder->radius());

					AcGePoint3d center = pCylinder->origin();

					acutPrintf(TEXT("\n中心点：(%.2f,%2.f,%.2f)"), center.x, center.y, center.z);

					AcGeVector3d axis = pCylinder->axisOfSymmetry();

					acutPrintf(TEXT("\n轴线向量:(%.2f,%2.f,%.2f)"), axis.x, axis.y, axis.z);
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
	//acutPrintf(_T("\n 打不开盒子2?"));
	return 999;
}
// 判断是否是圆弧板 并且检查单板件R 半径差值小于10
RBord getRBord(AcDbObjectId &id)
{
	AcDbEntity *pEnt = NULL;
	RBord bb;
	// 获取实体
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

					acutPrintf(TEXT("\n圆柱面的信息:"));

					acutPrintf(TEXT("\n半径:%.2f"), pCylinder->radius());

					AcGePoint3d center = pCylinder->origin();

					acutPrintf(TEXT("\n中心点：(%.2f,%2.f,%.2f)"), center.x, center.y, center.z);

					AcGeVector3d axis = pCylinder->axisOfSymmetry();

					acutPrintf(TEXT("\n轴线向量:(%.2f,%2.f,%.2f)"), axis.x, axis.y, axis.z);
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
	acutPrintf(_T("\n 打不开盒子1?"));
	return bb;
}


void testbref(AcDbObjectId &id)
{
	AcDbEntity *pEnt = NULL;

	// 获取实体
	if (acdbOpenObject(pEnt, id, AcDb::kForRead) == Acad::eOk)
	{
		//AcBrBrep brepEnt;

		//brepEnt.set(*pEnt);

		//acutPrintf(_T("st"));

		////创建一个面的遍历器

		//AcBr::ErrorStatus returnValue = AcBr::eOk;

		//AcBrBrepEdgeTraverser  brepEdgeTrav;

		//returnValue = brepEdgeTrav.setBrep(brepEnt);
		AcBrBrep brepEnt;

		brepEnt.set(*pEnt);

		//创建一个面的遍历器

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

						//acutPrintf(TEXT("\n圆柱面的信息:"));

						acutPrintf(TEXT("\n 圆弧半径半径:%.2f, %d"), pCylinder->radius(),id);

						/*AcGePoint3d center = pCylinder->origin();

						acutPrintf(TEXT("\n中心点：(%.2f,%2.f,%.2f)"), center.x, center.y, center.z);

						AcGeVector3d axis = pCylinder->axisOfSymmetry();

						acutPrintf(TEXT("\n轴线向量:(%.2f,%2.f,%.2f)"), axis.x, axis.y, axis.z);*/

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
	//建立一个BREP对象
}

// 活动层板拉槽
bool testhclc(const EntBox &p1)
{
	std::string huoc = "72IMOSXD01_IM_SHELF_INT";
	
	// p1是活动层板
	if (p1.Layer == huoc && gsst(p1))
	{
		return true;
	}
	return false;

}

// 竖向三合一在同一块板件内合成一个新的box