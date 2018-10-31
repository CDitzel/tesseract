#pragma once
#include "gdal/gdal_priv.h"
#include "gdal/vrtdataset.h"
#include "tiffio.h"
#include "utilities/exception.hpp"
#include <QImage>
#include <bitset>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unordered_map>
#include <utility>

using json = nlohmann::json;

namespace Tesseract {

template <typename T, typename... Ts>
void
emplace_back_All(std::vector<T>& vec, Ts... ts)
{ // "," prevents parameter pack from being folded
  (vec.emplace_back(ts), ...);
}

template <typename T>
static auto
emplace_back_All(std::vector<T>& vec)
{ // "," prevents parameter pack from being folded
  return [&](auto&&... ts) {
    (vec.emplace_back(std::forward<decltype(ts)>(ts)), ...);
  };
}

/*!
 * \brief emplace_All                   Emplaces multiple parameters into an
 * \param associativeContainer          Associative container of unique keys
 * \param ts                            Parameter pack, i.e. value-type of
 *                                      either std::set or std::map
 * \return                              True if all emplacements succeeded
 */
template <typename T, typename... Ts>
bool
emplace_All(T& associativeContainer, Ts... ts)
{
  return (associativeContainer.emplace(ts).second && ...);
}
// use like so:
// std::pair<int, int> s{ 3, 5 };
// if (emplace_All(m_, std::make_pair(1, 2), s, std::pair{ 4, 3 }))

template <typename... Ts>
void
obliterate(Ts&... ts)
{
  (ts.clear(), ...);
}

/*!
 * \brief shell_all                     Writes all elements to standard output
 * \param container                     Arbitrary container, its value type has
 *                                      to have implemented operator<<()
 * \param os                            Arbitrary subclass of std::ostream
 *
 */
template <typename T>
void
shell_all(const T& container, std::ostream& os = std::cout)
{
  os.setf(std::ios::left | std::ios::adjustfield | std::ios::unitbuf);
  os << "-- Shell all --\n";
  std::copy(std::cbegin(container), std::cend(container),
            std::ostream_iterator<typename T::value_type>{ os, "\n" });
}

/*!
 * \brief shell_it                      Allows for formatted output of any
 * \param obj                           Object that defined an output operator
 */
template <typename T>
void
shell_it(const T& obj)
{ // almost always auto
  auto sheller{ std::ostream_iterator<T>{ std::cout, "\n" } };
  sheller = obj;
}

/*!
 * \brief scaleValue        scales the given value accordings to the parameters
 * \param min               minimum value of the range to be transformed and the
 * \param max               maximum value of the range to be transformed
 * \param new_max           a new maximum for the projection to the range, e.g.
 *                          new_max = 1 ranges the value in the unit intervall
 */
static auto
scaleValue(float min, float max, float new_max = 1)
{
  const float diff{ max - min };
  assert(diff != 0); // prevent division by zero
  return [=](float val) -> float { return (val - min) / diff * new_max; };
}

/*!
 * \brief clampValue        this function object builder limits values to the
 * \param min               specified minimum cut-off number and the
 * \param max               specified maximum cut-off number
 */
static auto
clampValue(float min, float max)
{
  return [=](float val) -> float { return std::clamp(val, min, max); };
}

/*!
 * \brief multiCall         fire-and-forget function that compiles with a
 * \param functions         collection of function objects that are called
 *                          regardless of whether they return anything or what
 *                          they return. It has a variadic constructor while the
 *                          parameter pack is expanded with an initializer list
 *                          and the functions return value is thrown away and
 *                          0 is inserted eventually
 */
static auto
multiCall(auto&... functions)
{
  return [&](auto const& arg) {
    static_cast<void>(
      std::initializer_list<int>{ (static_cast<void>(functions(arg)), 0)... });
  };
}

static auto
for_each_param(auto f, auto... xs)
{
  (void)std::initializer_list<int>{ ((void)f(xs), 0)... };
}

/*!
 * \brief within            Closure builder that returns a function that
 *                          checks if any object implementing operator<() is
 * \param min               within bounds given by the minumum and maximum
 * \param max               value used while construction of the lambda
 */
template <typename T>
[[maybe_unused]] static auto
within(T min, T max)
{
  return [=](T val) {
    //    std::cerr << "min " << min << "max " << max << "val" << val <<
    //   std::endl;
    //    if (min <= val && val <= max)
    //      std::cerr << "SUCCESS " << std::endl;
    return min <= val && val <= max;
  };
}

/*!
 * \brief allWithin         Checks whether a number of objects are within range
 * \param min               defined by a minimum and a
 * \param max               maximum respectively
 * \param ts                Parameter pack that is to be checked for inclusion
 * \return                  True if all passed arguments are within bounds
 */
template <typename T, typename... Ts>
[[nodiscard]] bool
allWithin(T min, T max, Ts... ts)
{
  return ((min <= ts && ts <= max) && ...);
}

/*!
 * \brief combine           Creates complex predicates with logical conjunction
 * \param binary_func       Binary function, e.g. logical AND, logical OR, etc.
 * \param a                 Predicate function, i.e. function returning a bool
 * \param b                 Second predicate function that is combined with a
 */
template <typename F, typename A, typename B>
[[nodiscard]] auto
combine(F binary_func, A predicateA, B predicateB)
{
  return [=](auto param) {
    return binary_func(predicateA(param), predicateB(param));
  };
}
// use like so:
// static bool begins_with_a(const std::string &s){return s.find("a") == 0;}
// static bool ends_with_b(const std::string &s){return s.rfind("b") ==
// s.length() -1;}
// auto a_xxx_b {combine(logical_and<>{}, begins_with_a, ends_with_b);};
// std::copy_if(ifstream_iterator<std::string>{cin}, {},
// ostream_iterator<std::string>{std::cout, ","}, a_xxx_b);
// $ echo "ac cb ab axxb" | ./combine
// ab, axxxb,

/*!
 * \brief opt_print         Function builder that encapsulates the end iterator
 *                          of a data structure
 * \param container         Arbitrary container conforming to the STL standard
 */
template <typename C>
[[nodiscard]] static auto
safe_deref(const C& container)
{
  return [end_it{ std::cend(container) }](const auto& iter)
    ->std::optional<typename C::value_type>
  {
    if (iter != end_it)
      return *iter;
    else
      return std::nullopt;
  };
}

struct Pixel
{
  uint16_t x_, y_;
};

static std::ostream&
operator<<(std::ostream& os, Pixel const& pixel)
{
  os << "X: " << pixel.x_ << "Y: " << pixel.y_ << '\n';
}

// conveience function for simplified parsing of json files
// https://github.com/nlohmann/json
static void
from_json(const json& j, Pixel& pixel)
{
  pixel.x_ = j["name"].get<uint16_t>();
  pixel.y_ = j["age"].get<uint16_t>();
}

[[nodiscard]] static auto
index_to_pixel(uint16_t divisor)
{
  return [=](uint16_t dividend) {
    auto remainder{ std::modulus<uint16_t>{}(dividend, divisor) + 1 };
    auto quotient{ static_cast<uint16_t>(dividend / divisor + 1) };
    return Pixel{ static_cast<uint16_t>(remainder), quotient };
  };
}

[[nodiscard]] static auto
pixel_to_index(uint16_t width)
{
  return [=](const Pixel& p) {
    return static_cast<uint16_t>(width * (p.y_ - 1) + p.x_ - 1);
  };
}

enum class InvertTransformation
{
  No,
  Yes
};

static std::pair<double, double>
transformCoordSystem(double const& x, double const& y, double const& sinRot,
                     double const& cosRot, double const& transX,
                     double const& transY, InvertTransformation invert)
{
  //  double c{cos(rot)}, s{sin(rot)};
  if (invert == InvertTransformation::Yes)
    return { (cosRot * x + sinRot * y) - transX,
             (-sinRot * x + cosRot * y) - transY };
  return { (cosRot * x - sinRot * y) + transX,
           (sinRot * x + cosRot * y) + transY };
}

/*!
 *
 */
template <unsigned bitsPerPixel>
QImage
convertGrayscaleTiffImageBitDepth(std::string const& tiffPath, int ofs = 0)
{
  TIFF* tp{ TIFFOpen(tiffPath.c_str(), "r") };
  if (tp == nullptr)
    throw Exception{ "Failed to open TIFF: '" + tiffPath + "'" };
  int width, height;
  if (TIFFGetField(tp, TIFFTAG_IMAGEWIDTH, &width) != 1) {
    throw Exception{ "Failed to read height of TIFF: '" + tiffPath + "'" };
  } // Read dimensions of image, throw if error code != 1 which means success
  if (TIFFGetField(tp, TIFFTAG_IMAGELENGTH, &height) != 1) {
    throw Exception{ "Failed to read width of TIFF: '" + tiffPath + "'" };
  }
  QImage result{ width, height - ofs, QImage::Format_Grayscale8 };
  std::vector<uint16_t> src(width); // buffer for following bitshift operation
  for (size_t y{ 0 }; y < height - ofs; ++y) {
    TIFFReadScanline(tp, std::data(src), y, 0);
    uint8_t* dst = result.scanLine(y);
    for (size_t x{ 0 }; x < width; ++x) {
      dst[x] =
        src.at(x) >> (bitsPerPixel - 8); // cut off least significant bits
    }
  }
  if (result.isNull())
    throw Exception{ "Failed to write to QImage: '" + tiffPath + "'" };
  //  if (!result.save("/home/ditzel/Desktop/testradar.jpg", 0, 100))
  //    throw Exception{ "Failed to SAVE Image" + tiffPath + "'" };
  TIFFClose(tp);
  return result;
}

struct Point3D
{
  //  Point3D() = default;
  //  Point3D(float X, float Y, float Z, float Label = 0, float Intensity = 0)
  //    : x{ X }
  //    , y{ Y }
  //    , z{ Z }
  //    , label{ Label }
  //    , intensity{ Intensity }
  //  {
  //  }

  float x_;
  float y_;
  float z_;
  float label_;
  float intensity_;
  unsigned id_;
};

static std::ostream&
operator<<(std::ostream& os, Point3D const& point3D)
{
  os << "X: " << point3D.x_ << "Y: " << point3D.y_ << "Z: " << point3D.z_
     << "Label: " << point3D.label_ << "Intensity: " << point3D.intensity_
     << '\n';
}

static std::istream&
operator>>(std::istream& is, Point3D& point)
{
  return is >> point.x_ >> point.y_ >> point.z_ >> point.label_ >>
         point.intensity_;
}

struct Position2D
{
  double x_;
  double y_;
  double phi_;
  double vxh_;

  friend std::ostream& operator<<(std::ostream& os, const Position2D& position)
  {
    os << "\nEast: " << position.x_ << "\nNorth: " << position.y_
       << "\nYaw: " << position.phi_ << "\nVel: " << position.vxh_;
    return os;
  }
};

template <typename T>
/*!
 * \brief insertionSort             In just two lines of code
 * \param start
 * \param end
 */
void
insertionSort(T start, T end)
{
  for (auto i = start; i != end; ++i)
    std::rotate(std::upper_bound(start, i, *i), i, std::next(i));
}

template <class FwdIt, class Compare = std::less<>>
void
quickSort(FwdIt first, FwdIt last, Compare cmp = Compare{})
{
  auto const N{ std::distance(first, last) };
  if (N <= 1)
    return;
  auto const pivot{ std::next(first, N / 2) };
  std::nth_element(first, pivot, last, cmp);
  quickSort(first, pivot, cmp); // assert(std::is_sorted(first, pivot, cmp));
  quickSort(pivot, last, cmp);  // assert(std::is_sorted(pivot, last, cmp));

  // can be optimized by adding insertionSort call for small ranges
}

template <typename I, typename P>
auto
stable_partition(I f, I l, P p) -> I
{
  auto n{ std::distance(l - f) };
  if (n == 0)
    return f;
  if (n == 1)
    return f + p(*f);
  auto m{ f + (n / 2) };
  return std::rotate(stable_partition(f, m, p), m, stable_partition(m, l, p));
}

// use case: list of items, make continous selection and move that selection
// into a new 'p' position.
template <typename randIter>
auto
slide(randIter f, randIter l, randIter p) -> std::pair<randIter, randIter>
{
  if (p < f)
    return { p, std::rotate(p, f, l) };
  if (l < p)
    return { std::rotate(f, l, p), p };
  return { f, l };
}

// use case: list of items, select some of items (good guys) and move the to
// position around p.
// for instance: multiple selection on a list
template <typename I, // I models BidirectionalIterator
          typename S> // S models UnaryPredicate
auto
gather(I f, I l, I p, S s) -> std::pair<I, I>
{
  using value_type = typename std::iterator_traits<I>::value_type;
  return { std::stable_partition(f, p,
                                 [&](const value_type& x) { return !s(x); }),
           std::stable_partition(p, l, s) };
}

// trim for strings
// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static std::string
trimLeft(const std::string& s)
{
  auto temp = s;
  temp.erase(std::begin(temp),
             std::find_if(std::begin(temp), std::end(temp), [](char c) {
               return !std::isspace(c, std::locale());
             }));
  return temp;
}

/*!
 * Transform_if implementation, TODO: A lot
 */
template <typename T>
auto
map(T fn)
{
  return [=](auto reduce_fn) {
    return [=](auto accum, auto input) { return reduce_fn(accum, fn(input)); };
  };
}

template <typename T>
auto
filter(T predicate)
{
  return [=](auto accum, auto input) {
    if (predicate(input)) {
      return reduce_fn(accum, input);
    } else {
      return accum;
    }
  };
};

auto copy_and_advance{ [](auto it, auto input) {
  *it = input;
  return ++it;
} };

static double
degree2rad(double deg)
{
  return deg * M_PI / 180.0;
}

static void
LLtoUTM(const double& Lat, const double& Long, double& UTMNorthing,
        double& UTMEasting)
{
  // converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
  // East Longitudes are positive, West longitudes are negative.
  // North latitudes are positive, South latitudes are negative
  // Lat and Long are in decimal degrees
  // Written by Chuck Gantz- chuck.gantz@globalstar.com

  double a = 6378137.0;
  double eccSquared = 0.00669438;
  double k0 = 0.9996;

  double LongOrigin;
  double eccPrimeSquared;
  double N, T, C, A, M;

  // Make sure the longitude is between -180.00 .. 179.9
  double LongTemp, LatRad, LatDeg;

  LongTemp =
    (Long + 180) - int((Long + 180) / 360) * 360 - 180; // -180.00 .. 179.9;
  LatRad = degree2rad(Lat);
  LatDeg = Lat;

  double LongRad = degree2rad(LongTemp);
  double LongOriginRad;
  int zoneNumber = int((LongTemp + 180) / 6) + 1;
  if (LatDeg >= 56.0 && LatDeg < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0)
    zoneNumber = 32;

  // Special zones for Svalbard
  if (LatDeg >= 72.0 && LatDeg < 84.0) {
    if (LongTemp >= 0.0 && LongTemp < 9.0)
      zoneNumber = 31;
    else if (LongTemp >= 9.0 && LongTemp < 21.0)
      zoneNumber = 33;
    else if (LongTemp >= 21.0 && LongTemp < 33.0)
      zoneNumber = 35;
    else if (LongTemp >= 33.0 && LongTemp < 42.0)
      zoneNumber = 37;
  }
  LongOrigin =
    (zoneNumber - 1) * 6 - 180 + 3; //+3 puts origin in middle of zone
  LongOriginRad = degree2rad(LongOrigin);

  eccPrimeSquared = (eccSquared) / (1 - eccSquared);

  N = a / sqrt(1 - eccSquared * sin(LatRad) * sin(LatRad));
  T = tan(LatRad) * tan(LatRad);
  C = eccPrimeSquared * cos(LatRad) * cos(LatRad);
  A = cos(LatRad) * (LongRad - LongOriginRad);

  M =
    a * ((1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 -
          5 * eccSquared * eccSquared * eccSquared / 256) *
           LatRad -
         (3 * eccSquared / 8 + 3 * eccSquared * eccSquared / 32 +
          45 * eccSquared * eccSquared * eccSquared / 1024) *
           sin(2 * LatRad) +
         (15 * eccSquared * eccSquared / 256 +
          45 * eccSquared * eccSquared * eccSquared / 1024) *
           sin(4 * LatRad) -
         (35 * eccSquared * eccSquared * eccSquared / 3072) * sin(6 * LatRad));

  (UTMEasting) =
    (double)(k0 * N * (A + (1 - T + C) * A * A * A / 6 +
                       (5 - 18 * T + T * T + 72 * C - 58 * eccPrimeSquared) *
                         A * A * A * A * A / 120) +
             500000.0);

  (UTMNorthing) =
    (double)(k0 *
             (M +
              N * tan(LatRad) *
                (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24 +
                 (61 - 58 * T + T * T + 600 * C - 330 * eccPrimeSquared) * A *
                   A * A * A * A * A / 720)));
  if (Lat < 0)
    (UTMNorthing) += 10000000.0; // 10000000 meter offset for southern
                                 // hemisphere
}

static void
LLtoUTMWithAngle(const double& Lat, const double& Long, const double& Phi,
                 double& UTMNorthing, double& UTMEasting, double& UTMPhi)
{
  // do the calculation for the LL-Coordinate
  LLtoUTM(Lat, Long, UTMNorthing, UTMEasting);

  // do the same for a test point ten meters east of the original point
  double TestPointLong = Long + 1e-04; //~10m in degrees
  double TestPointUTM_N;
  double TestPointUTM_E;
  LLtoUTM(Lat, TestPointLong, TestPointUTM_N, TestPointUTM_E);

  // calculate the angle of the difference vector
  double dN = TestPointUTM_N - UTMNorthing;
  double dE = TestPointUTM_E - UTMEasting;

  double dphi = atan2(dN, dE);

  // correct the angle always in rad and with lat/lon to north and with utm to
  // east
  UTMPhi = degree2rad(Phi + 90.0) + dphi;
}

/* std::pair<QImage, double> */
/* getImgFromGDAL(std::string const& m_vrtFileName, float width, float height,
 */
/*                float egoE, float egoN) */
/* { */
/*   //
 * ***************************************************************************
 */
/*   // GDAL content */
/*   //
 * ***************************************************************************
 */
/*   VRTDataset* m_vrtGDALDataSet = nullptr; // the data source */
/*   //  VRTRasterBand *m_bandRed, *m_bandGreen, *m_bandBlue = nullptr; */
/*   // Register gdal driver and initialize data source */
/*   GDALAllRegister(); */
/*   m_vrtGDALDataSet = (VRTDataset*)GDALOpen(m_vrtFileName.c_str(),
 * GA_ReadOnly); */
/*   if (m_vrtGDALDataSet == nullptr) */
/*     std::cerr << "ERROR reading the gdal dataset!" << std::endl; */

/*   CPLAssert(poBand->GetRasterDataType() == GDT_Byte); */
/*   // get the red green and blue "bands" */
/*   VRTRasterBand* m_bandRed =
 * (VRTRasterBand*)m_vrtGDALDataSet->GetRasterBand(1); */
/*   VRTRasterBand* m_bandGreen = */
/*     (VRTRasterBand*)m_vrtGDALDataSet->GetRasterBand(2); */
/*   VRTRasterBand* m_bandBlue = */
/*     (VRTRasterBand*)m_vrtGDALDataSet->GetRasterBand(3); */

/*   //
 * ***************************************************************************
 */
/*   // Compute corner points for the map retrieval */
/*   //
 * ***************************************************************************
 */
/*   double geoTransform[6]; */
/*   m_vrtGDALDataSet->GetGeoTransform(geoTransform); */
/*   const double eastingTopLeft = geoTransform[0];  // min easting */
/*   const double northingTopLeft = geoTransform[3]; // max northing */
/*   const double xMetersPerPixel = geoTransform[1]; // positive */
/*   const double yMetersPerPixel = geoTransform[5]; // negative */
/*   //  std::cout << "Map top left X = " << eastingTopLeft << '\n'; */
/*   //  std::cout << "Map top-left Y = " << northingTopLeft << '\n'; */
/*   //  std::cout << "Resolution [UTM/pxl] " << xMetersPerPixel << '\n'; */
/*   //  std::cout << "Resolution [UTM/pxl] " << yMetersPerPixel << '\n'; */

/*   const double topLeft_easting = egoE - width; // -width; */
/*   const double topLeft_northing = egoN + height; */

/*   const double topRight_easting = egoE + width; */
/*   const double topRight_northing = egoN + height; */

/*   const double bottomLeft_easting = egoE - width; // -width; */
/*   const double bottomLeft_northing = egoN - height; */

/*   const double bottomRight_easting = egoE + width; */
/*   const double bottomRight_northing = egoN - height; */

/*   //  std::cout << std::setprecision(10) << "e: " << topLeft_easting */
/*   //            << "\t\t\t  e: " << topRight_easting << std::endl; */
/*   //  std::cout << std::setprecision(10) << "n: " << topLeft_northing */
/*   //            << "\t\t\t n: " << topRight_northing << std::endl; */

/*   //  std::cout << std::setprecision(10) << "e: " << bottomLeft_easting */
/*   //            << "\t\t\t e: " << bottomRight_easting << std::endl; */
/*   //  std::cout << std::setprecision(10) << "n: " << bottomLeft_northing */
/*   //            << "\t\t\t n: " << bottomRight_northing << std::endl; */

/*   const size_t xOffsetInPxl = */
/*     std::floor((topLeft_easting - eastingTopLeft) / xMetersPerPixel); */
/*   const size_t yOffsetInPxl = */
/*     std::floor((topLeft_northing - northingTopLeft) / yMetersPerPixel); */
/*   const size_t xSizeInPxl = */
/*     std::floor((bottomRight_easting - bottomLeft_easting) /
 * xMetersPerPixel);
 */
/*   const size_t ySizeInPxl = */
/*     std::floor((bottomLeft_northing - topLeft_northing) / yMetersPerPixel);
 */

/*   //  std::cout << "Eckpunkte: " << std::endl; */
/*   //  std::cout << "xOffsetInPxl " << xOffsetInPxl << '\n'; */
/*   //  std::cout << "yOffsetInPxl " << yOffsetInPxl << '\n'; */
/*   //  std::cout << "xSizeInPxl   " << xSizeInPxl << '\n'; */
/*   //  std::cout << "ySizeInPxl   " << ySizeInPxl << '\n'; */

/*   char* intbufRed = new char[xSizeInPxl * ySizeInPxl]; */
/*   char* intbufGreen = new char[xSizeInPxl * ySizeInPxl]; */
/*   char* intbufBlue = new char[xSizeInPxl * ySizeInPxl]; */

/*   CPLErr err = m_bandRed->RasterIO(GF_Read, xOffsetInPxl, yOffsetInPxl, */
/*                                    xSizeInPxl, ySizeInPxl, intbufRed, */
/*                                    xSizeInPxl, ySizeInPxl, GDT_Byte, 0, 0);
 */
/*   CPLErr err1 = m_bandGreen->RasterIO(GF_Read, xOffsetInPxl, yOffsetInPxl,
 */
/*                                       xSizeInPxl, ySizeInPxl, intbufGreen,
 */
/*                                       xSizeInPxl, ySizeInPxl, GDT_Byte, 0,
 * 0); */
/*   CPLErr err2 = m_bandBlue->RasterIO(GF_Read, xOffsetInPxl, yOffsetInPxl,
 */
/*                                      xSizeInPxl, ySizeInPxl, intbufBlue, */
/*                                      xSizeInPxl, ySizeInPxl, GDT_Byte, 0,
 * 0);
 */

/*   if (err || err1 || err2) */
/*     std::cout << "ERROR" << std::endl; */

/*   char* bufRed = intbufRed; */
/*   char* bufGreen = intbufGreen; */
/*   char* bufBlue = intbufBlue; */

/*   assert(bufRed != nullptr && bufGreen != nullptr && bufBlue != nullptr);
 */

/*   // Copy the data to a rgb image. */
/*   unsigned char* leimage = new unsigned char[3 * xSizeInPxl * ySizeInPxl];
 */
/*   size_t pxlidx = 0; */
/*   for (size_t v = 0; v < ySizeInPxl; v++) { */
/*     for (size_t u = 0; u < xSizeInPxl; u++) { */
/*       leimage[pxlidx * 3] = bufRed[pxlidx]; */
/*       leimage[(pxlidx * 3) + 1] = bufGreen[pxlidx]; */
/*       leimage[(pxlidx * 3) + 2] = bufBlue[pxlidx]; */
/*       pxlidx++; */
/*     } */
/*   } */

/*   // Close the GDAL provider. */
/*   GDALClose(m_vrtGDALDataSet); */

/*   const int bytesPerLine = 3 * xSizeInPxl; */
/*   QImage leqimage(leimage, xSizeInPxl, ySizeInPxl, bytesPerLine, */
/*                   QImage::Format_RGB888); */
/*   //  std::cout << "Size of the image is: " << (leqimage.byteCount()) /
 * 1024
 * / */
/*   //  1024 */
/*   //            << "MB" << std::endl; */

/*   return { leqimage, xMetersPerPixel }; */
/* } */

enum class ARS300 : uint16_t
{
  NRS = 3400,
  FRS = 1700
};
}
