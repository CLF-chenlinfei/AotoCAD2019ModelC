#include "StdAfx.h"
#include "CreateSome.h"
#include "Editor.h"
#include "Commands.h"
#include "Database.h"
#include <vector>
#include "alg.h"

// �˺������������������
// 		��������������һϵ�в����Ŀ�ݼ�
void AddCommands()
{	// ���AddLine����������CreateLine����
	Editor::AddCommand(_T("ww"), ACRX_CMD_MODAL, CreateLine);
	Editor::AddCommand(_T("dd"), ACRX_CMD_MODAL, remove0);
	Editor::AddCommand(_T("rr"), ACRX_CMD_MODAL, remove0);
	//Editor::AddCommand(_T("Addar"), ACRX_CMD_MODAL, CreateAr);
	// ���ChangeLineColor����������ChangeColor����
	//Editor::AddCommand(_T("ChangeLineColor"), ACRX_CMD_MODAL, ChangeColor);
}

// ���廭ֱ�ߺ���
void CreateLine()
{
	Database::GetModelSpaceExtent();
}

void remove0()
{
	const ACHAR* title = L"0";
	Database::deleteLayer(title);
}

void breftest()
{

}
