#ifndef MAP_UTIL_H
#define MAP_UTIL_H

#include <map>
using namespace std;

class MapUtil
{
public:
	template <typename K, typename V>
	static bool Contains(map<K, V>* map, K key);
};

#endif // !MAP_UTIL_H

template<typename K, typename V>
inline bool MapUtil::Contains(map<K, V>* map, K key)
{
	return map->find(key) != map->end();
}