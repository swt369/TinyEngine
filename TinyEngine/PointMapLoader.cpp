#include "PointMapLoader.h"

PointMapLoader & PointMapLoader::getInstance()
{
	static PointMapLoader instance;
	return instance;
}

PointMap * PointMapLoader::LoadPointMap(ifstream* in)
{
	vector<Point> points;
	string line;
	while (getline(*in, line))
	{
		float pointData[6];
		fill(pointData, pointData + 6, 0.0f);

		vector<string> data = StringUtil::split(line, ',');
		for (int i = 0; i < (data.size() <= 6 ? data.size() : 6); i++)
		{
			pointData[i] = stof(data[i]);
		}
		points.push_back(Point(pointData));
	}

	return new PointMap(points);
}

PointMapLoader::PointMapLoader() {}