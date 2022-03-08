#include "stdafx.h"
#include "inspection.h"
#include "alg.h"
#include "CreateSome.h"
#include <vector>
#include <algorithm>
//#include "name.h"

#define pass (void)0
using namespace std;

// ��ס�ܼ�������ľ�����Ҫ�����µ�ѭ��

void CkModel(vector<EntBox>ent) 
{
	vector<EntBox> ADDdoorArray; // �ϳɺ���Ű�
	vector<int> v; //���鼯�ϣ������ڼ��������
	vector<MyMj> MjArray;
	vector<AcGePoint3d> DrLsArry;
	// ʵ������������2����������
	if (!ent.empty())
	{
		for (int i = 0; i < ent.size(); i++)
		{
			// 7.�����ͨ
			bool ytz = false;// ��ͨ�����Ƿ�
			bool yty = false;
			bool isyt = false;
			bool iisdoor = ent[i].Layer == Layer_door && ent[i].Type == Solid
				&& std::find(v.begin(), v.end(), i) == v.end();
			EntBox newi = ent[i];
			// ����Ž�
			
			bool bmj = isMj(ent[i]);
			TEntBox tts = CreaterMJ(ent[i]);
			bool psx = false;
			bool pzy = false;
			MyMj np1;
			np1.p1 = tts;
			np1.fx = tts.fx;
			// ������С�ཻ�㣿
			std::vector<double> sxv;
			std::vector<double> zyv;

			for (int j = 0; j < ent.size(); j++)
			{
				// 1.�жϿ�λ�ص��ཻ ��/�� ��һ
				if (judeMore(ent[i], ent[j]))
				{
					CreateArrow(ent[i].center, 0, 0, 110, 100);
					CreateArrow(ent[i].center, 1, 0, 110, 100);
					CText(ent[i].center, _T("�ؿ�?���?ײ��?"),0);
				}
				// 2.�жϵƲ����λ��ײ 
				if (BoxIntersectBox2(ExspansionEnt(ent[i]), ExspansionEnt(ent[j]))
					&& isLamp(ent[i]) && isHole(ent[j]), 0)
				{

					Arrow3dXY(ent[j].center, 30, 111);
					CText(ent[j].center, _T("��ײ�Ʋ�?"), 0);
				}
				// 3.������1 ֻ�ǹ���
				if (Intervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("����"), 0);
				}
				// 4.������2 ������Ű�
				if (DoorBIntervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("����2"), 0);
				}
				// 5.������3 ֻ���Ű�
				if (DoorIntervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("����3"), 0);
				}
				// 6.���౳��
				if (ent[i].Layer == Layer_beib && ent[j].Layer == Layer_beib)
				{
					if (MoreBack(ent[i], ent[j], ent))
					{
						CreateArrow(ent[j].center, decXYZ(ent[j])-1, 0, 1, 150);
						CreateArrow(ent[i].center, decXYZ(ent[i])-1, 0, 1, 150);
						CText(ent[j].center, _T("�����ص�!"), 0);
					}
				}
				// 7.�����ͨ
				int yy = TestYt2(ent[i], ent[j]);
				if (yy!=0)
				{
					isyt = true;
					if (yy == 1) yty = true;
					if (yy == 2) ytz = true;
					
				}
				
				bool jisdoor = ent[j].Layer == Layer_door && ent[j].Type == Solid;
				if (iisdoor)
				{
					
					bool tc = FiltrationEnt(ent[i], 12, 10);
					bool tc2 = FiltrationEnt(ent[j], 12, 10);
					if (!tc && !tc2&&i != j && jisdoor && 
						(BoxTangencyBox(newi, ent[j]) || BoxIntersectBox(newi, ent[j], 0)))
					{
						v.push_back(i);
						v.push_back(j);
						newi.maxp = AcGePoint3d(max(newi.maxp.x, ent[j].maxp.x), max(newi.maxp.y, ent[j].maxp.y), max(newi.maxp.z, ent[j].maxp.z));
						newi.minp = AcGePoint3d(min(newi.minp.x, ent[j].minp.x), min(newi.minp.y, ent[j].minp.y), min(newi.minp.z, ent[j].minp.z));
					}
				}
				// ���һ���ж� jֵ���¸˼���һ����!
				if (j==ent.size()-1)
				{
					// ����������ͨ
					if (isyt && !yty)
					{
						CreateArrow(ent[i].maxp, 0, 0, 110, 100);
						CreateArrow(ent[i].maxp, 1, 0, 110, 100);
						CText(ent[i].center, _T("��ͨ�ߴ����!"), 0);
					}
					if (isyt && !ytz)
					{
						CreateArrow(ent[i].minp, 0, 0, 110, 100);
						CreateArrow(ent[i].minp, 1, 0, 110, 100);
						CText(ent[i].center, _T("��ͨ�ߴ����!"), 0);
					}
				}
				// �Ű�
				if (bmj)
				{
					int n = TestMj(tts, ent[j]);
					if (n == 1) 
					{
						if (tts.fx == 0)
						{
							sxv.push_back(abs(ent[j].minp.z - tts.sx.center.z));
						}
						psx = true;
					}
					

					if (n == 2)
					{
						if (tts.fx == 0)
						{
							zyv.push_back(abs(ent[j].minp.x - tts.sx.center.x));
							zyv.push_back(abs(ent[j].maxp.x - tts.sx.center.x));
						}
						else
						{
							zyv.push_back(abs(ent[j].minp.y - tts.sx.center.y));
							zyv.push_back(abs(ent[j].maxp.y - tts.sx.center.y));
						}
						pzy = true;
					}

				}
			}
			
			// �Ž´��� �Լ������Ž´���������С����
			if (bmj)
			{
				// �ҷ���һ������ʵ�л���ɺ������
				if (psx&&pzy)
				{
					CreateArrow(tts.sx.center, tts.fx, 0, 110, 150);
					CText(tts.sx.center, _T("�Ž����������档"), tts.fx);
					np1.minValue = 0;
					np1.xjfs = 3;
				}
				if (!psx && !pzy)
				{
					CreateArrow(tts.sx.center, tts.fx, 0, 110, 150);
					CText(tts.sx.center, _T("�Ž������ӣ�"), dec01(ent[i]));

				}
				if (psx && !pzy)
				{
					if (!sxv.empty())
					{
						np1.minValue = *(min_element(sxv.begin(), sxv.end()));
						np1.xjfs = 1;
					}
				}
				if (!psx && pzy)
				{
					if (!zyv.empty())
					{
						np1.minValue = *(min_element(zyv.begin(), zyv.end()));
						np1.xjfs = 0;
					}
				}
				MjArray.push_back(np1);
			}
			// ���ɺϳ��Ű弯��
			if (bigfun2c(newi) && iisdoor)ADDdoorArray.push_back(newi);
			// ���ּ�������
			if (isLskwf(ent[i]))DrLsArry.push_back(ent[i].center);
			// 12.����Ž�����
			// 291-900   x2
			// 901-1600  x3
			// 1601-2100 x4
			// 2101-2400 x5
			// 2401-2700 x6
			// 14.9mm��δ��ۼ�⣬ע��װ�κ� 
			// 16.��λ���
			// CText(LineCenter(rec18data[i].p1, rec18data[i].p2), _T("����ߴ����"));

		}
	}
	if (!DrLsArry.empty())
	{
		for (int i=0; i<DrLsArry.size();i++)
		{
			/*Arrow3dXY(DrLsArry[i], 100, 100);*/

		}
		acutPrintf(_T("\n ls number:%d !"), DrLsArry.size());
	}
	if (!ADDdoorArray.empty())
	{
		// �����Ŀ��ͬ���Ž�λ�ò�ͬ
		// �����ŷ�5-18 �Ű����¶��� ���Ҷ���ҲҪ��
		TestMjLs(ADDdoorArray, DrLsArry, MjArray);
		//acutPrintf(_T("\n mj number:%d !"), MjArray.size());
	}
	
	acutPrintf(_T("\n Hello yangyang ������:%d !"), ent.size()*ent.size());
}
