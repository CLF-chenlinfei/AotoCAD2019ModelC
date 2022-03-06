#pragma once
#include "name.h"
//#include "StdAfx.h"
// 判断两点位置相同
bool pointdpoint(AcGePoint3d p1, AcGePoint3d p2);

AcGePoint3d LineCenter(const AcGePoint3d &p1, const AcGePoint3d &p2);
// 浮点数相等
bool DoubleEq(double n1, double n2, double chaz=0.1);
// 膨胀实体
EntBox ExspansionEnt(EntBox &p1, double greater=2.0);

// 盒子相交2 这个是全面的
bool BoxIntersectBox2(EntBox &b1, EntBox &b2, double pz=0.0);
// 面与面相交
bool MandM( recmo &m1, recmo &m2, double pz);
//// 是孔位
bool isHole(EntBox &p1);
//// 是灯槽
bool isLamp(EntBox &p1);

// 非门板内相交检测
bool Intervene(EntBox &p1, EntBox &p2);
// 门板相交
bool DoorIntervene(EntBox &p1, EntBox &p2);
// 门，板件相交
bool DoorBIntervene(EntBox &p1, EntBox &p2);
// 随机传入两个实体 判断他们是实体并且在同一个位置
bool judeMore(EntBox &p1, EntBox &p2);


