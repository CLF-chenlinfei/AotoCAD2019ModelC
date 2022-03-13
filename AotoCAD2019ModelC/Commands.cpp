#include "StdAfx.h"
#include "CreateSome.h"
#include "Editor.h"
#include "Commands.h"
#include "Database.h"
#include <vector>
#include "alg.h"

// 此函数可以批量添加命令
// 		命令：可以想象成是一系列操作的快捷键
void AddCommands()
{	// 添加AddLine命令，命令调用CreateLine函数
	Editor::AddCommand(_T("ww"), ACRX_CMD_MODAL, CreateLine);
	Editor::AddCommand(_T("dd"), ACRX_CMD_MODAL, remove0);
	Editor::AddCommand(_T("rr"), ACRX_CMD_MODAL, remove0);
	//Editor::AddCommand(_T("Addar"), ACRX_CMD_MODAL, CreateAr);
	// 添加ChangeLineColor命令，命令调用ChangeColor函数
	//Editor::AddCommand(_T("ChangeLineColor"), ACRX_CMD_MODAL, ChangeColor);
}

// 定义画直线函数
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
