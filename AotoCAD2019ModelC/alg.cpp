#include "StdAfx.h"
#include "alg.h"
#include "name.h"


// 检查xy 顶面方向 板件有一项符合标准
bool dec_xy(EntBox &p1)
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
bool bigfun2c(EntBox &p1, int wd)
{
	// 两边大于某数
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y || x && z || y && z)return true;
	return false;

}

// 三边大于某数
bool bigfun3c(EntBox &p1, int wd)
{
	// 两边大于某数
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x&&y&&z)return true;
	return false;

}

// 1边大于某数
bool bigfun1c(EntBox &p1, int wd)
{
	// 两边大于某数
	bool x = round(p1.maxp.x - p1.minp.x) > wd;
	bool y = round(p1.maxp.y - p1.minp.y) > wd;
	bool z = round(p1.maxp.z - p1.minp.z) > wd;
	if (x || y || z)return true;
	return false;

}

// 非门板的 大于60*299 的板件
bool gsst(EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && 
		p1.Layer != Layer_door && p1.Type == Solid)return true;
	return false;

}
// 包含门板 大于60*299 的板件
bool gsdoor(EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer != Layer_wjls && p1.Type == Solid)return true;
	return false;
}
// 用于干涉检测的门板判断
// 包含门板 大于60*299 的板件
bool isdoor(EntBox &p1)
{

	bool a1 = bigfun1c(p1, 299);
	bool a2 = bigfun2c(p1, 61);
	bool a3 = bigfun3c(p1, 17);
	if (a1&&a2&&a3&&p1.Layer == Layer_door && p1.Type == Solid)return true;
	return false;
}
// 膨胀实体
EntBox ExspansionEnt(EntBox &p1, double greater)
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
bool BoxIntersectBox2(EntBox &b1, EntBox &b2, double pz)
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
bool pointdpoint(AcGePoint3d p1, AcGePoint3d p2)
{
	bool x = DoubleEq(p1.x, p2.x);
	bool y = DoubleEq(p1.y, p2.y);
	bool z = DoubleEq(p1.z, p2.z);
	if (x&&y&&z) return true;
	return false;
}
//
// 判断点位重叠
bool judeMore(EntBox &p1, EntBox &p2)
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
bool isLamp(EntBox &p1)
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
bool isHole(EntBox &p1)
{


	if (p1.Layer != Layer_door && p1.Type == Solid && (abs(p1.volume - 1856) < 1 || abs(p1.volume - 300) < 1)) return true;
	return false;

}
// 板件相交
bool Intervene(EntBox &p1, EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -1.0) && gsst(p1) && gsst(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// 门，板件相交
bool DoorBIntervene(EntBox &p1, EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, -6.0) && gsdoor(p1) && gsdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id)
		return true;
	return false;
}

// 门板相交
bool DoorIntervene(EntBox &p1, EntBox &p2)
{
	if (BoxIntersectBox2(p1, p2, 3.0) && isdoor(p1) && isdoor(p2) &&
		!EquaPoint(p1.maxp, p2.maxp) && p1.id != p2.id
		&& dec_xy(p1) && dec_xy(p2))
		return true;
	return false;
}