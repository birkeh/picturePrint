#include "common.h"


QStringList generateReadList(const QList<IMAGEFORMAT>& imageFormats)
{
	QStringList	readList;

	for(int z = 0;z < imageFormats.count();z++)
	{
		IMAGEFORMAT	i	= imageFormats[z];

		if(i.read)
			readList.append(i.extension.split(" "));
	}

	return(readList);
}

QStringList generateWriteList(const QList<IMAGEFORMAT>& imageFormats)
{
	QStringList	writeList;

	for(int z = 0;z < imageFormats.count();z++)
	{
		IMAGEFORMAT	i	= imageFormats[z];

		if(i.write)
			writeList.append(i.extension.split(""));
	}

	return(writeList);
}
