#pragma once
#include "name.h"
#include <vector>
#include "Database.h"
//#include "StdAfx.h"
// �ж�����λ����ͬ

struct EntBox
{
	AcGePoint3d minp;// С
	AcGePoint3d maxp;// ��
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

struct MyMj
{
	// �ཻ��ʽ��Ϊ1���£�0����
	int xjfs;
	double minValue;
	int fx; // �Ž�ƽ����
	TEntBox p1;

};
struct DrLs
{
	EntBox DoorDt;
	std::vector<AcGePoint3d> lspoint;
};

bool isLskwf(const EntBox &p1);
TEntBox CreaterMJ(const EntBox &mj);
bool maxbord(const EntBox &p1);
bool bigfun2c(const EntBox &p1, int wd = 60);
// ��������
bool BoxTangencyBox(const EntBox &b1, const EntBox &b2);
// �����ཻ2 �����ȫ���
bool BoxIntersectBox(const EntBox &b1, const EntBox &b2, double pz);
bool PointInBoxTangency(const AcGePoint3d &p1, const EntBox &box);
// �����������
bool PointBoxTangency(const AcGePoint3d &p1, const EntBox &box, double expand = 0.1);
// ָ������
bool FiltrationEnt(const EntBox &p1, int d1, int d2);
bool pointdpoint(AcGePoint3d p1, AcGePoint3d p2);

AcGePoint3d LineCenter(const AcGePoint3d &p1, const AcGePoint3d &p2);
// ���������
bool DoubleEq(double n1, double n2, double chaz=0.1);
// ����ʵ��
EntBox ExspansionEnt(const EntBox &p1, double greater=2.0);
int dec01(const EntBox &p1);
// �����ཻ2 �����ȫ���
bool BoxIntersectBox2(const EntBox &b1, const EntBox &b2, double pz=0.0);
// �������ཻ
bool MandM( recmo &m1, recmo &m2, double pz);
//// �ǿ�λ
bool isHole(const EntBox &p1);
//// �ǵƲ�
bool isLamp(const EntBox &p1);
//// ���Ž�
bool isMj(const EntBox &p1);
// ���Ű����ཻ���
bool Intervene(const EntBox &p1, const EntBox &p2);
// �Ű��ཻ
bool DoorIntervene(const EntBox &p1, const EntBox &p2);
// �ţ�����ཻ
bool DoorBIntervene(const EntBox &p1, const EntBox &p2);
// �����������ʵ�� �ж�������ʵ�岢����ͬһ��λ��
bool judeMore(const EntBox &p1, const EntBox &p2);
// �����ص�
bool MoreBack(const EntBox &p1, const EntBox &p2, std::vector<EntBox> &adlist);
//bool MoreBack(EntBox &p1, EntBox &p2);

bool isbb(const EntBox &p1);

int decXYZ(const EntBox &p1);

bool PointInBoxTangency(const AcGePoint3d &p1, const EntBox &box);

int TestYt2(const EntBox &p1, const EntBox &p2);

int TestMj(const TEntBox &newMj, const EntBox &p2);

int dec9mmBB(const EntBox &bb9, const EntBox &adlist);

void TestMjLs(std::vector<EntBox> &door, std::vector<AcGePoint3d> &ls, std::vector<MyMj> &mj);