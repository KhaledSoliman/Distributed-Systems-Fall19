//
// Created by khloud ghattas on 12/5/19.
//

#ifndef PROJ_CACHE_H
#define PROJ_CACHE_H

class Cache{
public:

private:
    int views;
    int ImageName;
    time_t createdAt;
    time_t updatedAt;
    Cache();
   ~Cache();
   void UpdateCache();
   void InsertCache();
};

#endif //PROJ_CACHE_H
