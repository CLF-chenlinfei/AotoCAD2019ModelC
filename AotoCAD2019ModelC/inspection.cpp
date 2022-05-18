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
	vector<EntBox> SHYArray; // �ϳɺ���Ű�
	vector<EntBox> SHYArrayH; // ��������һ
	//vector<EntBox> SHYArrayS; // ��������һ
	vector<int> v; //���鼯�ϣ������ڼ��������
	vector<MyMj> MjArray;
	vector<AcGePoint3d> DrLsArry; // �������ĵ㼯
	std::vector<rec3d> shym;
	std::vector<EntBox> entshy; // ����һʵ��
	std::vector<RBord> RArray; // Բ���������Ƚ�
	std::vector<EntBox> LzqArray; // ��ֱ����
	std::vector<EntBox> LsArray; // ���ֿ�λ��

	// ������һ���
	vector<RecBox> Marray;
	if (!ent.empty())
	{
		for (int i = 0; i < ent.size(); i++)
		{
			//��ȡ��������һ
			int int3 = isShy(ent[i]);
			/*if (int3 == 1)
			{
				EntBox nshy = ent[i];
				nshy.maxp.z = ent[i].maxp.z+10;
				nshy.minp.z = ent[i].minp.z-10;
				SHYArrayS.push_back(nshy);
			}*/
			if (int3 == 2)
			{
				EntBox nshy = ent[i];
				nshy.maxp.x = ent[i].maxp.x + 5;
				nshy.minp.x = ent[i].minp.x - 5;
				SHYArrayH.push_back(nshy);
			}
			if (int3 == 3)
			{
				EntBox nshy = ent[i];
				nshy.maxp.y = ent[i].maxp.y + 5;
				nshy.minp.y = ent[i].minp.y - 5;
				SHYArrayH.push_back(nshy);
			}

			// ����ǻ�ȡʵ��
			if (ent[i].Layer != Layer_door && 
				((ent[i].volume > 999 && ent[i].volume < 1001) || 
				(ent[i].volume > 299 && ent[i].volume < 301)) && ent[i].Type == Solid)
				entshy.push_back(ent[i]);
			// ����ǻ�����һȡ��
			if (ent[i].Layer != Layer_door && ((ent[i].volume > 1855 && ent[i].volume < 1857) || (ent[i].volume > 299 && ent[i].volume < 301)) &&ent[i].Type == Solid)
			{
				if ((int)(ent[i].maxp.x - ent[i].minp.x) == 29 || (int)(ent[i].maxp.x - ent[i].minp.x) == 12)
				{
					rec3d rc5 = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].maxp.z),ent[i].minp.x,3,ent[i].Layer };
					rec3d rc6 = { ent[i].id,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.x,3,ent[i].Layer };
					shym.push_back(rc5);
					shym.push_back(rc6);
				}
				if ((int)(ent[i].maxp.y - ent[i].minp.y) == 29 || (int)(ent[i].maxp.y - ent[i].minp.y) == 12)
				{
					rec3d rc3 = { ent[i].id,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.y,2 ,ent[i].Layer };
					rec3d rc4 = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].minp.y,2 ,ent[i].Layer };
					shym.push_back(rc3);
					shym.push_back(rc4);
				}
				if ((int)(ent[i].maxp.z - ent[i].minp.z) == 29 || (int)(ent[i].maxp.z - ent[i].minp.z) == 12)
				{
					rec3d rc1 = { ent[i].id ,AcGePoint3d(ent[i].minp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].maxp,ent[i].maxp.z,1 ,ent[i].Layer };
					rec3d rc2 = { ent[i].id ,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].minp.z,1 ,ent[i].Layer };
					shym.push_back(rc1);
					shym.push_back(rc2);
				}
			}

			// ��ȡ18 ��18�� ����box
			int br18 = JudgeBord(ent[i]);
			if (br18 != 0)
			{
				RecBox p1;
				if (br18 == 1)
				{
					p1.m18a = { ent[i].id ,AcGePoint3d(ent[i].minp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].maxp,ent[i].maxp.z,1,ent[i].Layer };
					p1.m18b = { ent[i].id ,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].minp.z,1 ,ent[i].Layer };
					p1.m18c = { ent[i].id,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.y,2 ,ent[i].Layer };
					p1.m18d = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].minp.y,2 ,ent[i].Layer };
					p1.me18a = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].maxp.z),ent[i].minp.x,3 ,ent[i].Layer };
					p1.me18b = { ent[i].id,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.x,3,ent[i].Layer };
				}
				if (br18 == 2)
				{
					p1.m18a = { ent[i].id ,AcGePoint3d(ent[i].minp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].maxp,ent[i].maxp.z,1,ent[i].Layer };
					p1.m18b = { ent[i].id ,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].minp.z,1,ent[i].Layer };
					p1.me18a = { ent[i].id,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.y,2 ,ent[i].Layer };
					p1.me18b = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].minp.y,2,ent[i].Layer };
					p1.m18c = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].maxp.z),ent[i].minp.x,3,ent[i].Layer };
					p1.m18d = { ent[i].id,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.x,3,ent[i].Layer };
				}
				if (br18 == 3)
				{
					p1.me18a = { ent[i].id ,AcGePoint3d(ent[i].minp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].maxp,ent[i].maxp.z,1 ,ent[i].Layer };
					p1.me18b= { ent[i].id ,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].minp.z,1,ent[i].Layer };
					p1.m18c = { ent[i].id,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.y,2 ,ent[i].Layer };
					p1.m18d = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].maxp.z),ent[i].minp.y,2 ,ent[i].Layer };
					p1.m18a = { ent[i].id,ent[i].minp,AcGePoint3d(ent[i].minp.x, ent[i].maxp.y, ent[i].maxp.z),ent[i].minp.x,3 ,ent[i].Layer };
					p1.m18b = { ent[i].id,AcGePoint3d(ent[i].maxp.x, ent[i].minp.y, ent[i].minp.z),ent[i].maxp,ent[i].maxp.x,3 ,ent[i].Layer };
				}
				Marray.push_back(p1);
			}
			// �����ж�
			int ifx = dec01(ent[i]);
			// 7.�����ͨ����
			bool p1yt = ((int)(ent[i].maxp.x - ent[i].minp.x) == 15 || (int)(ent[i].maxp.y - ent[i].minp.y) == 15) &&
				ent[i].Layer == Layer_wjls && (int)(ent[i].maxp.z - ent[i].minp.z) == 30;
			bool ytz = false;// ��ͨ�����Ƿ�
			bool yty = false;
			bool isyt = false;
			// ����Ű����
			bool iisdoor = ent[i].Layer == Layer_door && ent[i].Type == Solid
				&& std::find(v.begin(), v.end(), i) == v.end();
			EntBox newi = ent[i];
			// ��������Ž�
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
			// 9mm����δ���
			bool on_offa = false; // ��㿪��
			bool on_offi = false;
			bool bb9mm = (round(ent[i].maxp.x - ent[i].minp.x) == 9 || round(ent[i].maxp.z - ent[i].minp.z) == 9 || round(ent[i].maxp.y - ent[i].minp.y) == 9)
				&& ent[i].Layer == Layer_beib && ent[i].Type == Solid&&bigfun2c(ent[i], 80);
			// ����ײ���ײ
			bool deng = isLamp(ent[i]);
			// Բ�����
			int irb = isbox(ent[i].id);
			if (irb==1)
			{
				RArray.push_back(getRBord(ent[i].id));

			}
			// �ж��ǲ������
			bool bl = isblcb(ent[i]);
			bool blz = false;
			bool bly = false;
			/*if (bl)
			{
				EntBox npp1 = tsExspansionEnt(ent[i], 5);
				CreateLine(npp1.maxp, npp1.minp, 3);
			}*/
			// ========= �ж�����ֱ�� =========
			
			if (islzq(ent[i]))
			{
				LzqArray.push_back(ent[i]);
			}
			// ========= �ж�������һñ =========
			bool isshym1 = round(ent[i].volume) == 3150
				&& round(ent[i].maxp.z - ent[i].minp.z) == 14;
			int duocenb = 0;
			
			// ========= �ж����� =========
			bool suo1 = round(ent[i].volume) == 58050 && ent[i].Layer=="72IMOSXD01_IM_PULL";
			int sn_number = 0;
			

			for (int j = 0; j < ent.size(); j++)
			{
				// ����Ƿ�ײ��
				if (suo1)
				{
					// ͳ�ƺ����ཻ��ʵ�����
					if (BoxIntersectBox2(ent[i],ent[j]))
					{
						sn_number += 1;
					}
				}
				// ����λ�ص�
				bool isshym2 = round(ent[j].volume) == 3150 &&
					round(ent[j].maxp.z - ent[j].minp.z)==14;
				if (isshym2&&isshym1)
				{
					
					if (abs(ent[i].maxp.z - ent[j].minp.z) < 0.1 ||
						abs(ent[i].minp.z - ent[j].maxp.z) < 0.1)
					{
						if (abs(ent[i].center.x - ent[j].center.x) < 0.1 ||
							abs(ent[i].center.y - ent[j].center.y) < 0.1)
						{
							CreateLine(ent[i].center, ent[j].center, 3);
							duocenb = 1;
							//CText(ent[j].center, _T("���ײ��!"), 0);
						}
					}
				}
				// ��鲣�����
				if (bl)
				{
					EntBox npp1 = tsExspansionEnt(ent[i], 5);
					if (PointInBoxTangency(npp1.maxp, ent[j])) blz = true;
					if (PointInBoxTangency(npp1.minp, ent[j])) bly = true;
				}
				// ��������
				if (bb9mm&&testhclc(ent[j]))
				{
					// ��ʴ�ཻ���
					if (BoxIntersectBox2(ExspansionEnt(ent[i], -2), ExspansionEnt(ent[j], -2)))
					{
						CreateArrow(ent[j].center, 0, 0, 110, 100);
						CreateArrow(ent[j].center, 1, 0, 110, 100);
						CText(ent[j].center, _T("������۴���!"), 0);
					}
				}
				
				// 1.�жϿ�λ�ص��ཻ ��/�� ��һ
				if (judeMore(ent[i], ent[j]))
				{
					CreateArrow(ent[i].center, 0, 0, 110, 100);
					CreateArrow(ent[i].center, 1, 0, 110, 100);
					CText(ent[i].center, _T("�ؿ�?���?ײ��?"),0);
				}
				// 2.�жϵƲ����λ��ײ 
				if (deng&&isHole(ent[j]))
				{
					if (BoxIntersectBox2(ExspansionEnt(ent[i],4), ExspansionEnt(ent[j])))
					{
						Arrow3dXY(ent[j].center, 30, 111);
						CText(ent[j].center, _T("��ײ�Ʋ�?"), 0);
					}
				}
				
				// 3.������1 ֻ�ǹ���
				// ���Բ���ǶȲ�һ��

				if (Intervene(ent[i], ent[j]))
				{
					// ��ʱֻ���box
					// �ж�ij��box
					if (isbox(ent[j].id)==0&&irb==0)
					{
						Arrow3dXY(ent[j].center, 30, 112);
						Arrow3dXY(ent[i].center, 30, 112);
						CreateLine(ent[j].center, ent[i].center, 112);
						CText(LineCenter(ent[j].center, ent[i].center), _T("�������"), 0);
					}			
				}
				// 4.������2 ������Ű�
				if (DoorBIntervene(ent[i], ent[j]))
				{
					// ��ʱֻ���box
					// �ж�ij��box
					if (isbox(ent[j].id)==0 && irb == 0)
					{
						Arrow3dXY(ent[j].center, 30, 112);
						Arrow3dXY(ent[i].center, 30, 112);
						CreateLine(ent[j].center, ent[i].center, 112);
						CText(LineCenter(ent[j].center, ent[i].center), _T("������Ű����"), 0);
					}
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
				if (p1yt)
				{
					int yy = TestYt2(ent[i], ent[j]);
					if (yy != 0)
					{
						isyt = true;
						if (yy == 1) yty = true;
						if (yy == 2) ytz = true;

					}
				}
				// 8.����Ű�
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
				// 9.9���뱳��δ��ۼ��
				if (bb9mm)
				{
					int aa = dec9mmBB(ent[i], ent[j]);
					if (aa == 1) on_offi = true;
					if (aa == 2) on_offa = true;
				}
			}

			// ͳ�����ཻ����
			if (sn_number>4)
			{
				CreateArrow(ent[i].center, 0, 0, 110, 100);
				CreateArrow(ent[i].center, 1, 0, 110, 100);
				CText(ent[i].center, _T("ײ��!"), 0);
				CText(ent[i].center, _T("ײ��!"), 1);
			}
			
			// �������
			if (duocenb!=0)
			{
				//CText(ent[i].center, _T("���壬���λ!"), 0);
				CTextmin(ent[i].center, _T("���壬���λ!"), 0);
				CTextmin(ent[i].center, _T("���壬���λ!"), 1);
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
			if (bigfun2c(newi) && iisdoor&&!bmj)ADDdoorArray.push_back(newi);
			// ���ּ�������
			if (isLskwf(ent[i]))
			{
				DrLsArry.push_back(ent[i].center);
				LsArray.push_back(ent[i]);
			}
			// ����9���ױ���δ���
			if (bb9mm)
			{
				if (!on_offi)
				{
					CreateArrow(ent[i].maxp, ifx, 0, 110, 150);
					CText(ent[i].maxp, _T("�������?"),ifx);
				}
				if (!on_offa)
				{
					CreateArrow(ent[i].minp, ifx, 0, 110, 150);
					CText(ent[i].minp, _T("�������?"),ifx);
				}
				
			}
			//������ͨ����
			if (p1yt)
			{
				// ����������ͨ
				if (!yty)
				{
					CreateArrow(ent[i].maxp, ifx, 0, 110, 100);
					CText(ent[i].center, _T("��ͨ�ߴ����!"), ifx);
				}
				if (!ytz)
				{
					
					CreateArrow(ent[i].minp, ifx, 0, 110, 100);
					CText(ent[i].center, _T("��ͨ�ߴ����!"), ifx);
				}
				
			}
			// ��鲣����� �ߴ�
			if (bl)
			{
				if (!blz || !bly)
				{
					CreateArrow(ent[i].center, ifx, 0, 110, 100);
					CreateArrow(ent[i].center, ifx, 1, 110, 100);
					CText(ent[i].center, _T("�������ߴ����!"), ifx);
				}
			}
			
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

	if (!ADDdoorArray.empty())
	{
		std::vector<int> v; //���鼯�ϣ������ڼ��������
		std::vector<EntBox> NewAddDoorArray; //���鼯�ϣ������ڼ��������

		for (int i = 0; i < ADDdoorArray.size(); i++)
		{
			if (std::find(v.begin(), v.end(), i) == v.end())
			{
				for (int j = 0; j < ADDdoorArray.size(); j++)
				{
					// �����ཻ ������ ihej
					if (i != j && (BoxTangencyBox(ADDdoorArray[i], ADDdoorArray[j]) ||
						BoxIntersectBox(ADDdoorArray[i], ADDdoorArray[j], 0) ||
						EquaPoint(ADDdoorArray[i].maxp, ADDdoorArray[j].maxp) ||
						EquaPoint(ADDdoorArray[i].minp, ADDdoorArray[j].minp)))
					{
						v.push_back(i);
						v.push_back(j);
						ADDdoorArray[i].maxp = 
							AcGePoint3d(max(ADDdoorArray[i].maxp.x, ADDdoorArray[j].maxp.x),
								max(ADDdoorArray[i].maxp.y, ADDdoorArray[j].maxp.y),
								max(ADDdoorArray[i].maxp.z, ADDdoorArray[j].maxp.z));
						ADDdoorArray[i].minp =
							AcGePoint3d(min(ADDdoorArray[i].minp.x, ADDdoorArray[j].minp.x),
								min(ADDdoorArray[i].minp.y, ADDdoorArray[j].minp.y),
								min(ADDdoorArray[i].minp.z, ADDdoorArray[j].minp.z));
					}

				}
				ADDdoorArray[i].center = 
					AcGePoint3d((ADDdoorArray[i].maxp.x + ADDdoorArray[i].minp.x) / 2,
					(ADDdoorArray[i].maxp.y + ADDdoorArray[i].minp.y) / 2,
						(ADDdoorArray[i].maxp.z + ADDdoorArray[i].minp.z) / 2);

				NewAddDoorArray.push_back(ADDdoorArray[i]);
			}
		}
		// �����Ŀ��ͬ���Ž�λ�ò�ͬ
		// �����ŷ�5-18 �Ű����¶��� ���Ҷ���ҲҪ��
		TestMjLs(NewAddDoorArray, DrLsArry, MjArray);
	}
	if (RArray.size()>1)
	{
		for (int i=0; i<RArray.size();i++)
		{
			for (int j = 0; j < RArray.size(); j++)
			{
				if (abs(RArray[i].r-RArray[j].r)>10)
				{
					Arrow3dXY(RArray[i].center, 30, 112);
					Arrow3dXY(RArray[j].center, 30, 112);
					CreateLine(RArray[i].center, RArray[j].center, 112);
					CText(LineCenter(RArray[i].center, RArray[j].center), _T("Բ���뾶��ͳһ!"), 0);
				}
			}
		}
	}

	// �ж���ֱ�����Ű����ֿ�λ��ײ
	if (!LsArray.empty()&&!LzqArray.empty())
	{
		
		for (int i = 0; i < LsArray.size(); i++)
		{
			for (int j = 0; j < LzqArray.size(); j++)
			{
				if (BoxIntersectBox2(ExspansionEnt(LsArray[i], 4), LzqArray[j]))
				{
					
					CreateArrow(LsArray[i].center, 0, 0, 110, 100);
					CreateArrow(LsArray[i].center, 1, 0, 110, 100);
					CText(LsArray[i].center, _T("��������ֱ����ײ!"), 0);
				}
			}
		}
	}

	// ************************** �жϿ�λ�Ƿ��ٰ���� ************************** 
	if (!entshy.empty())
	{
		for (int i = 0; i < entshy.size(); i++)
		{
			
			for (int x = 0; x < ent.size(); x++)
			{
				// �ҵ���������box �ھ�����ѭ��
				if (PointInBoxTangency(entshy[i].center, ent[x])&&
					entshy[i].id!=ent[x].id)
				{
					break;
				}
				// �����һ����û�ҵ��ͻ�����
				if (x==ent.size()-1 && entshy[i].volume > 500)
				{
					CreateArrow(entshy[i].center, 0, 0, 110, 100);
					CreateArrow(entshy[i].center, 1, 0, 110, 100);
					CText(entshy[i].center, _T("�տ�!"), 0);
					CText(entshy[i].center, _T("�տ�!"), 1);
				}
			}
		}
	}

	// ====--====
	if (!entshy.empty())
	{
		// 
		TestShyBord(Marray, entshy,shym, SHYArrayH);
	}
	/*if (!ent.empty())
	{
		for (int i=0; i<ent.size();i++)
		{
			testbref(ent[i].id);
		}
	}*/
	//CTextmin(AcGePoint3d(0,0,0), _T("���壬���λ!"), 0);
	acutPrintf(_T("\n Hello LF ������:%d !"), ent.size()*ent.size());
}
