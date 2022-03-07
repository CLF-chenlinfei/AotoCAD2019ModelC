#pragma once
class Line
{
public:
	Line();
	~Line();

public:
	// ������������ֱ�ߵĶ���IDΪ�����޸����Բ�����׼����
	//						����3d�㣬һ�����ݿ�ָ�루��Ĭ��ֵ���̶��ģ�
	static AcDbObjectId Add(const AcGePoint3d &startPoint,
		const AcGePoint3d &endPoint,
		AcDbDatabase *pDb =
		acdbHostApplicationServices()->
		workingDatabase());
};
void CText(const AcGePoint3d& ptInsert, const ACHAR* text
	, int fx, AcDbObjectId style = AcDbObjectId::kNull, double height = 30, double width = 5000);

void CreateBox(AcGePoint3d& cp, int le, int col);
//����ͷ
void Arrow3dXY(AcGePoint3d& cp, int le, int col);

void CreateLine(const AcGePoint3d &startPoint,
	const AcGePoint3d &endPoint, int col);
void CreateArrow(AcGePoint3d& point, int xy, int sx, int color, int le);