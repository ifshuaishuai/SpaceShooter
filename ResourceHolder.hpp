#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP
 
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template<typename Identifier, typename Resource>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string &filename);

	template<typename Parameter>
	void load(Identifier id, const std::string &filename, const Parameter &param);

	Resource & get(Identifier id);
	const Resource & get(Identifier id) const;

private:
	void insertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
	std::map< Identifier, std::unique_ptr<Resource> >	mResourceMap;
};

 
#endif