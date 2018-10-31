#include "utilities/subdirectoryHandler.hpp"
#include <bitset>

void
SubdirectoryHandler::readFileNamesOfDirectory()
{
  fs::recursive_directory_iterator it{ snapshotDirPath_ };
  while (it != fs::recursive_directory_iterator{}) {
    if (!fs::is_regular_file(*it))
      continue;
    if (it->path().extension() == ".tiff") {
      tiffFileNames_.emplace_back(it->path().string());
    } else if (it->path().extension() == ".pcd") {
      pcdFileNames_.emplace_back(it->path().string());
    } else if (it->path().extension() == ".adma") {
      admaFileNames_.emplace_back(it->path().string());
    } else if (it->path().extension() == ".json") {
      jsonFileNames_.emplace_back(it->path().string());
    } else {
      //      std::cerr << "Found unsupported filetype " << it->path().string()
      //      << '\n';
    }
    it = std::next(it);
  }
}

void
SubdirectoryHandler::ExtractFromTIFF()
{
  for (auto const& filename : tiffFileNames_) {
    if (filename.find("_camera_16bit.") != std::string::npos) {
      camImages.insert_or_assign(
        "wideAngleImg",
        Tesseract::convertGrayscaleTiffImageBitDepth<12>(filename));
    } else if (filename.find("_stereo_left.") != std::string::npos) {
      camImages.insert_or_assign(
        "leftStereoImg",
        Tesseract::convertGrayscaleTiffImageBitDepth<12>(filename));
    } else if (auto start{ filename.find("radar_") };
               start != std::string::npos) {
      size_t stop{ filename.find_last_of('.') };
      std::string radarSignalName{ filename.substr(start, stop - start) };

      std::vector<uint16_t> radarSignalBuffer{};
      if (radarSignalName.find("_far_") != std::string::npos) {
        radarMode_ = tsr::ARS300::FRS;
        radarSignalBuffer.resize(3400);
      } else if (radarSignalName.find("_near_") != std::string::npos) {
        radarMode_ = tsr::ARS300::NRS;
        radarSignalBuffer.resize(1700);
      }
      TIFF* tp{ TIFFOpen(filename.c_str(), "r") };
      assert(tp != nullptr);
      int width{ 0 }, height{ 0 };
      TIFFGetField(tp, TIFFTAG_IMAGEWIDTH, &width);
      TIFFGetField(tp, TIFFTAG_IMAGELENGTH, &height);

      for (auto y{ 0 }; y < height; ++y) {
        if (int check_io{ TIFFReadScanline(tp, std::data(radarSignalBuffer) +
                                                 std::size(radarSignalBuffer) -
                                                 y * width - width,
                                           y, 0) };
            check_io != 1) {
          std::ostringstream errorStream{};
          errorStream << "Failed to read line: " << y << " of: " << filename;
          throw Exception{ errorStream.str() };
        }
      }
      TIFFClose(tp);

      if (radarMode_ == tsr::ARS300::FRS) {
        radarImagesFar.emplace(
          radarSignalName,
          Tesseract::convertGrayscaleTiffImageBitDepth<16>(filename, 2));
        radarDataFar.emplace(radarSignalName, radarSignalBuffer);

      } else if (radarMode_ == tsr::ARS300::NRS) {
        radarImagesNear.emplace(
          radarSignalName,
          Tesseract::convertGrayscaleTiffImageBitDepth<16>(filename, 2));
        radarDataNear.emplace(radarSignalName, radarSignalBuffer);
        if (false) {
          std::cout << __func__ << "     " << radarSignalName << std::endl;
          std::cout << std::left << std::setw(20)
                    << "Image Width: " << std::right << std::setw(4) << width
                    << std::endl
                    << std::left << std::setw(20)
                    << "Image Height: " << std::right << std::setw(4) << height
                    << std::endl
                    << std::left << std::setw(20);
          for (int r{ 0 }; r < height; ++r) {
            std::cout << "\nLine:" << std::setw(3) << std::right << r << "";
            for (int c{ 0 }; c < width; ++c) {
              std::cout << std::setw(6)
                        << radarDataNear[radarSignalName][r * width + c];
            }
          }
          std::cout << std::endl;
        }
      }
    }
  }
}

void
SubdirectoryHandler::ExtractFromPCD()
{
  for (auto const& filename : pcdFileNames_) {
    auto stop{ filename.find("_syn_imageRange.") };
    //    if (stop == std::string::npos)
    //      stop = filename.find("_StereoGP");
    if (stop != std::string::npos) {
      //    if (auto stop{ filename.find("_syn_imageRange.") };
      //        stop != std::string::npos) {
      auto start{ filename.substr(0, stop).find_last_of('_') };
      //      std::cerr << "Filename " << filename << std::endl;
      std::ifstream inputFile{ filename };
      assert(inputFile.is_open() && !inputFile.fail());
      for (size_t i{ 1 }; i <= 11; ++i) { // ignore the first 11 lines for pcd
        inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      std::string cloudName{ filename.substr(start + 1, stop - start - 1) };
      std::copy(std::istream_iterator<Tesseract::Point3D>{ inputFile },
                std::istream_iterator<Tesseract::Point3D>{},
                std::back_inserter(pointClouds[cloudName]));
    }
  }
}

void
SubdirectoryHandler::ExtractFromADMA()
{
  egoPositions_.reserve(std::size(admaFileNames_));
  for (auto const& filename : admaFileNames_) {
    std::ifstream admaFile{ filename };
    assert(admaFile.is_open() && !admaFile.fail());
    Tesseract::Position2D egoPosition{};
    std::string ignore{};
    for (std::string buffer{}; std::getline(admaFile, buffer);) {
      if (std::empty(buffer)) // Ignore empty lines
        continue;
      std::stringstream paramStream{};
      if (buffer.find("f64Inertial_Pos_Dez_In_Long") != std::string::npos) {
        paramStream.str(buffer);
        paramStream >> ignore >> egoPosition.x_;
      } else if (buffer.find("f64Inertial_Pos_Dez_In_Lat") !=
                 std::string::npos) {
        paramStream.str(buffer);
        paramStream >> ignore >> egoPosition.y_;
      } else if (buffer.find("f64Tilt_Tilt_Yaw") != std::string::npos) {
        paramStream.str(buffer);
        paramStream >> ignore >> egoPosition.phi_;
      } else if (buffer.find("f64Inertial_VH_In_VXH") != std::string::npos) {
        paramStream.str(buffer);
        paramStream >> ignore >> egoPosition.vxh_;
      }
    }
    egoPositions_.push_back(egoPosition);
  }
  std::for_each(std::begin(egoPositions_), std::end(egoPositions_),
                [](auto& egoPosition) {
                  Tesseract::LLtoUTMWithAngle(egoPosition.y_, egoPosition.x_,
                                              egoPosition.phi_, egoPosition.y_,
                                              egoPosition.x_, egoPosition.phi_);
                });
}

void
SubdirectoryHandler::ExtractFromJSON()
{ // if no radar data was annotated in the directory yet
  if (std::empty(jsonFileNames_)) {
    radarAnnotationsFar_["imgHeight"] = 200;
    radarAnnotationsFar_["imgWidth"] = 17;
    radarAnnotationsFar_["objects"];
    radarAnnotationsNear_["imgHeight"] = 100;
    radarAnnotationsNear_["imgWidth"] = 17;
    radarAnnotationsNear_["objects"];
  } else { // if radar data was processed before
    for (auto const& filename : jsonFileNames_) {
      if (filename.find("IntensityFar") != std::string::npos) {
        std::ifstream ifs{ filename };
        ifs >> radarAnnotationsFar_;
      } else if (filename.find("IntensityNear") != std::string::npos) {
        std::ifstream ifs{ filename };
        ifs >> radarAnnotationsNear_;
      } else
        throw Exception{ "Some serious shit going on" };
    }
  }
}

void
SubdirectoryHandler::archive(const RadarArchiver& archiver)
{
  if (archiver.mode_ == tsr::ARS300::FRS) {
    radarAnnotationsFar_["objects"].emplace_back(archiver.object_);
  } else if (archiver.mode_ == tsr::ARS300::NRS) {
    radarAnnotationsNear_["objects"].emplace_back(archiver.object_);
  }
}

void
SubdirectoryHandler::wipeArchive(tsr::ARS300 _mode)
{
  if (_mode == tsr::ARS300::FRS)
    tsr::obliterate(radarAnnotationsFar_["objects"]);
  else if (_mode == tsr::ARS300::NRS)
    tsr::obliterate(radarAnnotationsNear_["objects"]);
}

void
SubdirectoryHandler::serializeJSON() const
{
  //  std::cerr << "FAR Annoitation" << radarAnnotationsFar_ << std::endl;
  std::ofstream _radarFarOutputFile{ serializationPathFar_ };
  _radarFarOutputFile << std::setw(4) << radarAnnotationsFar_;
  //  std::cerr << "NEAR Annoitation" << radarAnnotationsNear_ << std::endl;
  std::ofstream _radarNearOutputFile(serializationPathNear_);
  _radarNearOutputFile << std::setw(4) << radarAnnotationsNear_;
}

const json&
SubdirectoryHandler::annotations(tsr::ARS300 _mode) const
{
  if (_mode == tsr::ARS300::FRS) {
    return radarAnnotationsFar_;
  } else if (_mode == tsr::ARS300::NRS) {
    return radarAnnotationsNear_;
  }
}
