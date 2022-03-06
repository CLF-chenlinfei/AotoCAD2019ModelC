
// 此语句会让所在的文件在一个单独的编译中只被包含一次，加快编译速度
#pragma once
class Editor
{
public:
	Editor();
	~Editor();
public:
	// 添加命令：按命令名（ACHAR是），命令标识符，调用函数传参
	static  void AddCommand(const ACHAR * cmdName,
		Adesk::Int32 commandFlags,
		AcRxFunctionPtr FunctionAddr);
	// 重载添加命令：按命令名，调用函数传参						
	static  void AddCommand(const ACHAR *cmdName,
		AcRxFunctionPtr FunctionAddr);
	// 修改颜色：传入数据库实体对象id,颜色索引						
	static  void SetColor(AcDbObjectId entId,
		Adesk::UInt16 colorIndex);
	// 点选同时获取实体指针、实体id，打开方式
	static AcDbEntity *Editor::selectEntity(AcDbObjectId &eId,
		AcDb::OpenMode openMode);
};

void Rotate(AcDbObjectId entId, const AcGePoint3d &ptBase, double rotation, AcGeVector3d vec);