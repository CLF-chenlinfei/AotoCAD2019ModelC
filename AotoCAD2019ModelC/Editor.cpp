#include "StdAfx.h"
#include "Editor.h"


Editor::Editor() {}
Editor::~Editor() {}

// ��������Ҫ��������
void Editor::AddCommand(const ACHAR * cmdName,
	Adesk::Int32 commandFlags,
	AcRxFunctionPtr FunctionAddr)
{	// ������acedRegCmds�����������д��������ΪToolsBox
	acedRegCmds->addCommand(_T("ToolsBox"),
		cmdName, 		// ������
		cmdName, 		// �������
		commandFlags, 	// ���Ʋ���
		FunctionAddr);	// ���ú�����
}

// ������������Ҫ��������
void Editor::AddCommand(const ACHAR *cmdName,
	AcRxFunctionPtr FunctionAddr)
{	// ���ҵ��ã�������Ҫ����������AddCommand
	AddCommand(cmdName,
		ACRX_CMD_MODAL, 	// д�������������ֻ��Ҫ����������
		FunctionAddr);
}

// �޸���ɫ������ID����ɫ����
void Editor::SetColor(AcDbObjectId entId, Adesk::UInt16 colorIndex)
{	// ������ɫ�����ķ�Χ
	assert(colorIndex >= 0 && colorIndex <= 256);
	// ��������ʼ�����ݿ�ʵ��ָ��
	AcDbEntity *pEnt = NULL;
	// acdbOpenObject�����������ݿ����id��entIdΪ����,��dwg���ҵ���������ڴ�
	// ������ָ���pEnt,kForWriteΪд���Ʋ���
	// �жϺ�������ֵ�Ƿ�����Acad::eOk
	if (acdbOpenObject(pEnt, entId, AcDb::kForWrite) == Acad::eOk)
	{	// AcDbEntity�����ķ���setColorIndex��������ɫ
		pEnt->setColorIndex(colorIndex);
		// ���������ر�ָ��
		pEnt->close();
	}
	// �򿪴��󣬴�ӡԭ��
	else
	{	// �˺�������C���Ե�printf
		acutPrintf(_T("\n�޷����Ķ�����ɫ"));
	}
}

// ��ѡ��ȡʵ��ָ�룺����ʵ��id���򿪷�ʽ
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
//			pEnty->erase();  //ɾ�����а�����ʵ��
//		pEnty->close();

void Rotate(AcDbObjectId entId, const AcGePoint3d &ptBase, double rotation, AcGeVector3d vec)
{
	//���α任�س�
	AcGeMatrix3d xform;
	//����
	/*AcGeVector3d vec(0.0, 0.0, 1.0);*/
	//������Ҫ���еı任
	xform.setToRotation(rotation*3.1415926535 *0.25, vec, ptBase);
	//����ʵ��ָ��
	AcDbEntity *pEnt = NULL;
	//��ͼ��ʵ��
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	//�����������ִ��ת��
	if (es == Acad::eOk)
	{
		es = pEnt->transformBy(xform);
		pEnt->close();
	}

}