#include "StdAfx.h"
#include "Editor.h"


Editor::Editor() {}
Editor::~Editor() {}

// 添加命令：需要三个参数
void Editor::AddCommand(const ACHAR * cmdName,
	Adesk::Int32 commandFlags,
	AcRxFunctionPtr FunctionAddr)
{	// 调用了acedRegCmds宏来新增命令，写死命令组为ToolsBox
	acedRegCmds->addCommand(_T("ToolsBox"),
		cmdName, 		// 命令名
		cmdName, 		// 命令别名
		commandFlags, 	// 控制参数
		FunctionAddr);	// 调用函数名
}

// 重载添加命令：需要两个参数
void Editor::AddCommand(const ACHAR *cmdName,
	AcRxFunctionPtr FunctionAddr)
{	// 自我调用：调用需要三个参数的AddCommand
	AddCommand(cmdName,
		ACRX_CMD_MODAL, 	// 写死这个参数，就只需要两个参数了
		FunctionAddr);
}

// 修改颜色：对象ID，颜色索引
void Editor::SetColor(AcDbObjectId entId, Adesk::UInt16 colorIndex)
{	// 断言颜色索引的范围
	assert(colorIndex >= 0 && colorIndex <= 256);
	// 声明并初始化数据库实体指针
	AcDbEntity *pEnt = NULL;
	// acdbOpenObject函数：以数据库对象id即entId为线索,从dwg中找到对象读入内存
	// 并返回指针给pEnt,kForWrite为写控制参数
	// 判断函数返回值是否正常Acad::eOk
	if (acdbOpenObject(pEnt, entId, AcDb::kForWrite) == Acad::eOk)
	{	// AcDbEntity类对象的方法setColorIndex，设置颜色
		pEnt->setColorIndex(colorIndex);
		// 操作完必须关闭指针
		pEnt->close();
	}
	// 打开错误，打印原因
	else
	{	// 此函数类似C语言的printf
		acutPrintf(_T("\n无法更改对象颜色"));
	}
}

// 点选获取实体指针：返回实体id，打开方式
AcDbEntity *Editor::selectEntity(AcDbObjectId &eId, AcDb::OpenMode openMode)
{
	ads_name en;
	ads_point pt;
	if (acedEntSel(NULL, en, pt) != RTNORM)
	{
		return NULL;
	}
	acdbGetObjectId(eId, en);
	AcDbEntity *pEnt;
	acdbOpenObject(pEnt, eId, openMode);
	return pEnt;
}

void eascEnt()
{

}
//AcDbEntity *pEnty = NULL;
//Acad::ErrorStatus es;
//int j;
//for (j = 0; j < arrIds.length(); j++)
//{
//	undefined
//		es = acdbOpenObject(pEnty, arrIds.at(j), AcDb::kForWrite);
//	if (Acad::eOk == es)
//	{
//		undefined
//			pEnty->erase();  //删除组中包含的实体
//		pEnty->close();

void Rotate(AcDbObjectId entId, const AcGePoint3d &ptBase, double rotation, AcGeVector3d vec)
{
	//几何变换矩陈
	AcGeMatrix3d xform;
	//向量
	/*AcGeVector3d vec(0.0, 0.0, 1.0);*/
	//设置需要进行的变换
	xform.setToRotation(rotation*3.1415926535 *0.25, vec, ptBase);
	//定义实体指针
	AcDbEntity *pEnt = NULL;
	//试图打开实体
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	//如果不出错则执行转换
	if (es == Acad::eOk)
	{
		es = pEnt->transformBy(xform);
		pEnt->close();
	}

}