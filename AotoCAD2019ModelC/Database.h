#pragma once
class Database
{
public:
	Database();
	~Database();
public:
	// ��ģ�Ϳռ��д������󣺴���ʵ�����ָ�룬���ݿ�ָ�루ͷ�ļ���Ĭ��ֵ��
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
	// ��ȡ��������ʵ�壺ͼ����(Ĭ��Ϊ��)�����ݿ����								
	static AcDbObjectIdArray GetAllEntIds(const ACHAR *layer = NULL,
		AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
	static void deleteLayer(const ACHAR *layer = NULL,
		AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
	// ��Ŀ¼6.1�����ģ�Ϳռ�ķ�Χ�У����ݿ�ָ��
	static void GetModelSpaceExtent(AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase());
};



