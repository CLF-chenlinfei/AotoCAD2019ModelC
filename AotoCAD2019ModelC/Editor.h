
// �����������ڵ��ļ���һ�������ı�����ֻ������һ�Σ��ӿ�����ٶ�
#pragma once
class Editor
{
public:
	Editor();
	~Editor();
public:
	// ����������������ACHAR�ǣ��������ʶ�������ú�������
	static  void AddCommand(const ACHAR * cmdName,
		Adesk::Int32 commandFlags,
		AcRxFunctionPtr FunctionAddr);
	// �����������������������ú�������						
	static  void AddCommand(const ACHAR *cmdName,
		AcRxFunctionPtr FunctionAddr);
	// �޸���ɫ���������ݿ�ʵ�����id,��ɫ����						
	static  void SetColor(AcDbObjectId entId,
		Adesk::UInt16 colorIndex);
	// ��ѡͬʱ��ȡʵ��ָ�롢ʵ��id���򿪷�ʽ
	static AcDbEntity *Editor::selectEntity(AcDbObjectId &eId,
		AcDb::OpenMode openMode);
};

void Rotate(AcDbObjectId entId, const AcGePoint3d &ptBase, double rotation, AcGeVector3d vec);