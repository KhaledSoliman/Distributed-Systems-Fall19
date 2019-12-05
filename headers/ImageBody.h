#ifndef PROJ_IMAGEBODY_H
#define PROJ_IMAGEBODY_H
#include <string>

class ImageBody{
public:
    const std::string &getImageName() const;

    const std::string &getImagePath() const;

    void setImageName(const std::string &imageName);

    void setImagePath(const std::string &imagePath);

private:
   std::string ImageName;

   std::string ImagePath;

   ImageBody();

   ~ImageBody();

  void StringBase46(std::string String_base64);
};
#endif //PROJ_IMAGEBODY_H
