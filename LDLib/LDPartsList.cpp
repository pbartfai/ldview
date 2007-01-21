#include "LDPartsList.h"
#include <LDLoader/LDLModel.h>
#include <LDLoader/LDLModelLine.h>
#include <TCFoundation/mystring.h>

LDPartsList::LDPartsList(void)
{
}

LDPartsList::~LDPartsList(void)
{
}

void LDPartsList::dealloc(void)
{
	TCObject::dealloc();
}

void LDPartsList::scanModel(LDLModel *model)
{
	LDPartCountMap::iterator it;

	m_partCountMap.clear();
	m_totalParts = 0;
	scanSubModel(model);
	m_partCounts.clear();
	m_partCounts.reserve(m_partCountMap.size());
	for (it = m_partCountMap.begin(); it != m_partCountMap.end(); it++)
	{
		m_partCounts.push_back(it->second);
	}
}

void LDPartsList::scanSubModel(LDLModel *subModel)
{
	LDLFileLineArray *fileLines = subModel->getFileLines();

	if (fileLines)
	{
		int i;
		int count = subModel->getActiveLineCount();

		for (i = 0; i < count; i++)
		{
			LDLFileLine *fileLine = (*fileLines)[i];

			if (fileLine->getLineType() == LDLLineTypeModel)
			{
				LDLModelLine *modelLine = (LDLModelLine *)fileLine;
				LDLModel *model = modelLine->getModel();

				if (model)
				{
					if (model->isPart())
					{
						char *filename = convertStringToLower(filenameFromPath(
							model->getName()));
						LDPartCount &partCount = m_partCountMap[filename];

						if (!partCount.getModel())
						{
							partCount.setModel(filename, model);
						}
						partCount.addPart(modelLine->getColorNumber());
						m_totalParts++;
						delete filename;
					}
					else
					{
						scanSubModel(model);
					}
				}
			}
		}
	}
}
