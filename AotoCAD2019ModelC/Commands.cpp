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
	//Editor::AddCommand(_T("Addar"), ACRX_CMD_MODAL, CreateAr);
	// ���ChangeLineColor����������ChangeColor����
	//Editor::AddCommand(_T("ChangeLineColor"), ACRX_CMD_MODAL, ChangeColor);
}

// ���廭ֱ�ߺ���
void CreateLine()
{
	Database::GetModelSpaceExtent();
}


