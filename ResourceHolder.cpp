#include "ResourceHolder.hpp"

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier id, const std::string &filename)
{
	std::unique_ptr<Resource> resource( new Resource() );
	if(!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - cannot load " + filename);

	insertResource(id, std::move(resource));
}

template<typename Identifier, typename Resource>
template<typename Parameter>
void ResourceHolder<Identifier, Resource>::load(Identifier id, const std::string &filename, const Parameter &param)
{
	std::unique_ptr<Resource> resource( new Resource() );
	if(!resource->loadFromFile(filename, param))
		throw std::runtime_error("ResourceHolder::load - cannot load " + filename + " with param " + param);

	insertResource(id, std::move(resource) );
}

// use reference &resource here, check
template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto insert = mResourceMap.insert( std::make_pair(id, std::move(resource) ));
	assert(insert.second);
}

template<typename Identifier, typename Resource>
Resource & ResourceHolder<Identifier, Resource>::get(Identifier id)
{
	return const_cast<Resource &>( static_cast<const ResourceHolder &>(*this).get(id) );
}

template<typename Identifier, typename Resource>
const Resource & ResourceHolder<Identifier, Resource>::get(Identifier id) const
{
	auto itr = mResourceMap.find(id);
	assert(itr != mResourceMap.end() );
	return *(itr->second);
}