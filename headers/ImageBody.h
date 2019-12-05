#ifndef PROJ_IMAGEBODY_H
#define PROJ_IMAGEBODY_H
#include <string>

class ImageBody{
public:

private:
   std::string ImageName;
   ImageBody();
   ~ImageBody();
   ImageBody(std::string String_base64);
};
#endif //PROJ_IMAGEBODY_H
