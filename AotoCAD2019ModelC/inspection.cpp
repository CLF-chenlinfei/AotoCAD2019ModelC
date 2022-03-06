#include "stdafx.h"
#include "inspection.h"
#include "alg.h"
#include "CreateSome.h"
#include <vector>
#include <algorithm>
//#include "name.h"

// ��ס�ܼ�������ľ�����Ҫ�����µ�ѭ��

void CkModel(std::vector<EntBox>ent) 
{
	// ʵ������������2����������
	if (!ent.empty())
	{
		EntBox new1;
		for (int i = 0; i < ent.size(); i++)
		{
			//�μ� JJJJJJ
			for (int j = 0; j < ent.size(); j++)
			{
				
				// 1.�жϿ�λ�ص��ཻ ��/�� ��һ
				if (judeMore(ent[i], ent[j]))
				{
					CreateArrow(ent[i].center, 0, 0, 110, 100);
					CreateArrow(ent[i].center, 1, 0, 110, 100);
					CText(ent[i].center, _T("�ؿ�?���?ײ��?"));
				}
				// 2.�жϵƲ����λ��ײ 
				if (BoxIntersectBox2(ExspansionEnt(ent[i]), ExspansionEnt(ent[j]))
					&& isLamp(ent[i]) && isHole(ent[j]))
				{

					Arrow3dXY(ent[j].center, 30, 111);
					CText(ent[j].center, _T("��ײ�Ʋ�?"));
				}
				// 3.������1 ֻ�ǹ���
				if (Intervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("����"));
				}
				// 4.������2 ������Ű�
				if (DoorBIntervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("����2"));
				}
				// 5.������3 ֻ���Ű�
				if (DoorIntervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("����3"));
				}
				// 6.���౳��
				// CText(bb.center, _T("X��:�����ص�!"));
				// CText(bb.center, _T("Y��:�����ص�!"));
				// 7.�����ͨ
				// 8.����Ž�(Ӧ�ð����Ž��������ཻ���Ž���������)
				// CText(p1.sx.center, _T("�Ž����������档"));
				// CText(p1.sx.center, _T("�Ž������ӣ�"));
				// CText(lsit1[j].p1.sx.center, _T("�����Ž¸�ֵ��һ��!"));
				// CText(errovec[i], _T("    __����δ����!"));
				// CText(bb9.minp, _T("�������?"));
				// CText(LineCenter(wdlist[x].center, ent[s].center), _T("�Ű巽����!"));
				// CText(LineCenter(wdlist[x].center, ent[s].center), _T("�Ű巽����!"));
				// CText(LineCenter(wdlist[x].center, wdlist[i].center), _T("_�����ŷ�δ����!"));
				//  ͬ�����Ž¸�ֵ Ӧ���Ǻϳɺ���Ű��ǰ�Ķ���
			}
			// 9.����Ű����ּ��ϱ�
			// 10.���ϳɺ���Ű���ŷ�
			// 11.����Ű����Ž¹�ϵ�� ���Ž����Ű��� ����Сֵ����ʱ�ж�Ϊ���ݲ�2
			// 12.����Ž�����
			// 291-900   x2
			// 901-1600  x3
			// 1601-2100 x4
			// 2101-2400 x5
			// 2401-2700 x6
			// 13.�������λ��
			// 14.9mm��δ��ۼ�⣬ע��װ�κ�
			// 15.����ŷ죨�����£������� ����Ӧ���ǲ����룬���Ҳ�5-18��
			// CText(wdlist[i].center, _T("X_�ŷ�5-18!"));
			// CText(wdlist[i].center, _T("Y_5-18�ŷ�!"));
			// 16.��λ���
			// CText(LineCenter(rec18data[i].p1, rec18data[i].p2), _T("����ߴ����"));

		}
	}
	
	acutPrintf(_T("\n Hello yangyang ������:%d !"), ent.size()*ent.size());
}
