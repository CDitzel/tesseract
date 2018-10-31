#include "models/

// RadarModel::RadarModel(const SubdirectoryHandler &snaphsot) {
//  calculateRadarGrid(1, 1, 1);
//  calculateRadarMesh(1);
//}

// RadarModel::RadarModel()
//{
//  radarMeshFar.resize(meshSize_ - 1);
//  out(radarMeshFar.size());
//  calculateRadarGrid(1, 1, 1);
//  calculateRadarMesh(1);
//}

int RadarModel::ctr = 0;

void
RadarModel::calculateRadarGrid(double egoE, double egoN, double egoPhi)
{
  double radarMountingPos{ 0 };
  double const sinCar{ sin(egoPhi) * radarMountingPos + egoN };
  double const cosCar{ cos(egoPhi) * radarMountingPos + egoE };

  int cellIndex{ 0 }; // skip the first row as typical for automotive radars
  for (float r = 2; r < 200; ++r) {
    for (float az = 0; az < 17; ++az) {
      radarGridFar[cellIndex][0] = {
        r * cos(azimuthAnglesFar[az] - halfBeamAngle + egoPhi) + cosCar,
        r * sin(azimuthAnglesFar[az] - halfBeamAngle + egoPhi) + sinCar
      }; // bottom left

      radarGridFar[cellIndex][1] = {
        r * cos(azimuthAnglesFar[az] + halfBeamAngle + egoPhi) + cosCar,
        r * sin(azimuthAnglesFar[az] + halfBeamAngle + egoPhi) + sinCar
      }; // bottom right

      radarGridFar[cellIndex][2] = {
        (r + 1) * cos(azimuthAnglesFar[az] + halfBeamAngle + egoPhi) + cosCar,
        (r + 1) * sin(azimuthAnglesFar[az] + halfBeamAngle + egoPhi) + sinCar
      }; // top right

      radarGridFar[cellIndex][3] = {
        (r + 1) * cos(azimuthAnglesFar[az] - halfBeamAngle + egoPhi) + cosCar,
        (r + 1) * sin(azimuthAnglesFar[az] - halfBeamAngle + egoPhi) + sinCar
      }; // top left

      ++cellIndex;
    }
  }
}

void
RadarModel::calculateRadarMesh(size_t scaleFactor)
{
  uint16_t t{ 0 };
  uint16_t colorIndex{ 0 };
  uint16_t voxelIndex{ 0 };
  uint32_t i = 0;

  for (auto& patch : radarGridFar) {
    ++voxelIndex;
    //         for (size_t i = 0; i < 4; ++i) {
    //           voxelColorFar[colorIndex++] =
    //             currentImg_far[voxelIndex] <= 0.57147
    //               ? 446.22 * currentImg_far[voxelIndex] / 255.0
    //               : 1.0f;
    //           voxelColorFar[colorIndex++] =
    //             currentImg_far[voxelIndex] <= 0.95
    //               ? ((currentImg_far[voxelIndex] - 0.6) * 728.57) / 255.0
    //               : 1.0f;
    //           voxelColorFar[colorIndex++] = 0.0f;
    //           voxelColorFar[colorIndex++] = 1.0f;
    //         }
    // x,y,z
    // x--------x
    // |   <3   |
    // |4      2|
    // |   1>   |
    // x--------x
    //    radarMeshFar[i++] = patch[0].first;
    //    radarMeshFar[i++] = patch[0].second;
    //    radarMeshFar[i++] = 0.f;
    //    //--1
    //    radarMeshFar[i++] = patch[1].first;
    //    radarMeshFar[i++] = patch[1].second;
    //    radarMeshFar[i++] = 0.f;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[1].first;
    //    radarMeshFar[i++] = patch[1].second;
    //    radarMeshFar[i++] = 0.f;
    //    //--2
    //    radarMeshFar[i++] = patch[1].first;
    //    radarMeshFar[i++] = patch[1].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[1].first;
    //    radarMeshFar[i++] = patch[1].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //--3
    //    radarMeshFar[i++] = patch[0].first;
    //    radarMeshFar[i++] = patch[0].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[0].first;
    //    radarMeshFar[i++] = patch[0].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //--4
    //    radarMeshFar[i++] = patch[0].first;
    //    radarMeshFar[i++] = patch[0].second;
    //    radarMeshFar[i++] = 0.f;
    //    // ----
    //    // x,y,z
    //    //    x--------x
    //    //    |   3>   |
    //    //    | 2    4 |
    //    // x-- -----x  |
    //    // |        |--x
    //    // |   	    |
    //    // |        |
    //    // x--------x
    //    radarMeshFar[i++] = patch[2].first;
    //    radarMeshFar[i++] = patch[2].second;
    //    radarMeshFar[i++] = 0.f;
    //    //--1
    //    radarMeshFar[i++] = patch[3].first;
    //    radarMeshFar[i++] = patch[3].second;
    //    radarMeshFar[i++] = 0.f;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[3].first;
    //    radarMeshFar[i++] = patch[3].second;
    //    radarMeshFar[i++] = 0.f;
    //    //--2
    //    radarMeshFar[i++] = patch[3].first;
    //    radarMeshFar[i++] = patch[3].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[3].first;
    //    radarMeshFar[i++] = patch[3].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //--3
    //    radarMeshFar[i++] = patch[2].first;
    //    radarMeshFar[i++] = patch[2].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[2].first;
    //    radarMeshFar[i++] = patch[2].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //--4
    //    radarMeshFar[i++] = patch[2].first;
    //    radarMeshFar[i++] = patch[2].second;
    //    radarMeshFar[i++] = 0.f;
    //    // ----
    //    // x,y,z
    //    //    x--------x
    //    //   / /      / /
    //    //  / 4      / 3
    //    // x-/------x /|
    //    // |  x-----|--x
    //    // | / / 	| / /
    //    // |/ 2     |/ 1
    //    // x-/------x /
    //    radarMeshFar[i++] = patch[1].first;
    //    radarMeshFar[i++] = patch[1].second;
    //    radarMeshFar[i++] = 0.f;
    //    //--1
    //    radarMeshFar[i++] = patch[2].first;
    //    radarMeshFar[i++] = patch[2].second;
    //    radarMeshFar[i++] = 0.f;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[0].first;
    //    radarMeshFar[i++] = patch[0].second;
    //    radarMeshFar[i++] = 0.f;
    //    //--2
    //    radarMeshFar[i++] = patch[3].first;
    //    radarMeshFar[i++] = patch[3].second;
    //    radarMeshFar[i++] = 0.f;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[1].first;
    //    radarMeshFar[i++] = patch[1].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //--3
    //    radarMeshFar[i++] = patch[2].first;
    //    radarMeshFar[i++] = patch[2].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //-------------------------------
    //    radarMeshFar[i++] = patch[0].first;
    //    radarMeshFar[i++] = patch[0].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;
    //    //--4
    //    radarMeshFar[i++] = patch[3].first;
    //    radarMeshFar[i++] = patch[3].second;
    //    radarMeshFar[i++] = 0.4 * scaleFactor;

    //-----------------------------------------
    //---------------- faces ------------------
    //-----------------------------------------
    //-- Left lower vertices for alle six patches
    // front quad
    meshFacesFar[0][t] = patch[0].first;
    meshFacesFar[0][t + 1] = patch[0].second;
    meshFacesFar[0][t + 2] = 0.0f;
    // back quad
    meshFacesFar[1][t] = patch[2].first;
    meshFacesFar[1][t + 1] = patch[2].second;
    meshFacesFar[1][t + 2] = 0.0f;
    // top quad
    meshFacesFar[2][t] = patch[3].first;
    meshFacesFar[2][t + 1] = patch[3].second;
    meshFacesFar[2][t + 2] = 0.4 * scaleFactor;
    // bottom quad
    meshFacesFar[3][t] = patch[1].first;
    meshFacesFar[3][t + 1] = patch[1].second;
    meshFacesFar[3][t + 2] = 0.0f;
    // left quad
    meshFacesFar[4][t] = patch[1].first;
    meshFacesFar[4][t + 1] = patch[1].second;
    meshFacesFar[4][t + 2] = 0.4 * scaleFactor;
    // right quad
    meshFacesFar[5][t] = patch[3].first;
    meshFacesFar[5][t + 1] = patch[3].second;
    meshFacesFar[5][t + 2] = 0.4 * scaleFactor;
    t += 3;
    //-- Right lower vertices for alle six patches
    // front quad
    meshFacesFar[0][t] = patch[1].first;
    meshFacesFar[0][t + 1] = patch[1].second;
    meshFacesFar[0][t + 2] = 0.0f;
    // back quad
    meshFacesFar[1][t] = patch[3].first;
    meshFacesFar[1][t + 1] = patch[3].second;
    meshFacesFar[1][t + 2] = 0.0f;
    // top quad
    meshFacesFar[2][t] = patch[0].first;
    meshFacesFar[2][t + 1] = patch[0].second;
    meshFacesFar[2][t + 2] = 0.4 * scaleFactor;
    // bottom quad
    meshFacesFar[3][t] = patch[0].first;
    meshFacesFar[3][t + 1] = patch[0].second;
    meshFacesFar[3][t + 2] = 0.0f;
    // left quad
    meshFacesFar[4][t] = patch[1].first;
    meshFacesFar[4][t + 1] = patch[1].second;
    meshFacesFar[4][t + 2] = 0.0f;
    // right quad
    meshFacesFar[5][t] = patch[3].first;
    meshFacesFar[5][t + 1] = patch[3].second;
    meshFacesFar[5][t + 2] = 0.0f;
    t += 3;
    //-- Right upper vertices for alle six patches
    // front quad
    meshFacesFar[0][t] = patch[1].first;
    meshFacesFar[0][t + 1] = patch[1].second;
    meshFacesFar[0][t + 2] = 0.4 * scaleFactor;
    // back quad
    meshFacesFar[1][t] = patch[3].first;
    meshFacesFar[1][t + 1] = patch[3].second;
    meshFacesFar[1][t + 2] = 0.4;
    // top quad
    meshFacesFar[2][t] = patch[1].first;
    meshFacesFar[2][t + 1] = patch[1].second;
    meshFacesFar[2][t + 2] = 0.4;
    // bottom quad
    meshFacesFar[3][t] = patch[3].first;
    meshFacesFar[3][t + 1] = patch[3].second;
    meshFacesFar[3][t + 2] = 0.0f;
    // left quad
    meshFacesFar[4][t] = patch[2].first;
    meshFacesFar[4][t + 1] = patch[2].second;
    meshFacesFar[4][t + 2] = 0.0f;
    // right quad
    meshFacesFar[5][t] = patch[0].first;
    meshFacesFar[5][t + 1] = patch[0].second;
    meshFacesFar[5][t + 2] = 0.0f;
    t += 3;
    //-- Left upper vertices for alle six patches
    // front quad
    meshFacesFar[0][t] = patch[0].first;
    meshFacesFar[0][t + 1] = patch[0].second;
    meshFacesFar[0][t + 2] = 0.4 * scaleFactor;
    // back quad
    meshFacesFar[1][t] = patch[2].first;
    meshFacesFar[1][t + 1] = patch[2].second;
    meshFacesFar[1][t + 2] = 0.4 * scaleFactor;
    // top quad
    meshFacesFar[2][t] = patch[2].first;
    meshFacesFar[2][t + 1] = patch[2].second;
    meshFacesFar[2][t + 2] = 0.4 * scaleFactor;
    // bottom quad
    meshFacesFar[3][t] = patch[2].first;
    meshFacesFar[3][t + 1] = patch[2].second;
    meshFacesFar[3][t + 2] = 0.0f;
    // left quad
    meshFacesFar[4][t] = patch[2].first;
    meshFacesFar[4][t + 1] = patch[2].second;
    meshFacesFar[4][t + 2] = 0.4 * scaleFactor;
    // right quad
    meshFacesFar[5][t] = patch[0].first;
    meshFacesFar[5][t + 1] = patch[0].second;
    meshFacesFar[5][t + 2] = 0.4 * scaleFactor;
    t += 3;
  }
}

void
draw(const RadarModel&, std::ostream& out, size_t position)
{
  out << std::string(position, ' ') << "RadarModel" << std::endl;
}

void
display(const RadarModel& radarModel, View& view)
{
  //  view.displayRadar(radarModel);
  std::cout << "Display of RadarModel" << std::endl;
};
