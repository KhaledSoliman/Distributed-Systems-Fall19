#ifndef PROJ_IMAGEBODY_H
#define PROJ_IMAGEBODY_H
#include <string>

class ImageBody{
public:
    const std::string &getImageName() const;
    void setImageName(const std::string &imageName);
private:
   std::string ImageName;
   ImageBody();
   ~ImageBody();
  void Base46(const std::string String_base64);
};
#endif //PROJ_IMAGEBODY_H
