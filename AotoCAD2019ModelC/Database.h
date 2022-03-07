#pragma once
class Database
{
public:
	Database();
	~Database();
public:
	// 在模型空间中创建对象：传入实体对象指针，数据库指针（头文件有默认值）
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
	// 获取层上所有实体：图层名(默认为空)、数据库对象								
	static AcDbObjectIdArray GetAllEntIds(const ACHAR *layer = NULL,
		AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
	static void deleteLayer(const ACHAR *layer = NULL,
		AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
	// 总目录6.1：获得模型空间的范围盒：数据库指针
	static void GetModelSpaceExtent(AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
};



