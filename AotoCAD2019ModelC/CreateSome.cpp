#include "StdAfx.h"
#include "CreateSome.h"
// ���ڽ��ڴ��е�ʵ����ӵ����ݿ��ļ�dwg��
#include "Database.h"
#include "Editor.h"
#include <vector>

Line::Line() {}
Line::~Line() {}

AcDbObjectId Line::Add(const AcGePoint3d &startPoint,
	const AcGePoint3d &endPoint,
	AcDbDatabase *pDb)
{	// AcDbLine�࣬�̳���AcDbCurve�࣬�����������ڴ��д���ֱ�߶���
	AcDbLine *pLine = new AcDbLine(startPoint, endPoint);
	// ��ֱ�߶����ָ�롢���ݿ����ָ�봫��PostToModelSpace������ӵ����ݿ��ļ�dwg��
	return Database::PostToModelSpace(pLine, pDb);
}

void CText(const AcGePoint3d& ptInsert, const ACHAR* text, int fx
	, AcDbObjectId style, double height, double width)
{

	//AcDbText  *pTextEntity = new AcDbText(insetPoint, text, AcDbObjectId::kNull, 100, 0);
	AcDbMText *pMText = new AcDbMText();
	//���ö������ֵ�����
	pMText->setTextStyle(style);
	pMText->setContents(text);
	pMText->setLocation(ptInsert);
	pMText->setTextHeight(height);
	pMText->setWidth(width);
	pMText->setAttachment(AcDbMText::kBottomLeft);
	pMText->setColorIndex(1);
	//pTextEntity->setTextStyle();
	//���α任�س�
	AcGeMatrix3d xform;

	if (fx == 1)
	{
		AcGeVector3d vec(0.0, 1.0, 0.0);
		//������Ҫ���еı任
		xform.setToRotation(270.0*3.1415926535 *0.25, vec, ptInsert);
		pMText->transformBy(xform);
		AcGeVector3d vec2(1.0, 0.0, 0.0);
		//������Ҫ���еı任
		xform.setToRotation(90.0*3.1415926535 *0.25, vec2, ptInsert);
		pMText->transformBy(xform);
		
	}
	else
	{
		AcGeVector3d vec(1.0, 0.0, 0.0);
		//������Ҫ���еı任
		xform.setToRotation(90.0*3.1415926535 *0.25, vec, ptInsert);
		pMText->transformBy(xform);
	}
	

	Database::PostToModelSpace(pMText);
}

void CTextmin(const AcGePoint3d& ptInsert, const ACHAR* text, int fx
	, AcDbObjectId style, double height, double width)
{

	//AcDbText  *pTextEntity = new AcDbText(insetPoint, text, AcDbObjectId::kNull, 100, 0);
	AcDbMText *pMText = new AcDbMText();
	//���ö������ֵ�����
	pMText->setTextStyle(style);
	pMText->setContents(text);
	pMText->setLocation(ptInsert);
	pMText->setTextHeight(height);
	pMText->setWidth(width);
	pMText->setAttachment(AcDbMText::kBottomLeft);
	pMText->setColorIndex(1);
	//pTextEntity->setTextStyle();
	//���α任�س�
	AcGeMatrix3d xform;

	if (fx == 1)
	{
		AcGeVector3d vec(0.0, 1.0, 0.0);
		//������Ҫ���еı任
		xform.setToRotation(270.0*3.1415926535 *0.25, vec, ptInsert);
		pMText->transformBy(xform);
		AcGeVector3d vec2(1.0, 0.0, 0.0);
		//������Ҫ���еı任
		xform.setToRotation(90.0*3.1415926535 *0.25, vec2, ptInsert);
		pMText->transformBy(xform);

	}
	else
	{
		AcGeVector3d vec(1.0, 0.0, 0.0);
		//������Ҫ���еı任
		xform.setToRotation(90.0*3.1415926535 *0.25, vec, ptInsert);
		pMText->transformBy(xform);
	}


	Database::PostToModelSpace(pMText);
}

void CreateBox(AcGePoint3d& cp, int le, int col)
{

	//////////////////////////////////////////////////////////////////////////
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x - le, cp.y, cp.z), AcGePoint3d(cp.x + le, cp.y, cp.z)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y - le, cp.z), AcGePoint3d(cp.x, cp.y + le, cp.z)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y, cp.z - le), AcGePoint3d(cp.x, cp.y, cp.z + le)), col);
	//////////////////////////////////////////////////////////////////////////
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x - le / 3, cp.y - le / 3, cp.z), AcGePoint3d(cp.x + le / 3, cp.y + le / 3, cp.z)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y - le / 3, cp.z - le / 3), AcGePoint3d(cp.x, cp.y + le / 3, cp.z + le / 3)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x - le / 3, cp.y, cp.z - le / 3), AcGePoint3d(cp.x + le / 3, cp.y, cp.z + le / 3)), col);
	//////////////////////////////////////////////////////////////////////////
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x + le / 3, cp.y - le / 3, cp.z), AcGePoint3d(cp.x - le / 3, cp.y + le / 3, cp.z)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y + le / 3, cp.z - le / 3), AcGePoint3d(cp.x, cp.y - le / 3, cp.z + le / 3)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x - le / 3, cp.y, cp.z + le / 3), AcGePoint3d(cp.x + le / 3, cp.y, cp.z - le / 3)), col);
	//////////////////////////////////////////////////////////////////////////

}

void CreateLine(const AcGePoint3d &startPoint,
	const AcGePoint3d &endPoint, int col)
{
	Editor::SetColor(Line::Add(startPoint, endPoint), col);
}
void Arrow3dXY(AcGePoint3d& cp, int le, int col)
{
	//le �������ӣ�
	// xz
	Editor::SetColor(Line::Add(cp, AcGePoint3d(cp.x + le / 2, cp.y, cp.z + le / 2)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x + le / 2, cp.y, cp.z + le / 2), AcGePoint3d(cp.x + le * 0.35, cp.y, cp.z + le / 5)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x + le * 2, cp.y, cp.z), AcGePoint3d(cp.x + le * 0.35, cp.y, cp.z + le / 5)), col);
	// 
	Editor::SetColor(Line::Add(cp, AcGePoint3d(cp.x + le * 0.6, cp.y, cp.z - le / 2)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x + le * 0.6, cp.y, cp.z - le / 2), AcGePoint3d(cp.x + le * 0.45, cp.y, cp.z - le / 8)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x + le * 2, cp.y, cp.z), AcGePoint3d(cp.x + le * 0.45, cp.y, cp.z - le / 8)), col);

	// yz
	Editor::SetColor(Line::Add(cp, AcGePoint3d(cp.x, cp.y + le / 2, cp.z + le / 2)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y + le / 2, cp.z + le / 2), AcGePoint3d(cp.x, cp.y + le * 0.35, cp.z + le / 5)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y + le * 2, cp.z), AcGePoint3d(cp.x, cp.y + le * 0.35, cp.z + le / 5)), col);
	// 
	Editor::SetColor(Line::Add(cp, AcGePoint3d(cp.x, cp.y + le * 0.6, cp.z - le / 2)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y + le * 0.6, cp.z - le / 2), AcGePoint3d(cp.x, cp.y + le * 0.45, cp.z - le / 8)), col);
	Editor::SetColor(Line::Add(AcGePoint3d(cp.x, cp.y + le * 2, cp.z), AcGePoint3d(cp.x, cp.y + le * 0.45, cp.z - le / 8)), col);
}

// ��ͷ 01 ����xy ���� 01 sx
void CreateArrow(const AcGePoint3d& point, int xy, int sx, int color, int le)
{
	// ���������� ����1 ����0 xy 0x y1

	double x = point.x;
	double y = point.y;
	double z = point.z;
	double g1 = le * 0.20;
	double g2 = le * 0.07;
	double w1 = le * 0.35;
	//int w2 = 6;
	AcDbObjectIdArray entIds;

	entIds.append(Line::Add(point, AcGePoint3d(x + w1, y, z + g1)));
	entIds.append
	(Line::Add(AcGePoint3d(x + w1, y, z + g1), AcGePoint3d(x + w1, y, z + g2)));
	entIds.append
	(Line::Add(AcGePoint3d(x + w1, y, z + g2), AcGePoint3d(x + le, y, z + g2)));
	//// ���Ƕ�
	entIds.append(Line::Add(point, AcGePoint3d(x + w1, y, z - g1)));
	entIds.append
	(Line::Add(AcGePoint3d(x + w1, y, z - g1), AcGePoint3d(x + w1, y, z - g2)));
	entIds.append
	(Line::Add(AcGePoint3d(x + w1, y, z - g2), AcGePoint3d(x + le, y, z - g2)));
	// ����
	entIds.append
	(Line::Add(AcGePoint3d(x + le, y, z + g2), AcGePoint3d(x + le, y, z - g2)));
	int len = entIds.length();
	double r = 90.0;
	if (xy == 1 && sx == 1)
	{
		// ��ͷ����ZY ������
		for (int i = 0; i < len; i++)
		{
			AcGeVector3d vec(0.0, 0.0, 1.0);
			Rotate(entIds[i], point, r, vec);
		}
		for (int i = 0; i < len; i++)
		{
			AcGeVector3d vec(1.0, 0.0, 0.0);
			Rotate(entIds[i], point, r, vec);
		}
	}
	if (xy == 0 && sx == 1)
	{
		// ��ͷ����ZY ������
		for (int i = 0; i < len; i++)
		{
			AcGeVector3d vec(0.0, 1.0, 0.0);
			Rotate(entIds[i], point, r, vec);
		}
	}
	if (xy == 1 && sx == 0)
	{
		// ����״̬ ��ͷ����ƽ��zy �᷽��
		for (int i = 0; i < len; i++)
		{
			AcGeVector3d vec(0.0, 0.0, 1.0);
			Rotate(entIds[i], point, r, vec);
		}
	}
	// ������ɫ
	for (int i = 0; i < len; i++)
	{
		Editor::SetColor(entIds[i], color);
	}

}