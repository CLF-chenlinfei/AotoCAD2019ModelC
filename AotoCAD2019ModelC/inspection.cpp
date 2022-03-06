#include "stdafx.h"
#include "inspection.h"
#include "alg.h"
#include "CreateSome.h"
#include <vector>
#include <algorithm>
//#include "name.h"

// 记住能检测两个的尽量不要开辟新的循环

void CkModel(std::vector<EntBox>ent) 
{
	// 实体数量必须有2个或者以上
	if (!ent.empty())
	{
		EntBox new1;
		for (int i = 0; i < ent.size(); i++)
		{
			//次级 JJJJJJ
			for (int j = 0; j < ent.size(); j++)
			{
				
				// 1.判断孔位重叠相交 三/二 合一
				if (judeMore(ent[i], ent[j]))
				{
					CreateArrow(ent[i].center, 0, 0, 110, 100);
					CreateArrow(ent[i].center, 1, 0, 110, 100);
					CText(ent[i].center, _T("重孔?多孔?撞孔?"));
				}
				// 2.判断灯槽与孔位相撞 
				if (BoxIntersectBox2(ExspansionEnt(ent[i]), ExspansionEnt(ent[j]))
					&& isLamp(ent[i]) && isHole(ent[j]))
				{

					Arrow3dXY(ent[j].center, 30, 111);
					CText(ent[j].center, _T("孔撞灯槽?"));
				}
				// 3.干涉检查1 只是柜体
				if (Intervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("干涉"));
				}
				// 4.干涉检查2 柜体和门板
				if (DoorBIntervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("干涉2"));
				}
				// 5.干涉检查3 只是门板
				if (DoorIntervene(ent[i], ent[j]))
				{
					Arrow3dXY(ent[j].center, 30, 112);
					Arrow3dXY(ent[i].center, 30, 112);
					CreateLine(ent[j].center, ent[i].center, 112);
					CText(LineCenter(ent[j].center, ent[i].center), _T("干涉3"));
				}
				// 6.检查多背板
				// CText(bb.center, _T("X轴:背板重叠!"));
				// CText(bb.center, _T("Y轴:背板重叠!"));
				// 7.检查衣通
				// 8.检查门铰(应该包含门铰与其他相交，门铰有无连接)
				// CText(p1.sx.center, _T("门铰与其他干涉。"));
				// CText(p1.sx.center, _T("门铰无连接？"));
				// CText(lsit1[j].p1.sx.center, _T("此门门铰盖值不一致!"));
				// CText(errovec[i], _T("    __拉手未对齐!"));
				// CText(bb9.minp, _T("背板入槽?"));
				// CText(LineCenter(wdlist[x].center, ent[s].center), _T("门板方向下!"));
				// CText(LineCenter(wdlist[x].center, ent[s].center), _T("门板方向上!"));
				// CText(LineCenter(wdlist[x].center, wdlist[i].center), _T("_上下门缝未对齐!"));
				//  同门内门铰盖值 应该是合成后的门板和前的都有
			}
			// 9.检查门板拉手集合表
			// 10.检查合成后的门板表门缝
			// 11.检查门板与门铰关系， 当门铰在门板内 切最小值不等时判断为错，容差2
			// 12.检查门铰数量
			// 291-900   x2
			// 901-1600  x3
			// 1601-2100 x4
			// 2101-2400 x5
			// 2401-2700 x6
			// 13.检查拉手位置
			// 14.9mm板未入槽检测，注意装饰盒
			// 15.检查门缝（有上下，有左右 上下应该是不对齐，左右差5-18）
			// CText(wdlist[i].center, _T("X_门缝5-18!"));
			// CText(wdlist[i].center, _T("Y_5-18门缝!"));
			// 16.孔位检查
			// CText(LineCenter(rec18data[i].p1, rec18data[i].p2), _T("板件尺寸错误"));

		}
	}
	
	acutPrintf(_T("\n Hello yangyang 计算量:%d !"), ent.size()*ent.size());
}
