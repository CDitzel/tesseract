#include <models/imageModel.hpp>
#include <iostream>

int ImageModel::ctr = 0;

ImageModel::ImageModel(const SubdirectoryHandler& snapshot)
  : leftStereoImage_{ snapshot.camImages.at("leftStereoImg") }
  , wideAngleImage_{ snapshot.camImages.at("wideAngleImg") }
{
  ++ctr;
  std::cout << "ImageModel Constructed " << std::endl;
}

ImageModel::~ImageModel()
{
  --ctr;
  std::cout << "ImageModel Destructed " << std::endl;
}

// void
// ImageModel::setView(View& view) const
//{
//}

void
draw(const ImageModel&, std::ostream& out, size_t position)
{
  out << std::string(position, ' ') << "ImageModel" << std::endl;
};

void
display(const ImageModel&)
{
}
