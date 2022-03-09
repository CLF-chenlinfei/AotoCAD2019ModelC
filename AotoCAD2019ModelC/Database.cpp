#include "StdAfx.h"
#include "Database.h"
#include <vector>
#include "alg.h"
#include "name.h"
#include "inspection.h"
//#include "detection.h"

Database::Database() {}
Database::~Database() {}

// 在模型空间中创建对象：传入实体对象指针，数据库指针（头文件有默认值）
AcDbObjectId Database::PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb)
{	// 声明并初始化块表对象指针
	AcDbBlockTable *pBlkTbl = NULL;
	// 声明并初始化错误状态
	Acad::ErrorStatus es;
	// 数据库对象调用，以读方式打开块表，将块表指针赋值给pBlkTbl
	es = pDb->getBlockTable(pBlkTbl, AcDb::kForRead);

	// 纠错函数1：如果块表获取失败；若没纠错函数，cad会直接崩溃关闭
	if (es != Acad::eOk)
	{	// _RXST，宽字符串另一种写法，acadErrorStatusText函数将es转换为字符串
		acutPrintf(_RXST("\n块表打开失败，错误代码：%s"),
			acadErrorStatusText(es));
		// 返回数据库对象id为空，跳出函数
		return AcDbObjectId::kNull;
	}

	// 声明并初始化块表记录对象指针
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	// 块表对象指针调用getAt方法获得块表记录指针pBlkTblRcd
	// 参数：模型空间（acdb.h中的一个宏）、块表记录指针（接收返回值）、以写方式定位
	es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForWrite);

	// 纠错函数2：如果块表记录获取失败；若没纠错函数，cad会直接崩溃关闭
	if (es != Acad::eOk)
	{
		acutPrintf(_RXST("\n模型空间块表记录打开失败，错误代码：%s"),
			acadErrorStatusText(es));
		// 关闭已经打开的块表
		pBlkTbl->close();
		// 返回数据库对象id为空，跳出函数
		return AcDbObjectId::kNull;
	}

	// 已经获取到块表记录指针，可以关闭块表了
	pBlkTbl->close();
	// 声明数据库对象ID变量
	AcDbObjectId outId;
	// 块表记录对象 指针 调用 函数appendAcDbEntity，
	// 将实体对象指针pEnt指向的内存中的记录添加到数据库文件dwg中，块表记录的最后
	// 并将添加成功后的实体对象id赋值给outId
	es = pBlkTblRcd->appendAcDbEntity(outId, pEnt);

	// 纠错函数3：如果块表记录添加失败；若没纠错函数，cad会直接崩溃关闭
	if (es != Acad::eOk)
	{
		acutPrintf(_RXST("\n无法在模型空间中添加实体，错误代码：%s"),
			acadErrorStatusText(es));
		// 关闭块表记录
		pBlkTblRcd->close();
		// 返回数据库对象id为空，跳出函数
		return AcDbObjectId::kNull;
	}
	// 若一切正常，清空数据库实体对象指针指向的内存
	pEnt->close();
	// 关闭块表记录
	pBlkTblRcd->close();
	// 返回实体对象ID给程序
	return outId;
}

void Database::deleteLayer(const ACHAR *layer, AcDbDatabase *pDb)
{
	AcDbLayerTable *pLayerTbl = NULL;
	Acad::ErrorStatus es;
	pDb->getLayerTable(pLayerTbl, AcDb::kForWrite);
	AcDbObjectId layerId;
	es = pLayerTbl->getAt(layer, layerId);
	if (Acad::eOk == es)
	{
		pLayerTbl->close();
	}
	//AcDbGroup *pGroup = NULL;
	
	// 获取数据库块表->数据库块表记录->关闭块表
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();
	// 遍历数据库块表记录
	AcDbBlockTableRecordIterator *pItr = NULL;
	pBlkTblRcd->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		AcDbEntity *pEnt;
		pItr->getEntity(pEnt, AcDb::kForWrite);
		// 如果实体的图层为指定过滤的图层
		if (pEnt->layerId() == layerId)
		{
			//acutPrintf(_T("\n:%d"), pEnt->objectId());
			pEnt->erase();
		}
		pEnt->close();
	}
	delete pItr;
	pBlkTblRcd->close();
}
// 获取层上所有实体：图层名(默认为空)、数据库对象
AcDbObjectIdArray Database::GetAllEntIds(const ACHAR *layer, AcDbDatabase *pDb)
{
	AcDbObjectIdArray entIds;
	// 图层过滤控制符
	bool bFilterLayer = false;
	AcDbObjectId layerId;
	// 如果传入了图层名
	if (layer != NULL)
	{	// 获取层表->判断是否有该图层->获得层表id->设置过滤控制符
		AcDbLayerTable *pLayerTbl = NULL;
		pDb->getLayerTable(pLayerTbl, AcDb::kForRead);
		// 容错函数：如果层表不包含该图层，关闭层表，返回空id列表
		if (!pLayerTbl->has(layer))
		{
			pLayerTbl->close();
			return entIds;
		}
		pLayerTbl->getAt(layer, layerId);
		pLayerTbl->close();
		bFilterLayer = true;
	}

	// 获取数据库块表->数据库块表记录->关闭块表
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	// 遍历数据库块表记录
	AcDbBlockTableRecordIterator *pItr = NULL;
	pBlkTblRcd->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
	{	// 如果过滤控制符为真，获得该层上所有实体的id
		if (bFilterLayer == true)
		{	// 获得每个实体的指针
			AcDbEntity *pEnt;
			AcDbEntity *pEnty;
			pItr->getEntity(pEnt, AcDb::kForRead);
			// 如果实体的图层为指定过滤的图层
			if (pEnt->layerId() == layerId)
			{	// 将实体id添加进id列表 entIds 中
				entIds.append(pEnt->objectId());
				acutPrintf(_T("\n %d:id"), pEnt->objectId());
				Acad::ErrorStatus es;
				es = acdbOpenObject(pEnty, pEnt->objectId(), AcDb::kForWrite);
				if (Acad::eOk == es)
				{
					pEnty->erase();  //删除组中包含的实体
					pEnty->close();
				}
				else
				{
					pEnty->close();
				}
			}
			pEnt->close();
		}
		else
		{
			// 将所有实体id添加进id列表 entIds 中
			AcDbObjectId objId;
			pItr->getEntityId(objId);
			entIds.append(objId);
		}
	}

	delete pItr;
	pBlkTblRcd->close();
	return entIds;
}
//////////////////////////////////////////////////////////////////////////
//获取所有的包围盒

//////////////////////////////////////////////////////////////////////////
// 总目录6.1：获得模型空间的范围盒：数据库指针
void Database::GetModelSpaceExtent(AcDbDatabase *pDb)
{	// 获得数据库块表指针-》获得模型空间的块表记录指针
	clock_t start_time = clock();
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();
	// ﻿计算包含块表记录中所有实体的最小范围盒
	AcDbExtents extent;
	//Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
	pBlkTblRcd->close();
	// 如果范围盒生成成功
	//if (es != Acad::eOk)
	AcDbBlockTableRecordIterator *pItr = NULL;
	pBlkTblRcd->newIterator(pItr);
	std::vector<EntBox> MydataR;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		//获取每个实体指针
		AcDbEntity *pEnt;
		pItr->getEntity(pEnt, AcDb::kForRead);
		AcDbObjectId objId;
		pItr->getEntityId(objId);

		if (acdbOpenObject(pEnt, objId, AcDb::kForRead) == Acad::eOk)
		{
			AcDbExtents pex;
			pEnt->getGeomExtents(pex);
			USES_CONVERSION;
			std::string model_layer = W2A(pEnt->layer());
			std::string model_type = W2A(pEnt->isA()->name());
			EntBox tempdt;
			tempdt.minp = pex.minPoint();
			tempdt.maxp = pex.maxPoint();
			tempdt.id = objId;
			//acutPrintf(_T("\n: %d"), tempdt.id);
			tempdt.Layer = model_layer;
			tempdt.Type = model_type;
			tempdt.volume = (pex.maxPoint().x - pex.minPoint().x)*(pex.maxPoint().y - pex.minPoint().y)*(pex.maxPoint().z - pex.minPoint().z);
			tempdt.center = (AcGePoint3d((pex.maxPoint().x + pex.minPoint().x) / 2, (pex.maxPoint().y + pex.minPoint().y) / 2, (pex.maxPoint().z + pex.minPoint().z) / 2));
			//MydataR.push_back(tempdt);
			if (tempdt.Type != "AcDbLine"&&
				tempdt.Type != "AcDbMText"&&
				tempdt.Type != "AcDbZombieEntity")
			{
				MydataR.push_back(tempdt);
			}

			pEnt->close();
		}
		else {
			pEnt->close();
		}
		pEnt->close();
	}
	// 清空元素:
	delete pItr;
	pBlkTblRcd->close();
	// 开始检查
	time_t now = time(0);
	tm *ltm = localtime(&now);
	int year = ltm->tm_year + 1900;
	int mon = 1 + ltm->tm_mon;
	int day = ltm->tm_mday;
	if (year < 2023 && mon < 4)
	{
		acutPrintf(_T("逆水行舟!"));
		//decall(MydataR);
		CkModel(MydataR);
	}
	else
	{
		acutPrintf(_T("有船新版本要更新，此版本废弃!"));
	}
	//decall(MydataR);
	clock_t end_time = clock();
	acutPrintf(_T("运行耗时:%.2fs。"), (double)(end_time - start_time) / CLOCKS_PER_SEC);
}

