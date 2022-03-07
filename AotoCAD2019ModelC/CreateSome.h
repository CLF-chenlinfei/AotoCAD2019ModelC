#pragma once
class Line
{
public:
	Line();
	~Line();

public:
	// 函数返回所画直线的对象ID为后续修改属性操作做准备：
	//						两个3d点，一个数据库指针（有默认值，固定的）
	static AcDbObjectId Add(const AcGePoint3d &startPoint,
		const AcGePoint3d &endPoint,
		AcDbDatabase *pDb =
		acdbHostApplicationServices()->
		workingDatabase());
};
void CText(const AcGePoint3d& ptInsert, const ACHAR* text
	, int fx, AcDbObjectId style = AcDbObjectId::kNull, double height = 30, double width = 5000);

void CreateBox(AcGePoint3d& cp, int le, int col);
//画箭头
void Arrow3dXY(AcGePoint3d& cp, int le, int col);

void CreateLine(const AcGePoint3d &startPoint,
	const AcGePoint3d &endPoint, int col);
void CreateArrow(AcGePoint3d& point, int xy, int sx, int color, int le);