#pragma once
#include "name.h"
#include <vector>
#include "Database.h"
//#include "StdAfx.h"
// 判断两点位置相同

struct EntBox
{
	AcGePoint3d minp;// 小
	AcGePoint3d maxp;// 大
	AcGePoint3d center;
	std::string Layer;
	std::string Type;
	AcDbObjectId id;
	double volume;
};

struct TEntBox
{
	EntBox sx;
	EntBox zy;
	int fx;
	
};

struct RBord
{
	// 状态判断
	
	double r;
	AcGePoint3d center;
};

struct MyMj
{
	// 相交方式分为1上下，0左右
	int xjfs;
	double minValue;
	int fx; // 门铰平行面
	TEntBox p1;

};
struct DrLs
{
	EntBox DoorDt;
	std::vector<AcGePoint3d> lspoint;
};


// 记住要改的 三合一面
struct rec3d
{
	AcDbObjectId id;
	AcGePoint3d p1;
	AcGePoint3d p2;
	// dd 是位置数，如正面 max.y =dd, 顶面 max.z=dd
	double dd;
	int zx;
	std::string Layer;
};
struct mianAk
{
	rec3d mian;
	int ji;
};
//
bool EquaPoint(const AcGePoint3d &p1, const AcGePoint3d &p2);
bool isLskwf(const EntBox &p1);
TEntBox CreaterMJ(const EntBox &mj);
bool maxbord(const EntBox &p1);
bool bigfun2c(const EntBox &p1, int wd = 60);
// 盒子相切
bool BoxTangencyBox(const EntBox &b1, const EntBox &b2);
// 盒子相交2 这个是全面的
bool BoxIntersectBox(const EntBox &b1, const EntBox &b2, double pz);
bool PointInBoxTangency(const AcGePoint3d &p1, const EntBox &box);
// 点与盒子相切
bool PointBoxTangency(const AcGePoint3d &p1, const EntBox &box, double expand = 0.1);
// 指定长款
bool FiltrationEnt(const EntBox &p1, int d1, int d2);
bool pointdpoint(AcGePoint3d p1, AcGePoint3d p2);

AcGePoint3d LineCenter(const AcGePoint3d &p1, const AcGePoint3d &p2);
// 浮点数相等
bool DoubleEq(double n1, double n2, double chaz=0.1);
// 膨胀实体
EntBox ExspansionEnt(const EntBox &p1, double greater=2.0);
int dec01(const EntBox &p1);
// 盒子相交2 这个是全面的
bool BoxIntersectBox2(const EntBox &b1, const EntBox &b2, double pz=0.0);
// 面与面相交
bool MandM( recmo &m1, recmo &m2, double pz);
//// 是孔位
bool isHole(const EntBox &p1);
//// 是灯槽
bool isLamp(const EntBox &p1);
//// 是门铰
bool isMj(const EntBox &p1);
// 非门板内相交检测
bool Intervene(const EntBox &p1, const EntBox &p2);
// 门板相交
bool DoorIntervene(const EntBox &p1, const EntBox &p2);
// 门，板件相交
bool DoorBIntervene(const EntBox &p1, const EntBox &p2);
// 随机传入两个实体 判断他们是实体并且在同一个位置
bool judeMore(const EntBox &p1, const EntBox &p2);
// 背板重叠
bool MoreBack(const EntBox &p1, const EntBox &p2, std::vector<EntBox> &adlist);
//bool MoreBack(EntBox &p1, EntBox &p2);

bool isbb(const EntBox &p1);

int decXYZ(const EntBox &p1);

bool PointInBoxTangency(const AcGePoint3d &p1, const EntBox &box);

int TestYt2(const EntBox &p1, const EntBox &p2);

int TestMj(const TEntBox &newMj, const EntBox &p2);

int dec9mmBB(const EntBox &bb9, const EntBox &adlist);

int JudgeBord(const EntBox &p1);

void TestMjLs(std::vector<EntBox> &door, std::vector<AcGePoint3d> &ls, std::vector<MyMj> &mj);

struct RecBox
{
	rec3d m18a;
	rec3d m18b;
	rec3d m18c;
	rec3d m18d;
	rec3d me18a;
	rec3d me18b;
};
// 拉直器
bool islzq(const EntBox &p1);

void TestShyBord(std::vector<RecBox> &marray, std::vector<EntBox> &shym,
	std::vector<rec3d> &shym2, std::vector<EntBox> &shyh);

void testbref(AcDbObjectId &id);
// 
int isbox(AcDbObjectId &id);

RBord getRBord(AcDbObjectId &id);

// 要剔除的实体，收口条和抽侧条
bool tcbord(const EntBox &p1);
bool testhclc(const EntBox &p1);
// 三合一判断
int isShy(const EntBox &p1);
// 判断是玻璃层板
bool isblcb(const EntBox &p1);
EntBox tsExspansionEnt(const EntBox &p1, double greater);