#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <memory>

template<typename T> class ResourceManager {

public:

    typedef std::shared_ptr<T> Resource;

    /// Preloads a resource
    void add(const std::string& key ) {
        if ( resources.count(key) == 0 ) resources[key] = load(key);
    }

    /// Gets the resource and loads it if it hasn't been loaded yet
    Resource get(const std::string& key) {
        Resource result;
        auto it = resources.find(key);
        if (it == resources.end())           // not loaded yet, load now
            return resources.insert(std::make_pair(key, load(key))).first->second;
        else
            return it->second;
    }

    /// Removes all unused loaded resources
    void clean() {
        auto it = resources.begin();        // could be more elegant with the C++11 foreach loop
        while (  it !=resources.end() ) {
            if ( it->second.unique() )
                resources.erase(it++);      // eraserd element's iterator is invalidated, so the post increment is important here...
            else                            // btw C++11 map::erase returns iterator, so this could be safely: it = resources.erase(it);
                ++it;
        }
    }

protected:

    /// Must be implemented for the corresponding resource type
    virtual Resource load(const std::string& key) = 0;

    /// Map for the resources
    std::map<std::string, Resource> resources;
};

#endif // RESOURCE_MANAGER_H
