#pragma once
#include "name.h"
//#include "StdAfx.h"
// �ж�����λ����ͬ
bool pointdpoint(AcGePoint3d p1, AcGePoint3d p2);

AcGePoint3d LineCenter(const AcGePoint3d &p1, const AcGePoint3d &p2);
// ���������
bool DoubleEq(double n1, double n2, double chaz=0.1);
// ����ʵ��
EntBox ExspansionEnt(EntBox &p1, double greater=2.0);

// �����ཻ2 �����ȫ���
bool BoxIntersectBox2(EntBox &b1, EntBox &b2, double pz=0.0);
// �������ཻ
bool MandM( recmo &m1, recmo &m2, double pz);
//// �ǿ�λ
bool isHole(EntBox &p1);
//// �ǵƲ�
bool isLamp(EntBox &p1);

// ���Ű����ཻ���
bool Intervene(EntBox &p1, EntBox &p2);
// �Ű��ཻ
bool DoorIntervene(EntBox &p1, EntBox &p2);
// �ţ�����ཻ
bool DoorBIntervene(EntBox &p1, EntBox &p2);
// �����������ʵ�� �ж�������ʵ�岢����ͬһ��λ��
bool judeMore(EntBox &p1, EntBox &p2);


