//
// Created by khloud ghattas on 12/5/19.
//

#ifndef PROJ_CACHE_H
#define PROJ_CACHE_H

class Cache{
public:
   int views;
   int ImageName;
   time_t createdAt;
   time_t updatedAt;
private:
    Cache();
   ~Cache();
   void UpdateCache(createdAt);
};

#endif //PROJ_CACHE_H
